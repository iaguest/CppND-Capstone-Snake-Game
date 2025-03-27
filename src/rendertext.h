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

  void SetText(std::string updatedText);

  void Render();

private:
  void CreateTexture(TTF_Font *font, std::string &text,
                     const SDL_Color &textColor, SDL_Renderer *sdl_renderer,
                     int xPos, int yPos);

  std::string text;
  SDL_Renderer *sdl_renderer{nullptr};
  TTF_Font *font{nullptr}; // no ownership
  SDL_Color textColor;
  int xPos;
  int yPos;

  SDL_Texture *textTexture{nullptr};
  SDL_Rect textRect{};
};

#endif