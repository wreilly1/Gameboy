#pragma once

#include "memory.h"
#include "instructions.h"
#include <memory>
#include <stdint.h>
#include <fstream>
#include <iostream>

namespace Flags {
    constexpr uint8_t ZERO = 0x80;
    constexpr uint8_t SUBTRACT = 0x40;
    constexpr uint8_t HALF_CARRY = 0x20;
    constexpr uint8_t CARRY = 0x10;
}

class Registers {
   public:
    uint16_t pc = 0;  // Program Counter/Pointer
    uint16_t sp;      // Stack pointer

    struct {
        union {
            struct {
                uint8_t f;
                uint8_t a;
            };
            uint16_t af;
        };
    };

    struct {
        union {
            struct {
                uint8_t c;
                uint8_t b;
            };
            uint16_t bc;
        };
    };

    struct {
        union {
            struct {
                uint8_t e;
                uint8_t d;
            };
            uint16_t de;
        };
    };

    struct {
        union {
            struct {
                uint8_t l;
                uint8_t h;
            };
            uint16_t hl;
        };
    };

    void set_flags(uint8_t flags, bool state);
    bool is_flag_set(uint8_t flag);

    // Debug
    void print_flags();
    void print_registers();
};

// Timer configuration constants
struct TimerConstants {
    static const int DIV_THRESHOLD = 256;
    static const int GAMEBOY_TICKS = 4 * 1024 * 1024;  // 4194304
    static const uint8_t TIMER_ENABLE_BIT = 0x04;
    static const uint8_t FREQUENCY_MASK = 0x03;
    static const uint8_t TIMER_OVERFLOW = 0xFF;
    static const uint16_t TIMER_MODULO_ADDR = 0xFF06;
};

class Timer {
    Registers *registers;
    Memory *mmu;
    Interrupts *interrupts;

    int div = 0;
    int tac = 0;
    int tima = 0;

    // Timer frequency threshold lookup table
    static const int TIMER_THRESHOLDS[4];

    void check();

   public:
    Timer(Memory *memory, Interrupts *interrupts);

    void inc();
};

enum InterruptFlags {
    INTERRUPT_VBLANK = (1 << 0),
    INTERRUPT_LCD = (1 << 1),
    INTERRUPT_TIMER = (1 << 2),
    INTERRUPT_SERIAL = (1 << 3),
    INTERRUPT_JOYPAD = (1 << 4)
};

struct InterruptDescriptor {
    uint8_t flag;
    uint8_t jump_address;
    bool enabled;
};

class Interrupts {
    Registers* registers;
    Memory* mmu;

    // Function pointer typedef for interrupt handlers
    typedef bool (Interrupts::*InterruptHandlerFunc)();
    
    // Interrupt handler table
    static const InterruptDescriptor interrupt_descriptors[5];
    InterruptHandlerFunc interrupt_handlers[5];
    
    // Individual interrupt handler methods
    bool handle_vblank();
    bool handle_lcd();
    bool handle_timer();
    bool handle_serial();
    bool handle_joypad();

   public:
    bool IME;

    Interrupts(Registers* registers, Memory* mmu);

    bool check();
    void set_master_flag(bool state);
    bool is_master_enabled();
    void set_interrupt_flag(uint8_t flag);
    void unset_interrupt_flag(uint8_t flag);
    bool is_interrupt_enabled(uint8_t flag);
    bool is_interrupt_flag_set(uint8_t flag);
    void trigger_interrupt(InterruptFlags interrupt, uint8_t jump_pc);
};

// CPU initialization constants structure
struct CPUInitState {
    static const uint8_t DEFAULT_A = 0x01;
    static const uint8_t DEFAULT_F = 0xb0;
    static const uint8_t DEFAULT_B = 0x00;
    static const uint8_t DEFAULT_C = 0x13;
    static const uint8_t DEFAULT_D = 0x00;
    static const uint8_t DEFAULT_E = 0xd8;
    static const uint8_t DEFAULT_H = 0x01;
    static const uint8_t DEFAULT_L = 0x4d;
    static const uint16_t DEFAULT_SP = 0xFFFE;
    static const uint16_t DEFAULT_PC = 0x0100;
};

// Memory initialization constants structure
struct MemoryInitState {
    static const uint16_t LCD_STAT_ADDR = 0xFF41;
    static const uint8_t LCD_STAT_VALUE = 0x80;
    static const uint16_t LCD_CONTROL_ADDR = 0xFF40;
    static const uint8_t LCD_CONTROL_VALUE = 0x91;
    static const uint16_t SOUND_CONTROL_ADDR = 0xFF26;
    static const uint8_t SOUND_CONTROL_VALUE = 0xF1;
    static const uint16_t SOUND_CH1_ADDR = 0xFF10;
    static const uint8_t SOUND_CH1_VALUE = 0x80;
    static const uint16_t SOUND_CH2_ADDR = 0xFF16;
    static const uint8_t SOUND_CH2_VALUE = 0x3F;
    static const uint16_t SOUND_CH3_ADDR = 0xFF1A;
    static const uint8_t SOUND_CH3_VALUE = 0x7F;
    static const uint16_t SOUND_CH4_ADDR = 0xFF20;
    static const uint8_t SOUND_CH4_VALUE = 0xFF;
};

// CPU timing and state constants
struct CPUConstants {
    static const int HALT_CYCLES = 4;
    static const int DEFAULT_CYCLES = 1;
    static const int INTERRUPT_CYCLES = 5;
};

class CPU {
    std::unique_ptr<InstructionSet> instructions;
    Memory *memory;
    Interrupts *interrupts;
    Registers *registers;

   public:
    CPU(Registers *registers, Interrupts *interrupts, Memory *memory);

    void reset();
    void step();
    void no_bootrom_init();
    void print_flags();
    void print_debug();
};
