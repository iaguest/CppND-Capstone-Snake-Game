#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <SDL2/SDL.h>
#include <string>

class TTF_Font;

class RenderText {
public:
  RenderText(std::string text, SDL_Renderer *sdl_renderer, TTF_Font *font);
  ~RenderText();

  void Render();

private:
  SDL_Renderer *sdl_renderer{nullptr};
  SDL_Texture *textTexture{nullptr};
  SDL_Rect textRect{};

  std::string text;
  TTF_Font *font{nullptr}; // no ownership
};

#endif