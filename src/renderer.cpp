#include "renderer.h"
#include "game.h"
#include "rendertext.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
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
  sdl_window = SDL_CreateWindow(Game::Name.c_str(), SDL_WINDOWPOS_CENTERED,
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

  prompt = std::make_unique<RenderText>("Enter your name: ", sdl_renderer, font,
                                        SDL_Color{255, 255, 255, 255}, 10, 10);
  userName = std::make_unique<RenderText>(
      "", sdl_renderer, font, SDL_Color{255, 255, 255, 255}, 150, 10);
}

Renderer::~Renderer() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(GameState state, Snake const snake, SDL_Point const &food,
                      std::vector<SDL_Rect> const &obstacles,
                      std::string const &userName) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  if (state == GameState::START_SCREEN) {
    RenderStartScreen(userName);
  } else {
    RenderRunningScreen(food, obstacles, snake);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderStartScreen(const std::string &name) {
  prompt->Render();
  userName->SetText(name);
  userName->Render();
}

void Renderer::RenderRunningScreen(const SDL_Point &food,
                                   std::vector<SDL_Rect> const &obstacles,
                                   const Snake &snake) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render obstacles
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
  for (SDL_Rect const &obstacle : obstacles) {
    SDL_Rect rect;
    rect.x = obstacle.x * block.w;
    rect.y = obstacle.y * block.h;
    rect.h = obstacle.h * block.h;
    rect.w = obstacle.w * block.w;

    SDL_RenderFillRect(sdl_renderer, &rect);
  }

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

void Renderer::UpdateWindowTitle(GameState state,
                                 const std::pair<std::string, int> &highScore,
                                 int score, int fps) {
  if (state != GameState::RUNNING)
    return;

  std::ostringstream title;
  title << Game::Name.c_str() << "  |  FPS: " << fps
        << "  |  Current Score: " << score;

  if (!highScore.first.empty() && highScore.second > 0) {
    title << "  |  High Score: " << highScore.first << " " << highScore.second;
  }

  SDL_SetWindowTitle(sdl_window, title.str().c_str());
}
