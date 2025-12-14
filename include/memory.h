#pragma once
#include "cartridge.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

#include <string>
#include <sstream>

namespace GameBoySystem {
    struct Colour {
        union {
            struct {
                uint8_t r, g, b, a;
            };
            uint8_t colours[4];
        };
    };

    namespace Timing {
        struct ClockData {
            int total_cycles = 0;
            int instruction_cycles = 0;
        };

        struct TimerRegisters {
            uint16_t div_counter = 0;
            uint8_t timer_counter = 0;
            uint8_t timer_modulo = 0;
            uint8_t timer_control = 0;
        };
    }

    namespace Graphics {
        struct SpriteAttributes {
            bool is_active;
            int y_position;
            int x_position;
            uint8_t tile_number;
            Colour* color_palette;
            
            struct AttributeFlags {
                union {
                    struct {
                        uint8_t gbc_palette_low : 1;
                        uint8_t gbc_palette_mid : 1;
                        uint8_t gbc_palette_high : 1;
                        uint8_t vram_bank : 1;
                        uint8_t dmg_palette : 1;
                        uint8_t flip_x : 1;
                        uint8_t flip_y : 1;
                        uint8_t bg_priority : 1;
                    };
                    uint8_t raw_value;
                };
            } flags;
        };

        struct TileData {
            uint8_t pixel_matrix[8][8] = {0};
        };

        constexpr Colour DEFAULT_PALETTE[4] = {
            {255, 255, 255, 255}, // White
            {192, 192, 192, 255}, // Light gray
            {96, 96, 96, 255},    // Dark gray
            {0, 0, 0, 255}        // Black
        };
    }

    namespace Input {
        struct ControllerState {
            uint8_t button_data = 0xFF;
        };
    }

    namespace Memory {
        struct SystemFlags {
            bool rom_disabled = false;
            bool cpu_halted = false;
            bool halt_bug_triggered = false;
            bool color_mode_enabled = false;
        };
    }
}

