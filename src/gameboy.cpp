#include "gameboy.h"

void Gameboy::init(std::string rom, std::string save_file, bool debug) {
    Cartridge *cartridge = new Cartridge(rom, save_file);
    GameboyConfig config = {debug, save_file};
    this->init(cartridge, config);
}

void Gameboy::init(Cartridge *cartridge, const GameboyConfig& config) {
    this->cartridge = cartridge;
    status.flags.debug = config.debug;
    
    create_core_components(cartridge);
    
    mmu->load_default_system_rom();

    create_optional_components(config);
    
    status.flags.isRunning = GameboyConstants::DEFAULT_RUNNING_STATE;
}

void Gameboy::create_core_components(Cartridge* cartridge) {
    mmu = std::make_unique<Memory>(cartridge);
    interrupts = std::make_unique<Interrupts>(&registers, mmu.get());
    cpu = std::make_unique<CPU>(&registers, interrupts.get(), mmu.get());
    ppu = std::make_unique<PPU>(&registers, interrupts.get(), mmu.get());
    timer = std::make_unique<Timer>(mmu.get(), interrupts.get());
    joypad = std::make_unique<Joypad>(&status, interrupts.get(), mmu.get());
}

void Gameboy::create_optional_components(const GameboyConfig& config) {
    if (config.debug)
        renderer = std::make_unique<DebugRenderer>(&status, cpu.get(), ppu.get(), &registers, interrupts.get(), mmu.get());
    else
        renderer = std::make_unique<Renderer>(&status, cpu.get(), ppu.get(), &registers, interrupts.get(), mmu.get());
    renderer->init();
}

bool Gameboy::run_step() {
    if (!status.flags.isPaused || status.flags.doStep) {
        mmu->cpu_clock.instruction_cycles = GameboyConstants::DEFAULT_INSTRUCTION_CYCLES;
        bool interrupted = interrupts->check();
        if (!interrupted)
            cpu->step();

        timer->inc();
        ppu->step();
    }

    status.flags.doStep = GameboyConstants::DEFAULT_STEP_STATE;
    joypad->check(mmu->cpu_clock.instruction_cycles);

    if (ppu->can_render || status.flags.isPaused) {
        renderer->render();
        ppu->can_render = false;
        return true;
    }
    return false;
}

void Gameboy::run() {
    while (status.flags.isRunning) {
        this->run_step();
    }
}

void Gameboy::run_until_next_frame() {
    while (!this->run_step()) {
    }
}
