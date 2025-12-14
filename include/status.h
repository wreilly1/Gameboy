#pragma once

namespace GameState {
    enum class ColorMode { NORMAL = 0, RETRO = 1, GRAY = 2 };
    
    struct Flags {
        bool debug = false;
        bool isRunning = false;
        bool isPaused = false;
        bool doStep = false;
        bool soundEnabled = true;
    };
    
    struct Display {
        ColorMode colorMode = ColorMode::NORMAL;
    };
    
    struct Status {
        Flags flags;
        Display display;
    };
}