class Memory {
   private:
    uint8_t gameboy_system_rom[0x100] = {
        // 0x0000: LD SP, $FFFE - Initialize stack pointer
        0x31, 0xFE, 0xFF,
        // 0x0003: XOR A - Clear accumulator  
        0xAF,
        // 0x0004: LD HL, $9FFF - Point to end of VRAM
        0x21, 0xFF, 0x9F,
        // 0x0007: LD (HL-), A - Clear VRAM loop start
        0x32,
        // 0x0008: BIT 7, H - Check if still in VRAM range
        0xCB, 0x7C,
        // 0x000A: JR NZ, -5 - Loop back to clear more VRAM
        0x20, 0xFB,
        // 0x000C: LD HL, $FF26 - Audio register
        0x21, 0x26, 0xFF,
        // 0x000F: LD C, $11 - Audio channel register offset
        0x0E, 0x11,
        // 0x0011: LD A, $80 - Audio enable value
        0x3E, 0x80,
        // 0x0013: LD (HL-), A - Enable audio and dec HL
        0x32,
        // 0x0014: LD ($FF00+C), A - Write to audio register
        0xE2,
        // 0x0015: INC C - Next audio register
        0x0C,
        // 0x0016: LD A, $F3 - Audio config value
        0x3E, 0xF3,
        // 0x0018: LD ($FF00+C), A - Write audio config
        0xE2,
        // 0x0019: LD (HL-), A - Write and dec HL
        0x32,
        // 0x001A: LD A, $77 - Another audio value
        0x3E, 0x77,
        // 0x001C: LD (HL), A - Write to audio register
        0x77,
        // 0x001D: LD A, $FC - Palette data
        0x3E, 0xFC,
        // 0x001F: LD ($FF47), A - Set background palette
        0xE0, 0x47,
        // 0x0021: LD DE, $0104 - Nintendo logo in cartridge
        0x11, 0x04, 0x01,
        // 0x0024: LD HL, $8010 - VRAM destination
        0x21, 0x10, 0x80,
        // 0x0027: LD A, (DE) - Load logo byte
        0x1A,
        // 0x0028: CALL $0095 - Decompress and write pixel data
        0xCD, 0x95, 0x00,
        // 0x002B: CALL $0096 - Decompress and write more pixel data
        0xCD, 0x96, 0x00,
        // 0x002E: INC DE - Next logo byte
        0x13,
        // 0x002F: LD A, E - Check if done
        0x7B,
        // 0x0030: CP $34 - Compare to end address
        0xFE, 0x34,
        // 0x0032: JR NZ, -15 - Loop if not done
        0x20, 0xF3,
        // 0x0034: LD DE, $00D8 - Tile data for 'R'
        0x11, 0xD8, 0x00,
        // 0x0037: LD B, $08 - 8 bytes to copy
        0x06, 0x08,
        // 0x0039: LD A, (DE) - Load byte
        0x1A,
        // 0x003A: INC DE - Next byte
        0x13,
        // 0x003B: LD (HL+), A - Store and inc HL
        0x22,
        // 0x003C: INC HL - Skip a byte
        0x23,
        // 0x003D: DEC B - Dec counter
        0x05,
        // 0x003E: JR NZ, -7 - Loop if not done
        0x20, 0xF9,
        // Tile map setup
        0x3E, 0x19,       // LD A, $19
        0xEA, 0x10, 0x99, // LD ($9910), A
        0x21, 0x2F, 0x99, // LD HL, $992F
        // Tile map loop
        0x0E, 0x0C,       // LD C, $0C
        0x3D,             // DEC A
        0x28, 0x08,       // JR Z, +8
        0x32,             // LD (HL-), A
        0x0D,             // DEC C
        0x20, 0xF9,       // JR NZ, -7
        0x2E, 0x0F,       // LD L, $0F
        0x18, 0xF3,       // JR -13
        // Scroll and sound setup
        0x67,             // LD H, A (H=0)
        0x3E, 0x64,       // LD A, $64
        0x57,             // LD D, A
        0xE0, 0x42,       // LD ($FF42), A
        0x3E, 0x91,       // LD A, $91
        0xE0, 0x40,       // LD ($FF40), A - Turn on LCD
        0x04,             // INC B
        // Main scroll loop
        0x1E, 0x02,       // LD E, $02
        0x0E, 0x0C,       // LD C, $0C
        // Wait for vblank
        0xF0, 0x44,       // LD A, ($FF44)
        0xFE, 0x90,       // CP $90
        0x20, 0xFA,       // JR NZ, -6
        0x0D,             // DEC C
        0x20, 0xF7,       // JR NZ, -9
        0x1D,             // DEC E
        0x20, 0xF2,       // JR NZ, -14
        0x0E, 0x13,       // LD C, $13
        0x24,             // INC H
        0x7C,             // LD A, H
        0x1E, 0x83,       // LD E, $83
        0xFE, 0x62,       // CP $62 - Sound trigger 1
        0x28, 0x06,       // JR Z, +6
        0x1E, 0xC1,       // LD E, $C1
        0xFE, 0x64,       // CP $64 - Sound trigger 2
        0x20, 0x06,       // JR NZ, +6
        0x7B,             // LD A, E
        0xE2,             // LD ($FF00+C), A
        0x0C,             // INC C
        0x3E, 0x87,       // LD A, $87
        0xE2,             // LD ($FF00+C), A
        0xF0, 0x42,       // LD A, ($FF42)
        0x90,             // SUB B
        0xE0, 0x42,       // LD ($FF42), A
        0x15,             // DEC D
        0x20, 0xD2,       // JR NZ, -46
        0x05,             // DEC B
        0x20, 0x4F,       // JR NZ, +79
        0x16, 0x20,       // LD D, $20
        0x18, 0xCB,       // JR -53
        // Decompression routines at 0x0095-0x00A7
        0x4F,             // LD C, A
        0x06, 0x04,       // LD B, $04
        0xC5,             // PUSH BC
        0xCB, 0x11,       // RL C
        0x17,             // RLA
        0xC1,             // POP BC
        0xCB, 0x11,       // RL C
        0x17,             // RLA
        0x05,             // DEC B
        0x20, 0xF5,       // JR NZ, -11
        0x22,             // LD (HL+), A
        0x23,             // INC HL
        0x22,             // LD (HL+), A
        0x23,             // INC HL
        0xC9,             // RET
        // Nintendo logo data from 0x00A8-0x00D7
        0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 
        0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
        0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
        0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
        0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
        0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
        // 'R' tile data at 0x00D8-0x00DF
        0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
        // Logo check routine at 0x00E0
        0x21, 0x04, 0x01, // LD HL, $0104
        0x11, 0xA8, 0x00, // LD DE, $00A8
        0x1A,             // LD A, (DE)
        0x13,             // INC DE
        0xBE,             // CP (HL)
        0x20, 0xFE,       // JR NZ, -2 (lock up)
        0x23,             // INC HL
        0x7D,             // LD A, L
        0xFE, 0x34,       // CP $34
        0x20, 0xF5,       // JR NZ, -11
        // Header checksum
        0x06, 0x19,       // LD B, $19
        0x78,             // LD A, B
        0x86,             // ADD (HL)
        0x23,             // INC HL
        0x05,             // DEC B
        0x20, 0xFB,       // JR NZ, -5
        0x86,             // ADD (HL)
        0x20, 0xFE,       // JR NZ, -2 (lock up)
        // Disable boot ROM
        0x3E, 0x01,       // LD A, $01
        0xE0, 0x50        // LD ($FF50), A
    };

