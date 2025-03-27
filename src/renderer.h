#ifndef RENDERER_H
#define RENDERER_H

#include "gamestate.h"
#include "rendertext.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

class RenderText;
class TTF_Font;

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(GameState state, Snake const snake, SDL_Point const &food);
  void UpdateWindowTitle(GameState state, int score, int fps);

private:
  void RenderStartScreen();
  void RenderRunningScreen(const SDL_Point &food, const Snake &snake);

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  SDL_Window *sdl_window{nullptr};
  SDL_Renderer *sdl_renderer{nullptr};
  TTF_Font *font{nullptr};
  std::unique_ptr<RenderText> prompt{nullptr};
};

#endif