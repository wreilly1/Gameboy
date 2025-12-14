# Game Boy Emulator

A cycle-accurate Game Boy emulator written in C++17 with comprehensive debugging capabilities.

## Features

- **Complete CPU Emulation**: Full Sharp LR35902 instruction set with cycle-accurate timing
- **Graphics Rendering**: Pixel-perfect PPU implementation with tile-based rendering
- **Memory Banking**: Support for multiple MBC types (MBC0, MBC1, MBC2, MBC3, MBC5)
- **Built-in Boot ROM**: Authentic Game Boy startup sequence with Nintendo logo
- **Debug System**: Real-time hardware state visualization and debugging tools
- **Save Game Support**: Battery-backed RAM save/load functionality
- **Cross-platform**: SDL2-based graphics for Windows, macOS, and Linux

## Requirements

- C++17 compatible compiler (GCC 7+ or Clang 5+)
- SDL2 development libraries
- SDL2_ttf development libraries
- Make build system

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev build-essential
```

**macOS:**
```bash
brew install sdl2 sdl2_ttf
```

**Arch Linux:**
```bash
sudo pacman -S sdl2 sdl2_ttf gcc make
```

## Building

1. Clone or download the repository
2. Navigate to the project directory
3. Run make to build:

```bash
make
```

This will create the `GameboyEmulator` executable.

## Usage

### Basic Usage

```bash
./GameboyEmulator --rom game.gb
```

### Command Line Options

- `--rom, -r <file>` - Game Boy ROM file (.gb) **[Required]**
- `--load-save, -l <file>` - Load save file (optional)
- `--debug, -d` - Enable debug mode with visualization tools
- `--help, -h` - Show help message

### Examples

**Run a game:**
```bash
./GameboyEmulator --rom Tetris.gb
```

**Run with debug mode:**
```bash
./GameboyEmulator --rom game.gb --debug
```

**Load a save file:**
```bash
./GameboyEmulator --rom game.gb --load-save save.sav
```

## Controls

### Game Controls
- **Arrow Keys** - D-Pad
- **Z** - A Button
- **X** - B Button
- **Enter** - Start
- **Space** - Select

### Debug Controls (Debug Mode Only)
- **F1** - Toggle register view
- **F2** - Toggle tile map viewer
- **F3** - Toggle sprite viewer
- **F4** - Toggle memory inspector
- **P** - Pause/Resume emulation
- **S** - Single step (when paused)

### System Controls
- **Escape** - Quit emulator

## File Structure

```
├── src/                    # Source files
│   ├── gameboy.cpp        # Main emulator class
│   ├── cpu.cpp            # CPU implementation
│   ├── ppu.cpp            # Graphics processing unit
│   ├── memory.cpp         # Memory management
│   ├── cartridge.cpp      # Cartridge and MBC handling
│   ├── renderer.cpp       # SDL2 rendering system
│   ├── joypad.cpp         # Input handling
│   └── instructions.cpp   # CPU instruction implementation
├── include/               # Header files
├── main.cpp              # Application entry point
├── Makefile              # Build configuration
└── README.md             # This file
```

## Supported Game Types

- **ROM Only** (MBC0) - Basic cartridges without banking
- **MBC1** - Most common banking controller (up to 2MB ROM, 32KB RAM)
- **MBC2** - Banking with built-in RAM (256x4 bits)
- **MBC3** - Advanced banking with RTC support (up to 2MB ROM, 32KB RAM)
- **MBC5** - Large ROM support (up to 8MB ROM, 128KB RAM)

## Save Files

The emulator automatically creates save files in the `saves/` directory when games write to battery-backed RAM. Save files are named with the game title and timestamp.

To load a specific save file, use the `--load-save` option.

## Debug Mode

Debug mode provides real-time visualization of the Game Boy's internal state:

- **Register Viewer** - CPU registers and flags
- **Tile Map Viewer** - Background and window tile maps
- **Sprite Viewer** - Object Attribute Memory (OAM)
- **Memory Inspector** - Raw memory content browser

Debug components can be toggled individually using function keys (F1-F4).

## Troubleshooting

### Common Issues

**"ROM file does not exist"**
- Verify the ROM file path is correct
- Ensure the file has read permissions

**"Unsupported MBC type"**
- The game uses an unsupported Memory Bank Controller
- Check if the game is a valid Game Boy ROM

**Black screen on startup**
- Try running with `--debug` to see system state
- Verify the ROM file is not corrupted

**Poor performance**
- Disable debug mode if enabled
- Ensure SDL2 is using hardware acceleration

### Performance Tips

- Run without debug mode for best performance
- Close other resource-intensive applications
- Use Release build configuration for optimal speed

## Technical Details

- **CPU**: Sharp LR35902 (8-bit, Z80-like)
- **Memory**: 64KB address space with banking
- **Graphics**: 160x144 pixel LCD, 4-shade monochrome
- **Audio**: Not implemented in current version
- **Timing**: Cycle-accurate at 4.194304 MHz

## Compatibility

The emulator has been tested with various Game Boy games and achieves high compatibility for supported MBC types. Some edge cases or advanced features may not be fully implemented.

## Building from Source

### Prerequisites
- C++17 compiler
- SDL2 and SDL2_ttf development libraries
- Make

### Compile
```bash
make clean  # Remove previous build artifacts
make        # Build the emulator
```

### Clean
```bash
make clean  # Remove build files
```

## License

This project is for educational purposes. Game Boy and related trademarks are property of Nintendo Co., Ltd.

## Contributing

This is an educational project. For improvements or bug reports, please ensure changes maintain code quality and emulation accuracy.

## Acknowledgments

- Nintendo for the original Game Boy hardware
- Game Boy development community for documentation and resources
- SDL2 team for cross-platform graphics support