#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <sstream>
#include <memory>
#include <functional>
#include <cstdint>

// MBC address range constants
struct MBCAddresses {
    static const uint16_t ROM_BANK_0_END = 0x4000;
    static const uint16_t ROM_BANK_1_END = 0x8000;
    static const uint16_t RAM_START = 0xA000;
    static const uint16_t RAM_END = 0xC000;
    static const uint16_t ROM_ENABLE_END = 0x2000;
    static const uint16_t ROM_BANK_SELECT_END = 0x4000;
    static const uint16_t RAM_BANK_SELECT_END = 0x6000;
};

// MBC general constants
struct MBCConstants {
    static const uint16_t ROM_BANK_SIZE = 0x4000;
    static const uint16_t RAM_BANK_SIZE = 0x2000;
    static const uint8_t RAM_ENABLE_VALUE = 0x0A;
    static const uint8_t RAM_ENABLE_MASK = 0x0F;
    static const uint8_t DEFAULT_ROM_BANK = 1;
    static const uint8_t DEFAULT_RAM_BANK = 0;
};

// MBC1 specific constants
struct MBC1Constants {
    static const uint8_t ROM_BANK_MASK = 0x1F;
    static const uint8_t RAM_BANK_MASK = 0x3;
    static const uint8_t MODE_MASK = 0x1;
    static const uint8_t BANK_SHIFT = 5;
};

// MBC2 specific constants
struct MBC2Constants {
    static const uint16_t ADDRESS_BIT = 0x0100;
};

// MBC3 specific constants
struct MBC3Constants {
    static const uint8_t ROM_BANK_MASK = 0x7F;
    static const uint8_t RAM_BANK_MASK = 0x0F;
    static const uint8_t MAX_RAM_BANK = 0x03;
};

// MBC5 specific constants
struct MBC5Constants {
    static const uint16_t ROM_BANK_LOW_END = 0x3000;
    static const uint8_t ROM_BANK_HIGH_MASK = 0x01;
    static const uint8_t ROM_BANK_HIGH_SHIFT = 8;
    static const uint16_t ROM_BANK_HIGH_MASK_CLEAR = 0x100;
    static const uint16_t ROM_BANK_LOW_MASK_CLEAR = 0xFF;
};

class MBC {
   protected:
    // Helper methods for address range checking
    bool is_rom_bank_0_range(uint16_t address) const { return address < MBCAddresses::ROM_BANK_0_END; }
    bool is_rom_bank_1_range(uint16_t address) const { return address >= MBCAddresses::ROM_BANK_0_END && address < MBCAddresses::ROM_BANK_1_END; }
    bool is_ram_range(uint16_t address) const { return address >= MBCAddresses::RAM_START && address < MBCAddresses::RAM_END; }
    bool is_ram_enable_range(uint16_t address) const { return address < MBCAddresses::ROM_ENABLE_END; }
    bool is_rom_bank_select_range(uint16_t address) const { return address >= MBCAddresses::ROM_ENABLE_END && address < MBCAddresses::ROM_BANK_SELECT_END; }
    bool is_ram_bank_select_range(uint16_t address) const { return address >= MBCAddresses::ROM_BANK_SELECT_END && address < MBCAddresses::ROM_BANK_1_END; }
    
   public:
    uint8_t *rom;
    uint8_t *ram;
    int rom_banks_count = 1;
    int ram_banks_count = 1;

    MBC(uint8_t *rom);
    MBC(uint8_t *rom, uint8_t *ram, int rom_banks_count, int ram_banks_count);
    virtual uint8_t read_byte(uint16_t address) = 0;
    virtual void write_byte(uint16_t address, uint8_t value) = 0;
};

class MBC0 : public MBC {
   public:
    using MBC::MBC;
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value){};
};

class MBC1 : public MBC {
   public:
    bool ram_enabled = false;
    bool mode = false;
    uint8_t rom_bank = MBCConstants::DEFAULT_ROM_BANK;
    uint8_t ram_bank = MBCConstants::DEFAULT_RAM_BANK;

    using MBC::MBC;
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
};

class MBC2 : public MBC1 {
   public:
    using MBC1::MBC1;
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
};

class MBC3 : public MBC1 {
   public:
    using MBC1::MBC1;
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
};

class MBC5 : public MBC1 {
   public:
    using MBC1::MBC1;
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
};

// ROM header constants structure
struct ROMHeader {
    static const uint16_t TITLE_START = 0x134;
    static const uint16_t TITLE_END = 0x143;
    static const uint16_t CGB_FLAG = 0x143;
    static const uint16_t MBC_TYPE = 0x147;
    static const uint16_t RAM_SIZE = 0x149;
    static const uint8_t CGB_COMPATIBLE = 0x80;
    static const uint8_t CGB_ONLY = 0xC0;
};

// Cartridge constants
struct CartridgeConstants {
    static const int ROM_BANK_SIZE = 0x4000;  // 16KB
    static const int RAM_BANK_SIZE = 0x2000;  // 8KB
    static const int KB_16 = 16 * 1024;
    static const int MAX_RAM_BANKS = 0x7F;
};

// MBC descriptor structure
struct MBCDescriptor {
    uint8_t type_code;
    const char* name;
    std::function<std::unique_ptr<MBC>(uint8_t*, uint8_t*, int, int)> factory;
};

// RAM size descriptor structure
struct RAMSizeDescriptor {
    uint8_t size_code;
    int bank_count;
    const char* description;
};

class Cartridge {
   private:
    // Static lookup tables
    static const MBCDescriptor MBC_TABLE[];
    static const RAMSizeDescriptor RAM_SIZE_TABLE[];
    
    // Helper methods for lookup tables
    const MBCDescriptor* find_mbc_descriptor(uint8_t type) const;
    const RAMSizeDescriptor* find_ram_size_descriptor(uint8_t type) const;

   public:
    std::string rom_title;
    bool cgb_game = false;
    uint8_t mbc_type = 0x0;

    std::unique_ptr<MBC> mbc;
    std::unique_ptr<uint8_t[]> memory;
    std::unique_ptr<uint8_t[]> ram;

    int rom_banks_count;
    int ram_banks_count;

    Cartridge(std::string rom, std::string save_file = "");

    void load_game_rom(std::string location);
    void detect_mbc_type(uint8_t type);
    int get_ram_banks_count(uint8_t type);

    uint8_t mbc_read(uint16_t address) { return mbc->read_byte(address); };
    void mbc_write(uint16_t address, uint8_t value) { mbc->write_byte(address, value); };

    void load_save_state(std::string save_file);
    void write_save_state();

    void printInfo();
};
