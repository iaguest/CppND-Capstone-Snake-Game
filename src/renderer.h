#ifndef RENDERER_H
#define RENDERER_H

#include "gamestate.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <vector>

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(GameState state, Snake const snake, SDL_Point const &food);
  void UpdateWindowTitle(GameState state, int score, int fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif