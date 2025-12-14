#include "cpu.h"

// Registers class implementation
bool Registers::is_flag_set(uint8_t flag) {
    return (this->f & flag) != 0;
}

void Registers::set_flags(uint8_t flags, bool state) {
    if (state) {
        this->f |= flags;  // Set bits
    } else {
        this->f &= ~flags; // Clear bits
    }
}

void Registers::print_flags() {
    std::cout << "Z: " << std::hex << +is_flag_set(Flags::ZERO) << " N: " << std::hex << +is_flag_set(Flags::SUBTRACT)
              << " H: " << std::hex << +is_flag_set(Flags::HALF_CARRY) << " C: " << std::hex << +is_flag_set(Flags::CARRY)
              << std::endl;
}

void Registers::print_registers() {
    std::cout << "A: " << std::hex << +a << " F: " << std::hex << +f << std::endl;
    std::cout << "B: " << std::hex << +b << " C: " << std::hex << +c << std::endl;
    std::cout << "D: " << std::hex << +d << " E: " << std::hex << +e << std::endl;
    std::cout << "H: " << std::hex << +h << " L: " << std::hex << +l << std::endl;

    std::cout << "SP: " << std::hex << +sp << std::endl;
    std::cout << "PC: " << std::hex << +pc << std::endl;
    std::cout << "\n" << std::endl;
}

// CPU class implementation
CPU::CPU(Registers* registers, Interrupts* interrupts, Memory* memory) {
    this->memory = memory;
    this->interrupts = interrupts;
    this->registers = registers;
    this->instructions = std::make_unique<InstructionSet>(registers, interrupts, memory);
}

void CPU::no_bootrom_init() {
    registers->a = CPUInitState::DEFAULT_A;
    registers->f = CPUInitState::DEFAULT_F;
    registers->b = CPUInitState::DEFAULT_B;
    registers->c = CPUInitState::DEFAULT_C;
    registers->d = CPUInitState::DEFAULT_D;
    registers->e = CPUInitState::DEFAULT_E;
    registers->h = CPUInitState::DEFAULT_H;
    registers->l = CPUInitState::DEFAULT_L;

    registers->sp = CPUInitState::DEFAULT_SP;
    registers->pc = CPUInitState::DEFAULT_PC;

    registers->set_flags(Flags::ZERO | Flags::HALF_CARRY | Flags::CARRY, true);
    registers->set_flags(Flags::SUBTRACT, false);

    memory->memory_flags.rom_disabled = true;
    memory->system_memory[0xFF0F] = 0xE1;
    memory->write_memory_byte(MemoryInitState::LCD_STAT_ADDR, MemoryInitState::LCD_STAT_VALUE);
    memory->write_memory_byte(MemoryInitState::LCD_CONTROL_ADDR, MemoryInitState::LCD_CONTROL_VALUE);

    memory->timer_registers.div_counter = 0xD3;
    memory->timer_registers.timer_counter = 0x00;
    memory->timer_registers.timer_modulo = 0x00;
    memory->timer_registers.timer_control = 0xF8;
}

void CPU::step() {
    if (memory->memory_flags.cpu_halted) {
        memory->cpu_clock.instruction_cycles = CPUConstants::HALT_CYCLES;
        return;
    }

    uint8_t instruction = this->memory->read_memory_byte(registers->pc);

    if (!memory->memory_flags.halt_bug_triggered)
        registers->pc++;

    memory->memory_flags.halt_bug_triggered = false;

    instructions->execute(instruction);
}

// Timer class implementation
// Define the timer frequency threshold lookup table
const int Timer::TIMER_THRESHOLDS[4] = {1024, 16, 64, 256};

Timer::Timer(Memory* mmu, Interrupts* interrupts) {
    this->mmu = mmu;
    this->interrupts = interrupts;
}

void Timer::inc() {
    mmu->cpu_clock.total_cycles += mmu->cpu_clock.instruction_cycles;
    mmu->cpu_clock.total_cycles %= TimerConstants::GAMEBOY_TICKS;

    div += mmu->cpu_clock.instruction_cycles;

    while (div >= TimerConstants::DIV_THRESHOLD) {
        div -= TimerConstants::DIV_THRESHOLD;
        mmu->timer_registers.div_counter++;
    }

    check();
}

void Timer::check() {
    if (mmu->timer_registers.timer_control & TimerConstants::TIMER_ENABLE_BIT) {
        tima += mmu->cpu_clock.instruction_cycles;

        // Use lookup table instead of switch statement
        int threshold = TIMER_THRESHOLDS[mmu->timer_registers.timer_control & TimerConstants::FREQUENCY_MASK];
        
        while (tima >= threshold) {
            tima -= threshold;
            if (mmu->timer_registers.timer_counter == TimerConstants::TIMER_OVERFLOW) {
                mmu->timer_registers.timer_counter = mmu->read_memory_byte(TimerConstants::TIMER_MODULO_ADDR);
                interrupts->set_interrupt_flag(INTERRUPT_TIMER);
            } else {
                mmu->timer_registers.timer_counter++;
            }
        }
    }
}

