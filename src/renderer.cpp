#include "renderer.h"
#include "rendertext.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
constexpr char *kFont = "./fonts/Roboto-Regular.ttf";
constexpr int kFontSize = 18;
} // namespace

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width), screen_height(screen_height),
      grid_width(grid_width), grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    throw std::runtime_error("SDL initialisation failed");
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    throw std::runtime_error("Window creation failed");
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    throw std::runtime_error("Renderer creation failed");
  }

  // Initialise ttf
  if (TTF_Init() == -1) {
    std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
    throw std::runtime_error("TTF initialisation failed");
  }

  // Open the font
  font = TTF_OpenFont(kFont, kFontSize);
  if (!font) {
    std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
    throw std::runtime_error("Failed to open font");
  }

  prompt =
      std::make_unique<RenderText>("Enter your name: ", sdl_renderer, font);
}

Renderer::~Renderer() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(GameState state, Snake const snake,
                      SDL_Point const &food) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  if (state == GameState::START_SCREEN) {
    RenderStartScreen();
  } else if (state == GameState::RUNNING) {
    RenderRunningScreen(food, snake);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderStartScreen() { prompt->Render(); }

void Renderer::RenderRunningScreen(const SDL_Point &food, const Snake &snake) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::UpdateWindowTitle(GameState state, int score, int fps) {
  if (state != GameState::RUNNING)
    return;

  std::string title{"Snake Score: " + std::to_string(score) +
                    " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
