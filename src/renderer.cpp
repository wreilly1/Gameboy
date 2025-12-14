#include "renderer.h"

class RendererComponents {
public:
    static void render_framebuffer_pixels(PPU *ppu, std::array<uint8_t, 160 * 144 * 4>& viewport_pixels) {
        for (int i = 0; i < 144 * 160; i++) {
            GameBoySystem::Colour colour = ppu->framebuffer[i];
            std::copy(colour.colours, colour.colours + 4, viewport_pixels.begin() + i * 4);
        }
    }
    
    static void regulate_frame_timing(std::chrono::steady_clock::time_point& startFrame, 
                                    std::chrono::steady_clock::time_point& endFrame, int framerate_time) {
        endFrame = std::chrono::steady_clock::now();
        auto timeTook = std::chrono::duration_cast<std::chrono::milliseconds>(endFrame - startFrame).count();
        if (timeTook < framerate_time)
            std::this_thread::sleep_for(std::chrono::milliseconds(framerate_time - timeTook));
        startFrame = std::chrono::steady_clock::now();
    }
    
    static void apply_color_mode(SDL_Texture *viewport_texture, GameState::ColorMode colorMode) {
        switch (colorMode) {
            case GameState::ColorMode::NORMAL:
                SDL_SetTextureColorMod(viewport_texture, 255, 255, 255);
                break;
            case GameState::ColorMode::RETRO:
                SDL_SetTextureColorMod(viewport_texture, 155, 188, 15);
                break;
            case GameState::ColorMode::GRAY:
                SDL_SetTextureColorMod(viewport_texture, 224, 219, 205);
                break;
        }
    }
};

Renderer::Renderer(GameState::Status *status, CPU *cpu, PPU *gpu, Registers *registers, Interrupts *interrupts, Memory *mmu) {
    this->cpu = cpu;
    this->ppu = gpu;
    this->registers = registers;
    this->mmu = mmu;
    this->interrupts = interrupts;
    this->status = status;
    viewport_pixels.fill(0xFF);
    framerate_time = 1000 / 60;
}

void Renderer::init() {
    init_window(window_width, window_height);
    
    viewport_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                       SDL_TEXTUREACCESS_STREAMING, 160, 144);
}

void Renderer::init_window(float window_width, float window_height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                             window_width * 2, window_height * 2, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, window_width, window_height);
    SDL_SetWindowResizable(window, SDL_FALSE);

    SDL_SetWindowTitle(window, mmu->game_cartridge->rom_title.c_str());
}

void Renderer::render() {
    RendererComponents::regulate_frame_timing(startFrame, endFrame, framerate_time);

    RendererComponents::apply_color_mode(viewport_texture, status->display.colorMode);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, viewport_texture);

    draw();

    SDL_RenderCopy(renderer, viewport_texture, NULL, &viewport_rect);
    SDL_RenderPresent(renderer);
}

void Renderer::draw() { 
    draw_viewport(); 
}

void Renderer::draw_viewport() {
    RendererComponents::render_framebuffer_pixels(ppu, viewport_pixels);
    SDL_UpdateTexture(viewport_texture, NULL, viewport_pixels.data(), 160 * 4);
}

