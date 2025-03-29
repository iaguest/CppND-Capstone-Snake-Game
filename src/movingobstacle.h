#ifndef MOVINGOBSTACLE_H
#define MOVINGOBSTACLE_H

#include <SDL2/SDL.h>

struct MovingObstacle {
  enum class Direction { LEFT, RIGHT };

  SDL_Rect rect;
  Direction dir;
};

#endif MOVINGOBSTACLE_H