   public:
    // Core system memory and cartridge
    uint8_t system_memory[0xFFFF];
    Cartridge *game_cartridge;

    // System components using namespace organization
    GameBoySystem::Timing::ClockData cpu_clock;
    GameBoySystem::Timing::TimerRegisters timer_registers;
    GameBoySystem::Input::ControllerState controller_input;
    GameBoySystem::Memory::SystemFlags memory_flags;

    // Graphics subsystem
    GameBoySystem::Graphics::SpriteAttributes sprite_data[40] = {GameBoySystem::Graphics::SpriteAttributes{}};
    GameBoySystem::Graphics::TileData graphics_tiles[384];

    // Color palettes with modern initialization
    GameBoySystem::Colour background_colors[4] = {
        {255, 255, 255, 255}, // White
        {0, 0, 0, 255},       // Black
        {0, 0, 0, 255},       // Black
        {0, 0, 0, 255}        // Black
    };

    GameBoySystem::Colour sprite_colors_0[4] = {
        {0, 0, 0, 255},   // Transparent/Black
        {0, 0, 0, 255},   // Black
        {0, 0, 0, 255},   // Black
        {0, 0, 0, 255}    // Black
    };

    GameBoySystem::Colour sprite_colors_1[4] = {
        {0, 0, 0, 255},   // Transparent/Black
        {0, 0, 0, 255},   // Black
        {0, 0, 0, 255},   // Black
        {0, 0, 0, 255}    // Black
    };

    // Accessor methods for modern C++ style
    const auto& get_default_palette() const noexcept { 
        return GameBoySystem::Graphics::DEFAULT_PALETTE; 
    }

    Memory(Cartridge *cartridge);

    void load_default_system_rom();

    uint8_t read_memory_byte(uint16_t address);
    void write_memory_byte(uint16_t address, uint8_t value);

    uint16_t read_memory_short(uint16_t address);
    void write_memory_short(uint16_t address, uint16_t value);

    void write_stack_short(uint16_t *sp, uint16_t value);
    uint16_t read_stack_short(uint16_t *sp);

    void update_tile_data(uint16_t address, uint8_t value);
    void update_sprite_data(uint16_t address, uint8_t value);
    void update_color_palette(GameBoySystem::Colour *palette, uint8_t value);

    void save_cartridge_state();
};