// Base DebugComponent helper methods
void DebugComponent::draw_text(int x_pos, int y_pos, const std::string& text) {
    SDL_Color textColor = {0, 0, 0, 0};
    if (!font) {
        return;
    }
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = {x_pos, y_pos, text_width, text_height};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void DebugComponent::draw_rectangle(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

std::string DebugComponent::to_hex_string(const uint32_t i) {
    std::stringstream s;
    s << "0x" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << i;
    return s.str();
}

// RegisterComponent implementation
void RegisterComponent::update(const DebugGameState& state) {
    cached_state = state;
}

void RegisterComponent::render() {
    if (!visible || !cached_state.registers) return;
    
    draw_text(bounds.x, bounds.y, "A: " + std::to_string(+cached_state.registers->a));
    draw_text(bounds.x + 70, bounds.y, "F: " + std::to_string(+cached_state.registers->f));

    draw_text(bounds.x, bounds.y + 20, "B: " + std::to_string(+cached_state.registers->b));
    draw_text(bounds.x + 70, bounds.y + 20, "C: " + std::to_string(+cached_state.registers->c));

    draw_text(bounds.x, bounds.y + 40, "H: " + std::to_string(+cached_state.registers->h));
    draw_text(bounds.x + 70, bounds.y + 40, "L: " + std::to_string(+cached_state.registers->l));

    draw_text(bounds.x, bounds.y + 60, "SP: " + std::to_string(+cached_state.registers->sp));
    draw_text(bounds.x, bounds.y + 80, "PC: " + std::to_string(+cached_state.registers->pc));
}

// TileMapComponent implementation
TileMapComponent::~TileMapComponent() {
    if (tilemap_texture) {
        SDL_DestroyTexture(tilemap_texture);
    }
}

void TileMapComponent::init() {
    tilemap_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                       SDL_TEXTUREACCESS_STREAMING, 128, 256);
    tilemap_pixels.fill(0xFF);
}

void TileMapComponent::update(const DebugGameState& state) {
    if (!visible) return;
    
    for (int i = 0; i < 384; i++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                uint8_t colour_n = state.mmu->graphics_tiles[i].pixel_matrix[y][x];
                int offsetX = ((i * 8 + x) % 128);
                int offsetY = y + (int(i / 16)) * 8;
                int offset = 4 * (offsetY * 128 + offsetX);

                GameBoySystem::Colour colour = state.mmu->background_colors[colour_n];
                std::copy(colour.colours, colour.colours + 4, tilemap_pixels.begin() + offset);
            }
        }
    }
    
    SDL_UpdateTexture(tilemap_texture, NULL, tilemap_pixels.data(), 128 * 4);
}

void TileMapComponent::render() {
    if (!visible || !tilemap_texture) return;
    
    SDL_Rect dest_rect = {bounds.x, bounds.y, bounds.w, bounds.h};
    SDL_RenderCopy(renderer, tilemap_texture, NULL, &dest_rect);
}

// BackgroundComponent implementation
BackgroundComponent::~BackgroundComponent() {
    if (background_texture) {
        SDL_DestroyTexture(background_texture);
    }
}

void BackgroundComponent::init() {
    background_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                          SDL_TEXTUREACCESS_STREAMING, 256, 256);
    background_pixels.fill(0xFF);
}

void BackgroundComponent::update(const DebugGameState& state) {
    if (!visible) return;
    
    // Draw background tiles
    for (int i = 0; i <= 1023; i++) {
        int tile = state.mmu->read_memory_byte(0x9800 + i);
        if (!state.ppu->control->bgWindowDataSelect && tile < 128)
            tile += 256;

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                uint8_t color = state.mmu->graphics_tiles[tile].pixel_matrix[y][x];
                int xi = (i % 32) * 8 + x;
                int yi = (i / 32) * 8 + y;
                int offset = 4 * (yi * 256 + xi);
                GameBoySystem::Colour colour = state.mmu->background_colors[color];
                std::copy(colour.colours, colour.colours + 4, background_pixels.begin() + offset);
            }
        }
    }

    // Draw sprites
    for (auto sprite : state.mmu->sprite_data) {
        if (!sprite.is_active)
            continue;
        for (int tile_num = 0; tile_num < 1 + int(state.ppu->control->spriteSize); tile_num++) {
            int y_pos = sprite.y_position + tile_num * 8;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    uint8_t xF = sprite.flags.flip_x ? 7 - x : x;
                    uint8_t yF = sprite.flags.flip_y ? 7 - y : y;

                    int tile = sprite.tile_number & (state.ppu->control->spriteSize ? 0xFE : 0xFF);
                    uint8_t colour_n = state.mmu->graphics_tiles[tile + tile_num].pixel_matrix[yF][xF];

                    if (!colour_n)
                        continue;
                    int xi = (state.mmu->read_memory_byte(0xff43) + sprite.x_position + x) % 256;
                    int yi = (state.mmu->read_memory_byte(0xff42) + y_pos + y) % 256;
                    int offset = 4 * (yi * 256 + xi);

                    if (static_cast<size_t>(offset) >= background_pixels.size())
                        continue;
                    GameBoySystem::Colour colour = sprite.color_palette[colour_n];
                    std::copy(colour.colours, colour.colours + 4, background_pixels.begin() + offset);
                }
            }
        }
    }
    
    SDL_UpdateTexture(background_texture, NULL, background_pixels.data(), 256 * 4);
}

