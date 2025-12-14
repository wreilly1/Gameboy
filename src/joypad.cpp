#include "joypad.h"
#include "cpu.h"

// Static gameboy key mapping table
const GameboyKeyMapping Joypad::GAMEBOY_KEY_MAPPINGS[] = {
    {SDLK_RIGHT, JOYPAD_RIGHT},
    {SDLK_LEFT, JOYPAD_LEFT},
    {SDLK_UP, JOYPAD_UP},
    {SDLK_DOWN, JOYPAD_DOWN},
    {SDLK_z, JOYPAD_A},
    {SDLK_x, JOYPAD_B},
    {SDLK_SPACE, JOYPAD_START},
    {SDLK_RETURN, JOYPAD_SELECT}
};

// Static system key mapping table
const SystemKeyMapping Joypad::SYSTEM_KEY_MAPPINGS[] = {
    {SDLK_c, &Joypad::toggle_color_mode, false},
    {SDLK_p, &Joypad::toggle_pause, true},
    {SDLK_s, &Joypad::save_state, false},
    {SDLK_n, &Joypad::step_debug, true},
    {SDLK_m, &Joypad::toggle_sound, false},
    {SDLK_ESCAPE, &Joypad::quit_game, false}
};

Joypad::Joypad(GameState::Status* status, Interrupts* interrupts, Memory* mmu) {
    this->interrupts = interrupts;
    this->mmu = mmu;
    this->status = status;
    this->joypad_state = JoypadConstants::DEFAULT_CYCLES;
}

void Joypad::key_press(ButtonFlags key) { this->joypad_state = mmu->controller_input.button_data & ~(JoypadConstants::BUTTON_MASK & key); }

void Joypad::key_release(ButtonFlags key) { this->joypad_state = mmu->controller_input.button_data | (JoypadConstants::BUTTON_MASK & key); }
void Joypad::update_joypad_memory() {
    if (this->joypad_state) {
        mmu->controller_input.button_data = this->joypad_state;
        this->joypad_state = 0;
        interrupts->set_interrupt_flag(INTERRUPT_JOYPAD);
    }
}

void Joypad::check(int last_instr_cycles) {
    joypad_cycles += last_instr_cycles;
    if (!status->flags.isPaused) {
        if (joypad_cycles < JoypadConstants::CYCLE_THRESHOLD)
            return;
        joypad_cycles -= JoypadConstants::CYCLE_THRESHOLD;
    }

    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_KEYUP: {
            ButtonFlags button = find_gameboy_button(event.key.keysym.sym, false);
            if (button != ButtonFlags(0)) {
                key_release(button);
            } else {
                handle_system_key(event.key.keysym.sym, false);
            }
            break;
        }
        case SDL_KEYDOWN: {
            ButtonFlags button = find_gameboy_button(event.key.keysym.sym, true);
            if (button != ButtonFlags(0)) {
                key_press(button);
            } else {
                handle_system_key(event.key.keysym.sym, true);
            }
            break;
        }
        case SDL_QUIT:
            this->status->flags.isRunning = false;
            exit(0);
            break;
    }
    update_joypad_memory();
}

// Helper method implementations
ButtonFlags Joypad::find_gameboy_button(SDL_Keycode key, bool key_down) const {
    constexpr size_t table_size = sizeof(GAMEBOY_KEY_MAPPINGS) / sizeof(GAMEBOY_KEY_MAPPINGS[0]);
    for (size_t i = 0; i < table_size; ++i) {
        if (GAMEBOY_KEY_MAPPINGS[i].key == key) {
            return GAMEBOY_KEY_MAPPINGS[i].button;
        }
    }
    return ButtonFlags(0);
}

bool Joypad::handle_system_key(SDL_Keycode key, bool key_down) {
    if (!key_down && key != SDLK_ESCAPE) return false; // Only handle key releases for most system keys
    
    constexpr size_t table_size = sizeof(SYSTEM_KEY_MAPPINGS) / sizeof(SYSTEM_KEY_MAPPINGS[0]);
    for (size_t i = 0; i < table_size; ++i) {
        if (SYSTEM_KEY_MAPPINGS[i].key == key) {
            if (!SYSTEM_KEY_MAPPINGS[i].debug_only || status->flags.debug) {
                (this->*SYSTEM_KEY_MAPPINGS[i].action)();
                return true;
            }
        }
    }
    return false;
}

// System action method implementations
void Joypad::toggle_color_mode() {
    int currentMode = static_cast<int>(status->display.colorMode);
    status->display.colorMode = static_cast<GameState::ColorMode>((currentMode + 1) % JoypadConstants::COLOR_MODES);
}

void Joypad::toggle_pause() {
    if (status->flags.debug)
        status->flags.isPaused = !status->flags.isPaused;
}

void Joypad::save_state() {
    mmu->save_cartridge_state();
}

void Joypad::step_debug() {
    if (status->flags.debug)
        status->flags.doStep = true;
}

void Joypad::toggle_sound() {
    status->flags.soundEnabled = !status->flags.soundEnabled;
}

void Joypad::quit_game() {
    status->flags.isRunning = false;
}
