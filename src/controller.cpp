#include "controller.h"
#include "gamestate.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <iostream>

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1)
    snake.direction = input;
  return;
}

void Controller::HandleInput(GameState &state, std::string &userName,
                             Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      state = GameState::EXITING;
    } else if (state == GameState::START_SCREEN) {
      HandleStartScreenInput(e, state, userName);
    } else if (state == GameState::RUNNING) {
      HandleRunningGameInput(e, snake);
    }
  }
}

void Controller::HandleRunningGameInput(SDL_Event &e, Snake &snake) const {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
    case SDLK_UP:
      ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
      break;

    case SDLK_DOWN:
      ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
      break;

    case SDLK_LEFT:
      ChangeDirection(snake, Snake::Direction::kLeft, Snake::Direction::kRight);
      break;

    case SDLK_RIGHT:
      ChangeDirection(snake, Snake::Direction::kRight, Snake::Direction::kLeft);
      break;
    }
  }
}

void Controller::HandleStartScreenInput(SDL_Event &e, GameState &state,
                                        std::string &userName) const {
  if (e.type == SDL_TEXTINPUT) {
    userName += e.text.text;
  }
  if (e.type == SDL_KEYDOWN) {
    if (e.key.keysym.sym == SDLK_BACKSPACE && !userName.empty()) {
      userName.pop_back();
    } else if (e.key.keysym.sym == SDLK_RETURN) {
      state = GameState::RUNNING;
    }
  }
}
