#include "cartridge.h"

// Static MBC lookup table definition
const MBCDescriptor Cartridge::MBC_TABLE[] = {
    {0x00, "ROM ONLY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC0>(memory); }},
    {0x08, "ROM+RAM", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC0>(memory); }},
    {0x09, "ROM+RAM+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC0>(memory); }},
    
    {0x01, "MBC1", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC1>(memory, ram, rom_banks, ram_banks); }},
    {0x02, "MBC1+RAM", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC1>(memory, ram, rom_banks, ram_banks); }},
    {0x03, "MBC1+RAM+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC1>(memory, ram, rom_banks, ram_banks); }},
    
    {0x05, "MBC2", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC2>(memory, ram, rom_banks, ram_banks); }},
    {0x06, "MBC2+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC2>(memory, ram, rom_banks, ram_banks); }},
    
    {0x0F, "MBC3+TIMER+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC3>(memory, ram, rom_banks, ram_banks); }},
    {0x10, "MBC3+TIMER+RAM+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC3>(memory, ram, rom_banks, ram_banks); }},
    {0x11, "MBC3", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC3>(memory, ram, rom_banks, ram_banks); }},
    {0x12, "MBC3+RAM", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC3>(memory, ram, rom_banks, ram_banks); }},
    {0x13, "MBC3+RAM+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC3>(memory, ram, rom_banks, ram_banks); }},
    
    {0x19, "MBC5", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC5>(memory, ram, rom_banks, ram_banks); }},
    {0x1A, "MBC5+RAM", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC5>(memory, ram, rom_banks, ram_banks); }},
    {0x1B, "MBC5+RAM+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC5>(memory, ram, rom_banks, ram_banks); }},
    {0x1C, "MBC5+RUMBLE", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC5>(memory, ram, rom_banks, ram_banks); }},
    {0x1D, "MBC5+RUMBLE+RAM", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC5>(memory, ram, rom_banks, ram_banks); }},
    {0x1E, "MBC5+RUMBLE+RAM+BATTERY", [](uint8_t* memory, uint8_t* ram, int rom_banks, int ram_banks) { return std::make_unique<MBC5>(memory, ram, rom_banks, ram_banks); }}
};

// Static RAM size lookup table definition
const RAMSizeDescriptor Cartridge::RAM_SIZE_TABLE[] = {
    {0x00, 0, "No RAM"},
    {0x01, 0, "Unused"},
    {0x02, 1, "8 KB (1 bank)"},
    {0x03, 4, "32 KB (4 banks)"},
    {0x04, 16, "128 KB (16 banks)"},
    {0x05, 8, "64 KB (8 banks)"}
};

Cartridge::Cartridge(std::string rom, std::string save_file) {
    load_game_rom(rom);
    load_save_state(save_file);
}

void Cartridge::load_game_rom(std::string location) {
    std::ifstream GAME_ROM(location, std::ios::binary);
    GAME_ROM.seekg(0, std::ios::end);
    long size = GAME_ROM.tellg();
    if (size % CartridgeConstants::KB_16 != 0) {
        std::cout << "Size must be a multiple of 16 KB" << std::endl;
        return;
    }

    memory = std::make_unique<uint8_t[]>(size);

    GAME_ROM.seekg(std::ios::beg);
    GAME_ROM.read((char *)memory.get(), size);

    rom_banks_count = size / CartridgeConstants::ROM_BANK_SIZE;
    ram_banks_count = get_ram_banks_count(memory[ROMHeader::RAM_SIZE]);

    ram = std::make_unique<uint8_t[]>(ram_banks_count * CartridgeConstants::RAM_BANK_SIZE);

    rom_title = std::string(memory.get() + ROMHeader::TITLE_START, memory.get() + ROMHeader::TITLE_END);
    cgb_game = memory[ROMHeader::CGB_FLAG] == ROMHeader::CGB_COMPATIBLE || memory[ROMHeader::CGB_FLAG] == ROMHeader::CGB_ONLY;
    mbc_type = memory[ROMHeader::MBC_TYPE];

    detect_mbc_type(memory[ROMHeader::MBC_TYPE]);

    this->printInfo();
}

void Cartridge::detect_mbc_type(uint8_t type) {
    const MBCDescriptor* descriptor = find_mbc_descriptor(type);
    if (descriptor != nullptr) {
        mbc = descriptor->factory(memory.get(), ram.get(), rom_banks_count, ram_banks_count);
    } else {
        std::cout << "Unsupported MBC type: 0x" << std::hex << +type << std::endl;
        exit(1);
    }
}

int Cartridge::get_ram_banks_count(uint8_t type) {
    const RAMSizeDescriptor* descriptor = find_ram_size_descriptor(type);
    if (descriptor != nullptr) {
        return descriptor->bank_count;
    } else {
        std::cout << "Incorrect RAM type: 0x" << std::hex << +type << std::endl;
        exit(1);
    }
}

