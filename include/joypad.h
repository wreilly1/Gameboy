#pragma once
#include <SDL2/SDL.h>

#include "memory.h"
#include <functional>

class Interrupts;

#include "status.h"

// Joypad constants
struct JoypadConstants {
    static const int CYCLE_THRESHOLD = 65536;
    static const int DEFAULT_CYCLES = 0;
    static const uint8_t BUTTON_MASK = 0xFF;
    static const int COLOR_MODES = 3;
};

enum ButtonFlags {
    JOYPAD_A = (1 << 0),
    JOYPAD_B = (1 << 1),
    JOYPAD_SELECT = (1 << 2),
    JOYPAD_START = (1 << 3),

    JOYPAD_RIGHT = (1 << 4),
    JOYPAD_LEFT = (1 << 5),
    JOYPAD_UP = (1 << 6),
    JOYPAD_DOWN = (1 << 7),
};

// Key mapping structures
struct GameboyKeyMapping {
    SDL_Keycode key;
    ButtonFlags button;
};

class Joypad; // Forward declaration

struct SystemKeyMapping {
    SDL_Keycode key;
    void (Joypad::*action)();
    bool debug_only;
};

class Joypad {
    GameState::Status* status;
    Memory* mmu;
    Interrupts* interrupts;

    uint8_t joypad_state;
    int joypad_cycles = JoypadConstants::DEFAULT_CYCLES;

    // Static lookup tables
    static const GameboyKeyMapping GAMEBOY_KEY_MAPPINGS[];
    static const SystemKeyMapping SYSTEM_KEY_MAPPINGS[];
    
    // Helper methods for lookup tables
    ButtonFlags find_gameboy_button(SDL_Keycode key, bool key_down) const;
    bool handle_system_key(SDL_Keycode key, bool key_down);
    
    void update_joypad_memory();
    
    // System action methods
    void toggle_color_mode();
    void toggle_pause();
    void save_state();
    void step_debug();
    void toggle_sound();
    void quit_game();

   public:
    Joypad(GameState::Status* status, Interrupts* interrupts, Memory* mmu);

    void key_press(ButtonFlags key);
    void key_release(ButtonFlags key);
    void check(int last_instr_cycles);
};