// Interrupts class implementation
// Static interrupt descriptor table
const InterruptDescriptor Interrupts::interrupt_descriptors[5] = {
    {INTERRUPT_VBLANK, 0x40, true},   // Priority 0 - VBlank
    {INTERRUPT_LCD, 0x48, true},      // Priority 1 - LCD
    {INTERRUPT_TIMER, 0x50, true},    // Priority 2 - Timer
    {INTERRUPT_SERIAL, 0x58, false},  // Priority 3 - Serial (not typically used)
    {INTERRUPT_JOYPAD, 0x60, true}    // Priority 4 - Joypad
};

Interrupts::Interrupts(Registers *registers, Memory *mmu) {
    this->mmu = mmu;
    this->registers = registers;
    
    // Initialize interrupt handler function pointers
    interrupt_handlers[0] = &Interrupts::handle_vblank;
    interrupt_handlers[1] = &Interrupts::handle_lcd;
    interrupt_handlers[2] = &Interrupts::handle_timer;
    interrupt_handlers[3] = &Interrupts::handle_serial;
    interrupt_handlers[4] = &Interrupts::handle_joypad;
}

void Interrupts::set_master_flag(bool state) { this->IME = state ? 1 : 0; }

bool Interrupts::is_master_enabled() { return this->IME & 1U; }

void Interrupts::set_interrupt_flag(uint8_t flag) {
    uint8_t IF_value = mmu->read_memory_byte(0xFF0F);
    IF_value |= flag;
    return mmu->write_memory_byte(0xFF0F, IF_value);
}

void Interrupts::unset_interrupt_flag(uint8_t flag) {
    uint8_t IF_value = mmu->read_memory_byte(0xFF0F);
    IF_value &= ~flag;
    return mmu->write_memory_byte(0xFF0F, IF_value);
}

bool Interrupts::is_interrupt_enabled(uint8_t flag) { return (mmu->read_memory_byte(0xFFFF) & flag); }

bool Interrupts::is_interrupt_flag_set(uint8_t flag) { return (mmu->read_memory_byte(0xFF0F) & flag); }

bool Interrupts::check() {
    if (mmu->read_memory_byte(0xFFFF) & mmu->read_memory_byte(0xFF0F) & 0x0F)
        mmu->memory_flags.cpu_halted = false;

    if (!this->is_master_enabled())
        return false;

    // Check interrupts in priority order using function pointer table
    for (int i = 0; i < 5; i++) {
        const InterruptDescriptor& desc = interrupt_descriptors[i];
        if (desc.enabled && interrupt_handlers[i] != nullptr) {
            if ((this->*interrupt_handlers[i])()) {
                return true;
            }
        }
    }

    return false;
}

void Interrupts::trigger_interrupt(InterruptFlags interrupt, uint8_t jump_pc) {
    this->mmu->write_stack_short(&registers->sp, this->registers->pc);
    this->registers->pc = jump_pc;
    this->set_master_flag(false);
    this->unset_interrupt_flag(interrupt);
    mmu->memory_flags.cpu_halted = false;

    mmu->cpu_clock.instruction_cycles = 20;
}

// Individual interrupt handler methods
bool Interrupts::handle_vblank() {
    if (this->is_interrupt_enabled(INTERRUPT_VBLANK) && this->is_interrupt_flag_set(INTERRUPT_VBLANK)) {
        this->trigger_interrupt(INTERRUPT_VBLANK, 0x40);
        return true;
    }
    return false;
}

bool Interrupts::handle_lcd() {
    if (this->is_interrupt_enabled(INTERRUPT_LCD) && this->is_interrupt_flag_set(INTERRUPT_LCD)) {
        this->trigger_interrupt(INTERRUPT_LCD, 0x48);
        return true;
    }
    return false;
}

bool Interrupts::handle_timer() {
    if (this->is_interrupt_enabled(INTERRUPT_TIMER) && this->is_interrupt_flag_set(INTERRUPT_TIMER)) {
        this->trigger_interrupt(INTERRUPT_TIMER, 0x50);
        return true;
    }
    return false;
}

bool Interrupts::handle_serial() {
    if (this->is_interrupt_enabled(INTERRUPT_SERIAL) && this->is_interrupt_flag_set(INTERRUPT_SERIAL)) {
        this->trigger_interrupt(INTERRUPT_SERIAL, 0x58);
        return true;
    }
    return false;
}

bool Interrupts::handle_joypad() {
    if (this->is_interrupt_enabled(INTERRUPT_JOYPAD) && this->is_interrupt_flag_set(INTERRUPT_JOYPAD)) {
        this->trigger_interrupt(INTERRUPT_JOYPAD, 0x60);
        return true;
    }
    return false;
}