void Cartridge::load_save_state(std::string save_file) {
    if (save_file.empty())
        return;

    std::ifstream SAVE(save_file, std::ios::binary);
    SAVE.seekg(0, std::ios::end);

    long size = SAVE.tellg();
    if (size != (CartridgeConstants::MAX_RAM_BANKS * CartridgeConstants::RAM_BANK_SIZE + sizeof(rom_title))) {
        std::cout << "Save file possibly corrupted. Save not loaded." << std::endl;
        return;
    }

    char save_title[16];
    SAVE.seekg(0, std::ios::beg);
    SAVE.read((char *)save_title, sizeof(save_title));
    std::cout << "Save file " << save_title << std::endl;
    if (rom_title != save_title) {
        std::cout << "This save file is not for this rom. Save not loaded." << std::endl;
        return;
    }
    SAVE.seekg(sizeof(save_title));
    SAVE.read((char *)ram.get(), CartridgeConstants::MAX_RAM_BANKS * CartridgeConstants::RAM_BANK_SIZE);

    std::cout << "Save file loaded successfully" << std::endl;
}

void Cartridge::write_save_state() {
    std::filesystem::create_directory("saves");

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << "saves/" << rom_title << "_" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ".save";
    std::string filename = oss.str();

    std::ofstream out(filename, std::ios_base::binary);

    size_t title_length = rom_title.size();
    out.write(reinterpret_cast<const char *>(&title_length), sizeof(title_length));
    out.write(rom_title.c_str(), title_length);

    // Write the RAM data
    out.write(reinterpret_cast<const char *>(ram.get()), sizeof(uint8_t) * (CartridgeConstants::MAX_RAM_BANKS * CartridgeConstants::RAM_BANK_SIZE));

    std::cout << "Saved state to: " << filename << std::endl;
}

void Cartridge::printInfo() {
    std::cout << "Rom Title: " << rom_title << std::endl;
    std::cout << "CGB Game: " << (cgb_game ? "Yes" : "No") << std::endl;
    
    const MBCDescriptor* mbc_desc = find_mbc_descriptor(mbc_type);
    std::cout << "MBC: " << +mbc_type << " (" << (mbc_desc ? mbc_desc->name : "Unknown") << ")" << std::endl;
    
    std::cout << "ROM Banks: " << rom_banks_count << std::endl;
    
    const RAMSizeDescriptor* ram_desc = find_ram_size_descriptor(memory[ROMHeader::RAM_SIZE]);
    std::cout << "RAM Banks: " << ram_banks_count << " (" << (ram_desc ? ram_desc->description : "Unknown") << ")" << std::endl;
}

// Helper method implementations
const MBCDescriptor* Cartridge::find_mbc_descriptor(uint8_t type) const {
    constexpr size_t table_size = sizeof(MBC_TABLE) / sizeof(MBC_TABLE[0]);
    for (size_t i = 0; i < table_size; ++i) {
        if (MBC_TABLE[i].type_code == type) {
            return &MBC_TABLE[i];
        }
    }
    return nullptr;
}

const RAMSizeDescriptor* Cartridge::find_ram_size_descriptor(uint8_t type) const {
    constexpr size_t table_size = sizeof(RAM_SIZE_TABLE) / sizeof(RAM_SIZE_TABLE[0]);
    for (size_t i = 0; i < table_size; ++i) {
        if (RAM_SIZE_TABLE[i].size_code == type) {
            return &RAM_SIZE_TABLE[i];
        }
    }
    return nullptr;
}

// MBC implementations
MBC::MBC(uint8_t *rom) { this->rom = rom; };
MBC::MBC(uint8_t *rom, uint8_t *ram, int rom_banks_count, int ram_banks_count) {
    this->rom = rom;
    this->ram = ram;
    this->rom_banks_count = rom_banks_count;
    this->ram_banks_count = ram_banks_count;
};

uint8_t MBC0::read_byte(uint16_t address) {
    if (address < MBCAddresses::ROM_BANK_1_END)
        return rom[address];

    return 0;
}

uint8_t MBC1::read_byte(uint16_t address) {
    if (is_rom_bank_0_range(address)) {
        int bank = mode * (ram_bank << MBC1Constants::BANK_SHIFT) % rom_banks_count;
        return rom[bank * MBCConstants::ROM_BANK_SIZE + address];
    } else if (is_rom_bank_1_range(address)) {
        int bank = ((ram_bank << MBC1Constants::BANK_SHIFT) | rom_bank) % rom_banks_count;
        return rom[bank * MBCConstants::ROM_BANK_SIZE + address - MBCAddresses::ROM_BANK_0_END];
    } else if (is_ram_range(address)) {
        if (ram_enabled) {
            int bank = mode * ram_bank % ram_banks_count;
            return ram[bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START];
        }
    }
    return 0xFF;
}