void BackgroundComponent::render() {
    if (!visible || !background_texture) return;
    
    SDL_Rect dest_rect = {bounds.x, bounds.y, bounds.w, bounds.h};
    SDL_RenderCopy(renderer, background_texture, NULL, &dest_rect);
}

void BackgroundComponent::draw_background_overflow(const DebugGameState& state) {
    if (!visible) return;
    
    int overflowX = std::max(*state.ppu->scrollX + 160 - 256, 0);
    int overflowY = std::max(*state.ppu->scrollY + 144 - 256, 0);

    draw_rectangle(bounds.x + *state.ppu->scrollX, 144 + *state.ppu->scrollY, 
                   160 - overflowX, 144, 255, 255, 255, 100);

    if (overflowX)
        draw_rectangle(bounds.x, 144 + *state.ppu->scrollY, overflowX, 144, 255, 255, 255, 100);

    if (overflowY)
        draw_rectangle(bounds.x + *state.ppu->scrollX, 144, 160 - overflowX, overflowY, 255, 255, 255, 100);

    if (overflowX && overflowY)
        draw_rectangle(bounds.x, 144, overflowX, overflowY, 255, 255, 255, 100);
}

// SpriteMapComponent implementation
SpriteMapComponent::~SpriteMapComponent() {
    if (spritemap_texture) {
        SDL_DestroyTexture(spritemap_texture);
    }
}

void SpriteMapComponent::init() {
    spritemap_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                         SDL_TEXTUREACCESS_STREAMING, 40, 64);
    spritemap_pixels.fill(0xFF);
}

void SpriteMapComponent::update(const DebugGameState& state) {
    if (!visible) return;
    
    auto draw_sprite = [this, &state](GameBoySystem::Graphics::SpriteAttributes sprite, int tile_off, int off_x, int off_y) {
        if (!sprite.is_active)
            return;
        for (int x = 0; x < 8; x++) {
            uint8_t xF = sprite.flags.flip_x ? 7 - x : x;
            for (int y = 0; y < 8; y++) {
                uint8_t yF = sprite.flags.flip_y ? 7 - y : y;
                uint8_t colour_n = state.mmu->graphics_tiles[sprite.tile_number + tile_off].pixel_matrix[yF][xF];
                int offsetX = ((off_x + x) % 40);
                int offsetY = y + off_y;
                int offset = 4 * (offsetY * 40 + offsetX);

                GameBoySystem::Colour colour = sprite.color_palette[colour_n];
                std::copy(colour.colours, colour.colours + 4, spritemap_pixels.begin() + offset);
            }
        }
    };

    if (state.ppu->control->spriteSize) {
        for (int i = 0, row = 0; i < 20; i++) {
            draw_sprite(state.mmu->sprite_data[i], 0, i * 8, row * 8);
            draw_sprite(state.mmu->sprite_data[i], 1, i * 8, row * 8 + 8);
            if (((i + 1) % 5) == 0)
                row += 2;
        }
    } else {
        for (int i = 0, row = 0; i < 40; i++) {
            draw_sprite(state.mmu->sprite_data[i], 0, i * 8, row * 8);
            if (((i + 1) % 5) == 0)
                row++;
        }
    }

    SDL_UpdateTexture(spritemap_texture, NULL, spritemap_pixels.data(), 40 * 4);
}

void SpriteMapComponent::render() {
    if (!visible || !spritemap_texture) return;
    
    SDL_Rect dest_rect = {bounds.x, bounds.y, bounds.w, bounds.h};
    SDL_RenderCopy(renderer, spritemap_texture, NULL, &dest_rect);
}

// StatusComponent implementation
void StatusComponent::update(const DebugGameState& state) {
    cached_state = state;
}

