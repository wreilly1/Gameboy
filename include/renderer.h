#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <unistd.h>

#include <array>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <algorithm>
#include <memory>
#include <vector>

#include "cpu.h"
#include "memory.h"
#include "ppu.h"

#include "status.h"

// Forward declarations
class DebugComponent;
class DebugRenderer;

// Game state structure for components
struct DebugGameState {
    CPU *cpu;
    PPU *ppu;
    Registers *registers;
    Interrupts *interrupts;
    Memory *mmu;
    GameState::Status *status;
};

// Base class for debug components
class DebugComponent {
protected:
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Rect bounds;
    bool visible = true;
    DebugGameState *game_state;

public:
    DebugComponent(SDL_Renderer *renderer, TTF_Font *font, int x, int y, int w, int h) 
        : renderer(renderer), font(font), bounds({x, y, w, h}) {}
    
    virtual ~DebugComponent() = default;
    
    virtual void update(const DebugGameState& state) = 0;
    virtual void render() = 0;
    virtual const char* get_name() const = 0;
    
    // Accessors
    SDL_Rect get_bounds() const { return bounds; }
    bool is_visible() const { return visible; }
    void set_visible(bool v) { visible = v; }
    void set_bounds(int x, int y, int w, int h) { bounds = {x, y, w, h}; }

protected:
    void draw_text(int x_pos, int y_pos, const std::string& text);
    void draw_rectangle(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    std::string to_hex_string(const uint32_t i);
};

class Renderer {
   public:
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *viewport_texture;

    CPU *cpu;
    PPU *ppu;
    Registers *registers;
    Interrupts *interrupts;
    Memory *mmu;
    GameState::Status *status;

    // Viewport
    int viewport_width = 160;
    int viewport_height = 144;
    std::array<uint8_t, 160 * 144 * 4> viewport_pixels;
    SDL_Rect viewport_rect = {0, 0, 160, 144};
    float window_height = 144;
    float window_width = 160;

    int framerate_time = 1000 / 60;
    std::chrono::steady_clock::time_point startFrame;
    std::chrono::steady_clock::time_point endFrame;

    void init_window(float window_width, float window_height);
    void draw_viewport();

   public:
    Renderer(GameState::Status *status, CPU *cpu, PPU *ppu, Registers *registers, Interrupts *interrupts, Memory *mmu);
    void render();
    virtual void init();
    virtual void draw();
};

// Specific debug component classes
class RegisterComponent : public DebugComponent {
    DebugGameState cached_state;
public:
    RegisterComponent(SDL_Renderer *renderer, TTF_Font *font, int x, int y) 
        : DebugComponent(renderer, font, x, y, 200, 150) {}
    
    void update(const DebugGameState& state) override;
    void render() override;
    const char* get_name() const override { return "Registers"; }
};

class TileMapComponent : public DebugComponent {
    SDL_Texture *tilemap_texture;
    std::array<uint8_t, 128 * 256 * 4> tilemap_pixels;
    bool needs_update = true;
    
public:
    TileMapComponent(SDL_Renderer *renderer, TTF_Font *font, int x, int y) 
        : DebugComponent(renderer, font, x, y, 128, 256), tilemap_texture(nullptr) {}
    
    ~TileMapComponent();
    void init();
    void update(const DebugGameState& state) override;
    void render() override;
    const char* get_name() const override { return "TileMap"; }
};

class BackgroundComponent : public DebugComponent {
    SDL_Texture *background_texture;
    std::array<uint8_t, 256 * 256 * 4> background_pixels;
    bool needs_update = true;
    
public:
    BackgroundComponent(SDL_Renderer *renderer, TTF_Font *font, int x, int y) 
        : DebugComponent(renderer, font, x, y, 256, 256), background_texture(nullptr) {}
    
    ~BackgroundComponent();
    void init();
    void update(const DebugGameState& state) override;
    void render() override;
    const char* get_name() const override { return "Background"; }
    void draw_background_overflow(const DebugGameState& state);
};

class SpriteMapComponent : public DebugComponent {
    SDL_Texture *spritemap_texture;
    std::array<uint8_t, 64 * 40 * 4> spritemap_pixels;
    bool needs_update = true;
    
public:
    SpriteMapComponent(SDL_Renderer *renderer, TTF_Font *font, int x, int y) 
        : DebugComponent(renderer, font, x, y, 40 * 2, 64 * 2), spritemap_texture(nullptr) {}
    
    ~SpriteMapComponent();
    void init();
    void update(const DebugGameState& state) override;
    void render() override;
    const char* get_name() const override { return "SpriteMap"; }
};

class StatusComponent : public DebugComponent {
    DebugGameState cached_state;
public:
    StatusComponent(SDL_Renderer *renderer, TTF_Font *font, int x, int y) 
        : DebugComponent(renderer, font, x, y, 400, 150) {}
    
    void update(const DebugGameState& state) override;
    void render() override;
    const char* get_name() const override { return "Status"; }
};

class DebugRenderer : public Renderer {
private:
    std::vector<std::unique_ptr<DebugComponent>> components;
    TTF_Font *font;
    DebugGameState game_state;
    
    int window_height = 144 + 256; // viewport + background height
    int window_width = 256 + 128 + 80; // background + tilemap + spritemap

public:
    using Renderer::Renderer;

    void init() override;
    void draw() override;
    
    // Component management
    template<typename T, typename... Args>
    void add_component(Args&&... args) {
        auto component = std::make_unique<T>(renderer, font, std::forward<Args>(args)...);
        if constexpr (std::is_same_v<T, TileMapComponent> || 
                      std::is_same_v<T, BackgroundComponent> || 
                      std::is_same_v<T, SpriteMapComponent>) {
            component->init();
        }
        components.push_back(std::move(component));
    }
    
    void remove_component(const std::string& name);
    void toggle_component(const std::string& name);
    DebugComponent* get_component(const std::string& name);
};