void MBC1::write_byte(uint16_t address, uint8_t value) {
    if (is_ram_enable_range(address)) {
        ram_enabled = (value & MBCConstants::RAM_ENABLE_MASK) == MBCConstants::RAM_ENABLE_VALUE;
    } else if (is_rom_bank_select_range(address)) {
        value &= MBC1Constants::ROM_BANK_MASK;
        if (value == 0)
            value = MBCConstants::DEFAULT_ROM_BANK;
        rom_bank = value;
    } else if (is_ram_bank_select_range(address)) {
        ram_bank = value & MBC1Constants::RAM_BANK_MASK;
    } else if (address >= MBCAddresses::RAM_BANK_SELECT_END && address < MBCAddresses::ROM_BANK_1_END) {
        mode = value & MBC1Constants::MODE_MASK;
    } else if (is_ram_range(address)) {
        if (ram_enabled) {
            int bank = (ram_bank * mode) % ram_banks_count;
            ram[bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START] = value;
        }
    }
}

uint8_t MBC2::read_byte(uint16_t address) {
    if (is_rom_bank_0_range(address))
        return rom[address];
    else if (is_rom_bank_1_range(address))
        return rom[rom_bank * MBCConstants::ROM_BANK_SIZE + address - MBCAddresses::ROM_BANK_0_END];
    else if (is_ram_range(address)) {
        if (ram_enabled)
            return ram[ram_bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START];
    }

    return 0;
}

void MBC2::write_byte(uint16_t address, uint8_t value) {
    if (is_ram_enable_range(address)) {
        if ((address & MBC2Constants::ADDRESS_BIT) == 0)
            ram_enabled = value == MBCConstants::RAM_ENABLE_VALUE;
    } else if (is_rom_bank_select_range(address)) {
        if ((address & MBC2Constants::ADDRESS_BIT) != 0)
            rom_bank = value;
    } else if (is_ram_range(address)) {
        if (ram_enabled)
            ram[ram_bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START] = value;
    }
}

uint8_t MBC3::read_byte(uint16_t address) {
    if (is_rom_bank_0_range(address))
        return rom[address];
    else if (is_rom_bank_1_range(address))
        return rom[rom_bank * MBCConstants::ROM_BANK_SIZE + address - MBCAddresses::ROM_BANK_0_END];
    else if (is_ram_range(address)) {
        if (ram_enabled) {
            if (ram_bank <= MBC3Constants::MAX_RAM_BANK)
                return ram[ram_bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START];
        }
    }

    return 0;
}

void MBC3::write_byte(uint16_t address, uint8_t value) {
    if (is_ram_enable_range(address))
        ram_enabled = (value & MBCConstants::RAM_ENABLE_MASK) == MBCConstants::RAM_ENABLE_VALUE;
    else if (is_rom_bank_select_range(address)) {
        rom_bank = value & MBC3Constants::ROM_BANK_MASK;
        if (rom_bank == 0x00)
            rom_bank = MBCConstants::DEFAULT_ROM_BANK;
    } else if (is_ram_bank_select_range(address))
        ram_bank = value & MBC3Constants::RAM_BANK_MASK;
    else if (is_ram_range(address)) {
        if (ram_enabled) {
            if (ram_bank <= MBC3Constants::MAX_RAM_BANK)
                ram[ram_bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START] = value;
        }
    }
}

uint8_t MBC5::read_byte(uint16_t address) {
    if (is_rom_bank_0_range(address))
        return rom[address];
    else if (is_rom_bank_1_range(address))
        return rom[rom_bank * MBCConstants::ROM_BANK_SIZE + address - MBCAddresses::ROM_BANK_0_END];
    else if (is_ram_range(address)) {
        if (ram_enabled)
            return ram[ram_bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START];
    }

    return 0;
}

void MBC5::write_byte(uint16_t address, uint8_t value) {
    if (is_ram_enable_range(address))
        ram_enabled = (value & MBCConstants::RAM_ENABLE_MASK) == MBCConstants::RAM_ENABLE_VALUE;
    else if (address < MBC5Constants::ROM_BANK_LOW_END)
        rom_bank = (rom_bank & MBC5Constants::ROM_BANK_HIGH_MASK_CLEAR) | value;
    else if (is_rom_bank_select_range(address))
        rom_bank = (rom_bank & MBC5Constants::ROM_BANK_LOW_MASK_CLEAR) | ((value & MBC5Constants::ROM_BANK_HIGH_MASK) << MBC5Constants::ROM_BANK_HIGH_SHIFT);
    else if (is_ram_bank_select_range(address))
        ram_bank = (value & MBCConstants::RAM_ENABLE_MASK) % ram_banks_count;
    else if (is_ram_range(address)) {
        if (ram_enabled)
            ram[ram_bank * MBCConstants::RAM_BANK_SIZE + address - MBCAddresses::RAM_START] = value;
    }
}