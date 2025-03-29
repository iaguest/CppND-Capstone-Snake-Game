#ifndef GAME_H
#define GAME_H

#include "controller.h"
#include "gamestate.h"
#include "movingobstacle.h"
#include "renderer.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <atomic>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void Render(Renderer &renderer);
  int GetScore() const;
  int GetSize() const;
  void OnExit() const;

  static std::string Name;

private:
  std::string GetHighScorePath() const;
  std::pair<std::string, int> ReadHighScore() const;
  void WriteHighScore(const std::pair<std::string, int> &highScore) const;

  void PlaceFood();
  void InitialiseObstacles(std::size_t grid_width, std::size_t grid_height);
  bool IsObstacleCell(int x, int y);
  bool IsObstacleRow(int y);
  void Update();
  void UpdateObstacles(int grid_width);

  std::atomic<GameState> state;
  std::size_t grid_width;
  std::string userName;
  Snake snake;
  SDL_Point food;
  std::vector<MovingObstacle> obstacles;
  std::mutex obstacleMutex;
  std::thread obstacleThread;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  std::pair<std::string, int> high_score;
};

#endif