void StatusComponent::render() {
    if (!visible || !cached_state.mmu) return;
    
    int x = bounds.x;
    int y = bounds.y;
    
    draw_text(x, y, "Ticks: " + std::to_string(+cached_state.mmu->cpu_clock.total_cycles));
    draw_text(x, y + 20, "IME: " + std::to_string(+cached_state.interrupts->is_master_enabled()));
    draw_text(x + 70, y + 20, "HALT: " + std::to_string(+cached_state.mmu->memory_flags.cpu_halted));

    draw_text(x, y + 40, "IE:   " + to_hex_string(cached_state.mmu->read_memory_byte(0xFFFF)) + 
              " (" + std::bitset<8>(cached_state.mmu->read_memory_byte(0xFFFF)).to_string() + ")");
    draw_text(x, y + 60, "IF:   " + to_hex_string(cached_state.mmu->read_memory_byte(0xFF0F)) + 
              " (" + std::bitset<8>(cached_state.mmu->read_memory_byte(0xFF0F)).to_string() + ")");
    draw_text(x, y + 80, "LCDC: " + to_hex_string(cached_state.mmu->read_memory_byte(0xFF40)) + 
              " (" + std::bitset<8>(cached_state.mmu->read_memory_byte(0xFF40)).to_string() + ")");
    draw_text(x, y + 100, "DIV:  " + to_hex_string(cached_state.mmu->read_memory_byte(0xFF04)) + 
              " (" + std::bitset<8>(cached_state.mmu->read_memory_byte(0xFF04)).to_string() + ")");

    // Draw paused message
    if (cached_state.status && cached_state.status->flags.isPaused) {
        draw_rectangle(bounds.x + bounds.w - 58, bounds.y + bounds.h - 22, 55, 20, 255, 0, 0, 100);
        draw_text(bounds.x + bounds.w - 55, bounds.y + bounds.h - 22, "STOPPED");
    }
}

// DebugRenderer implementation
void DebugRenderer::init() {
    viewport_pixels.fill(0xFF);
    init_window(window_width, window_height);

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
        SDL_Quit();
    }

    font = TTF_OpenFont("fonts/VT323-Regular.ttf", 18);

    viewport_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                        viewport_width, viewport_height);

    // Setup game state
    game_state.cpu = cpu;
    game_state.ppu = ppu;
    game_state.registers = registers;
    game_state.interrupts = interrupts;
    game_state.mmu = mmu;
    game_state.status = status;

    // Add default components
    add_component<RegisterComponent>(viewport_width + 10, 0);
    add_component<StatusComponent>(viewport_width + 150, 0);
    add_component<TileMapComponent>(0, 144);
    add_component<SpriteMapComponent>(128, 144);
    add_component<BackgroundComponent>(208, 144);
}

void DebugRenderer::draw() {
    // Update game state
    game_state.cpu = cpu;
    game_state.ppu = ppu;
    game_state.registers = registers;
    game_state.interrupts = interrupts;
    game_state.mmu = mmu;
    game_state.status = status;

    // Draw main viewport
    draw_viewport();

    // Update and render all components
    for (auto& component : components) {
        if (component->is_visible()) {
            component->update(game_state);
            component->render();
        }
    }

    // Draw background overflow if BackgroundComponent exists
    auto* bg_component = dynamic_cast<BackgroundComponent*>(get_component("Background"));
    if (bg_component && bg_component->is_visible()) {
        bg_component->draw_background_overflow(game_state);
    }
}

void DebugRenderer::remove_component(const std::string& name) {
    components.erase(
        std::remove_if(components.begin(), components.end(),
                      [&name](const auto& component) {
                          return std::string(component->get_name()) == name;
                      }),
        components.end());
}

void DebugRenderer::toggle_component(const std::string& name) {
    auto* component = get_component(name);
    if (component) {
        component->set_visible(!component->is_visible());
    }
}

DebugComponent* DebugRenderer::get_component(const std::string& name) {
    auto it = std::find_if(components.begin(), components.end(),
                          [&name](const auto& component) {
                              return std::string(component->get_name()) == name;
                          });
    return it != components.end() ? it->get() : nullptr;
}