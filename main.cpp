#include "gameboy.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <map>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

class EmulatorConfig {
public:
    std::string rom_path;
    std::string save_file_path;
    bool debug_mode = false;
    bool show_help = false;
    
    void validate() const {
        if (rom_path.empty() && !show_help) {
            throw std::runtime_error("ROM file path is required");
        }
        
        if (!rom_path.empty() && !fs::exists(rom_path)) {
            throw std::runtime_error("ROM file does not exist: " + rom_path);
        }
        
        if (!save_file_path.empty() && !fs::exists(save_file_path)) {
            throw std::runtime_error("Save file does not exist: " + save_file_path);
        }
    }
};

class ArgumentParser {
private:
    std::map<std::string, std::string> arguments;
    std::vector<std::string> flags;
    
public:
    ArgumentParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg.length() >= 2 && arg.substr(0, 2) == "--") {
                if (i + 1 < argc && std::string(argv[i + 1])[0] != '-') {
                    arguments[arg.substr(2)] = argv[++i];
                } else {
                    flags.push_back(arg.substr(2));
                }
            } else if (arg.length() == 2 && arg[0] == '-') {
                if (i + 1 < argc && std::string(argv[i + 1])[0] != '-') {
                    arguments[std::string(1, arg[1])] = argv[++i];
                } else {
                    flags.push_back(std::string(1, arg[1]));
                }
            } else {
                throw std::runtime_error("Unknown argument: " + arg);
            }
        }
    }
    
    std::string get_argument(const std::string& key, const std::string& default_value = "") const {
        auto it = arguments.find(key);
        return (it != arguments.end()) ? it->second : default_value;
    }
    
    bool has_flag(const std::string& flag) const {
        return std::find(flags.begin(), flags.end(), flag) != flags.end();
    }
    
    EmulatorConfig parse_config() const {
        EmulatorConfig config;
        
        config.show_help = has_flag("help") || has_flag("h");
        config.debug_mode = has_flag("debug") || has_flag("d");
        
        config.rom_path = get_argument("rom", get_argument("r"));
        config.save_file_path = get_argument("load-save", get_argument("l"));
        
        return config;
    }
};

class EmulatorLauncher {
public:
    static void show_usage(const std::string& program_name) {
        std::cout << "Game Boy Emulator\n\n";
        std::cout << "Usage: " << program_name << " [OPTIONS]\n\n";
        std::cout << "Required:\n";
        std::cout << "  --rom, -r <file>       Game Boy ROM file (.gb)\n\n";
        std::cout << "Optional:\n";
        std::cout << "  --load-save, -l <file> Save file to load\n";
        std::cout << "  --debug, -d            Enable debug mode\n";
        std::cout << "  --help, -h             Show this help message\n\n";
        std::cout << "Examples:\n";
        std::cout << "  " << program_name << " --rom game.gb\n";
        std::cout << "  " << program_name << " -r game.gb -d\n";
        std::cout << "  " << program_name << " --rom game.gb --load-save save.sav\n";
    }
    
    static void run_emulator(const EmulatorConfig& config) {
        try {
            Gameboy emulator;
            emulator.init(config.rom_path, config.save_file_path, config.debug_mode);
            
            std::cout << "Starting Game Boy emulator...\n";
            std::cout << "ROM: " << config.rom_path << "\n";
            
            if (!config.save_file_path.empty()) {
                std::cout << "Save file: " << config.save_file_path << "\n";
            }
            
            if (config.debug_mode) {
                std::cout << "Debug mode: enabled\n";
            }
            
            std::cout << "\nPress ESC to quit\n\n";
            
            emulator.run();
            
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to initialize emulator: " + std::string(e.what()));
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        ArgumentParser parser(argc, argv);
        EmulatorConfig config = parser.parse_config();
        
        if (config.show_help) {
            EmulatorLauncher::show_usage(argv[0]);
            return 0;
        }
        
        config.validate();
        EmulatorLauncher::run_emulator(config);
        
        return 0;
        
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n\n";
        EmulatorLauncher::show_usage(argv[0]);
        return 1;
    }
}