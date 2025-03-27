#ifndef RENDERER_H
#define RENDERER_H

#include "gamestate.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <vector>

class TTF_Font;

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height,
           TTF_Font *font);
  ~Renderer();

  void Render(GameState state, Snake const snake, SDL_Point const &food);
  void UpdateWindowTitle(GameState state, int score, int fps);

private:
  void RenderStartScreen();
  void RenderRunningScreen(const SDL_Point &food, const Snake &snake);  

  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  TTF_Font *font;
};

#endif