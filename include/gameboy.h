// #include <cstring>
#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <set>
#include <chrono>
#include <thread>
#include <getopt.h>
#include <memory>

#include "status.h"

#include "cpu.h"
#include "ppu.h"
#include "renderer.h"
#include "cartridge.h"
#include "joypad.h"
#include "memory.h"

// GB configuration structure
struct GameboyConfig {
    bool debug = false;
    std::string save_file = "";
};

// GB constants
struct GameboyConstants {
    static const int DEFAULT_INSTRUCTION_CYCLES = 0;
    static const bool DEFAULT_RUNNING_STATE = true;
    static const bool DEFAULT_STEP_STATE = false;
};

class Gameboy {
   private:
    // Factory methods for component creation
    void create_core_components(Cartridge* cartridge);
    void create_optional_components(const GameboyConfig& config);
    
   public:
    GameState::Status status;

    Registers registers;
    std::unique_ptr<Memory> mmu;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<PPU> ppu;
    std::unique_ptr<Interrupts> interrupts;
    std::unique_ptr<Timer> timer;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Joypad> joypad;
    Cartridge *cartridge;

    void init(std::string rom, std::string save_file = "", bool debug = false);
    void init(Cartridge *cartridge, const GameboyConfig& config = {});
    bool run_step();
    void run();
    void run_until_next_frame();
};