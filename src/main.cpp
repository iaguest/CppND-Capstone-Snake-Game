// #define SDL_MAIN_HANDLED
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  constexpr const char *kFont = "./fonts/Roboto-Regular.ttf";

  if (TTF_Init() == -1) {
    std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
    return 1;
  }

  TTF_Font *font = TTF_OpenFont(kFont, 18);
  if (!font) {
    std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
    return 1;
  }

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, font);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  TTF_CloseFont(font);
  TTF_Quit();

  return 0;
}