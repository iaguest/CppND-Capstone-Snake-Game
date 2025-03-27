#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gamestate.h"
#include "snake.h"
#include <string>

class Controller {
public:
  void HandleInput(GameState &state, std::string &userName, Snake &snake) const;

private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  void HandleStartScreenInput(SDL_Event &e, GameState &state, std::string &userName) const;

  void HandleRunningGameInput(SDL_Event &e, Snake &snake) const;
};

#endif