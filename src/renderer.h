#ifndef RENDERER_H
#define RENDERER_H

#include "gamestate.h"
#include "rendertext.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <atomic>
#include <memory>
#include <utility>
#include <vector>

class RenderText;
class TTF_Font;

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(std::atomic<GameState> &state, Snake const snake, SDL_Point const &food,
              std::vector<SDL_Rect> const &obstacles, std::string const &userName);
  void UpdateWindowTitle(std::atomic<GameState> &state,
                         const std::pair<std::string, int> &highScore,
                         int score, int fps);

private:
  void RenderStartScreen(const std::string &name);
  void RenderRunningScreen(const SDL_Point &food, std::vector<SDL_Rect> const &obstacles, const Snake &snake);

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  SDL_Window *sdl_window{nullptr};
  SDL_Renderer *sdl_renderer{nullptr};
  TTF_Font *font{nullptr};
  std::unique_ptr<RenderText> prompt{nullptr};
  std::unique_ptr<RenderText> userName{nullptr};
};

#endif