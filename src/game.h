#ifndef GAME_H
#define GAME_H

#include "controller.h"
#include "gamestate.h"
#include "renderer.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <random>
#include <string>
#include <vector>

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void OnExit() const;

  static std::string Name;

private:
  std::string GetHighScorePath() const;
  std::pair<std::string, int> ReadHighScore() const;
  void WriteHighScore(const std::pair<std::string, int> &highScore) const;

  void PlaceFood();
  void PlaceObstacles(std::size_t grid_width, std::size_t grid_height);
  bool IsObstacleCell(int x, int y);
  void Update();

  GameState state;
  std::string userName;
  Snake snake;
  SDL_Point food;
  std::vector<SDL_Rect> obstacles;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  std::pair<std::string, int> high_score;
};

#endif