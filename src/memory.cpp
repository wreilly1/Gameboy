#include "memory.h"

Memory::Memory(Cartridge *cartridge) { this->game_cartridge = cartridge; }

void Memory::load_default_system_rom() { 
    std::copy(gameboy_system_rom, gameboy_system_rom + sizeof(gameboy_system_rom), system_memory); 
}

void Memory::save_cartridge_state() { game_cartridge->write_save_state(); }

void Memory::update_tile_data(uint16_t laddress, uint8_t value) {
    uint16_t address = laddress & 0xFFFE;

    uint16_t tile = (address >> 4) & 511;
    uint16_t y = (address >> 1) & 7;

    uint8_t bitIndex;
    for (uint8_t x = 0; x < 8; x++) {
        bitIndex = 1 << (7 - x);

        graphics_tiles[tile].pixel_matrix[y][x] = ((system_memory[address] & bitIndex) ? 1 : 0) + ((system_memory[address + 1] & bitIndex) ? 2 : 0);
    }
}

void Memory::update_sprite_data(uint16_t laddress, uint8_t value) {
    uint16_t address = laddress - 0xFE00;
    GameBoySystem::Graphics::SpriteAttributes *sprite = &sprite_data[address >> 2];
    sprite->is_active = false;
    switch (address & 3) {
        case 0:
            sprite->y_position = value - 16;
            break;
        case 1:
            sprite->x_position = value - 8;
            break;
        case 2:
            sprite->tile_number = value;
            break;
        case 3:
            sprite->flags.raw_value = value;
            sprite->color_palette = (sprite->flags.dmg_palette) ? sprite_colors_1 : sprite_colors_0;
            sprite->is_active = true;
            break;
    }
}

void Memory::update_color_palette(GameBoySystem::Colour *palette, uint8_t value) {
    palette[0] = get_default_palette()[value & 0x3];
    palette[1] = get_default_palette()[(value >> 2) & 0x3];
    palette[2] = get_default_palette()[(value >> 4) & 0x3];
    palette[3] = get_default_palette()[(value >> 6) & 0x3];
}

uint8_t Memory::read_memory_byte(uint16_t address) {
    switch (address) {
        case 0xff00: {
            switch (system_memory[0xff00] & 0x30) {  // Mask `00110000` to check which SELECT
                case 0x10:
                    return (uint8_t)(controller_input.button_data & 0x0F) | 0x10;
                case 0x20:
                    return ((uint8_t)(controller_input.button_data >> 4) & 0x0F) | 0x20;
                default:
                    return 0xFF;
            }
        }
        case 0xff04:
            return timer_registers.div_counter;
        case 0xff05:
            return timer_registers.timer_counter;
        case 0xff06:
            return timer_registers.timer_modulo;
        case 0xff07:
            return timer_registers.timer_control;
        case 0xff0f:
            return system_memory[0xFF0F];
        default:
            break;
    }

    // Handle address ranges that can't use switch
    if (address < 0x100 && !memory_flags.rom_disabled)
        return system_memory[address];

    // Switchable ROM banks
    if (address < 0x8000)
        return game_cartridge->mbc_read(address);

    // Switchable RAM banks
    if (address >= 0xA000 && address <= 0xBFFF)
        return game_cartridge->mbc_read(address);

    return system_memory[address];
}

void Memory::write_memory_byte(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFF40:
            system_memory[address] = value;
            if (!(value & (1 << 7))) {
                system_memory[0xFF44] = 0x00;
                system_memory[0xFF41] &= 0x7C;
            }
            break;
        case 0xFF46:
            // Copy Sprites from ROM to RAM (OAM)
            for (uint16_t i = 0; i < 160; i++) 
                write_memory_byte(0xFE00 + i, read_memory_byte((value << 8) + i));
            break;
        case 0xff50:
            memory_flags.rom_disabled = true;
            break;
        case 0xff04:
            timer_registers.div_counter = 0;
            break;
        case 0xff05:
            timer_registers.timer_counter = value;
            break;
        case 0xff06:
            timer_registers.timer_modulo = value;
            break;
        case 0xff07:
            timer_registers.timer_control = value;
            break;
        case 0xff47:
            update_color_palette(background_colors, value);
            break;
        case 0xff48:
            update_color_palette(sprite_colors_0, value);
            break;
        case 0xff49:
            update_color_palette(sprite_colors_1, value);
            break;
        default:
            break;
    }

    // Handle special address ranges
    if (address >= 0xFEA0 && address <= 0xFEFF)  // Writing in unused area
        return;

    // Handle address ranges that can't use switch
    if (address < 0x8000)
        game_cartridge->mbc_write(address, value);
    else if (address >= 0xA000 && address < 0xC000)
        game_cartridge->mbc_write(address, value);
    else
        system_memory[address] = value;

    // Update graphics data
    if (address >= 0x8000 && address < 0x9800)
        update_tile_data(address, value);

    if (address >= 0xFE00 && address <= 0xFE9F)
        update_sprite_data(address, value);
}

uint16_t Memory::read_memory_short(uint16_t address) { 
    return read_memory_byte(address) | (read_memory_byte(address + 1) << 8); 
}

void Memory::write_memory_short(uint16_t address, uint16_t value) {
    write_memory_byte(address, (uint8_t)(value & 0x00ff));
    write_memory_byte(address + 1, (uint8_t)((value & 0xff00) >> 8));
}

void Memory::write_stack_short(uint16_t *sp, uint16_t value) {
    *sp -= 2;
    write_memory_short(*sp, value);
}

uint16_t Memory::read_stack_short(uint16_t *sp) {
    uint16_t value = read_memory_short(*sp);
    *sp += 2;

    return value;
}