#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <SDL2/SDL.h>
#include <string>

class TTF_Font;

class RenderText {
public:
  RenderText(std::string text, SDL_Renderer *sdl_renderer, TTF_Font *font,
             SDL_Color textColor, int xPos, int yPos);
  ~RenderText();

  void Render();

private:
  std::string text;
  SDL_Renderer *sdl_renderer{nullptr};
  TTF_Font *font{nullptr}; // no ownership
  SDL_Color textColor;

  SDL_Texture *textTexture{nullptr};
  SDL_Rect textRect{};
};

#endif