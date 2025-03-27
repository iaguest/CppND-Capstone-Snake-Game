#include "rendertext.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

RenderText::RenderText(std::string text, SDL_Renderer *sdl_renderer,
                       TTF_Font *font)
    : text(text), sdl_renderer(sdl_renderer), font(font) {
  SDL_Color textColor = {255, 255, 255, 255};

  SDL_Surface *textSurface =
      TTF_RenderText_Blended(font, text.c_str(), textColor);
  if (!textSurface) {
    std::cerr << "TTF_RenderText_Blended: " << TTF_GetError() << std::endl;
    throw std::runtime_error("Error rendering text");
  }

  textTexture = SDL_CreateTextureFromSurface(sdl_renderer, textSurface);
  if (!textTexture) {
    std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError()
              << std::endl;
    SDL_FreeSurface(textSurface);
    throw std::runtime_error("Error rendering text");
  }

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;

  SDL_FreeSurface(textSurface);

  textRect = {0, 0, textWidth, textHeight};
}

RenderText::~RenderText() {
  if (textTexture) {
    SDL_DestroyTexture(textTexture);
  }
}

void RenderText::Render() {
  if (textTexture) {
    SDL_RenderCopy(sdl_renderer, textTexture, nullptr, &textRect);
  }
}
