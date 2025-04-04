#include "game.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <utility>

namespace {
constexpr const char *kOrg = "IGApps";
constexpr const int kObstacleCount = 3;
} // namespace

std::string Game::Name = "Snake Game";

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : grid_width(grid_width), userName(""), snake(grid_width, grid_height),
      engine(dev()), random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  state.store(GameState::START_SCREEN);
  high_score = ReadHighScore();

  InitialiseObstacles(grid_width, grid_height);
  PlaceFood();
}

Game::~Game() {
  if (obstacleThread.joinable()) {
    obstacleThread.join();
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  while (state.load() != GameState::GAME_OVER) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(std::ref(state), userName, snake);
    Update();
    Render(renderer);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(std::ref(state), high_score, score,
                                 frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  if (score > high_score.second) {
    WriteHighScore(std::make_pair(userName, score));
  }

  OnExit();
}

void Game::Render(Renderer &renderer) {
  std::lock_guard<std::mutex> lock(obstacleMutex);
  renderer.Render(std::ref(state), snake, food, obstacles, userName);
}

void Game::OnExit() const {
  SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  if (!snake.alive) {
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
      if (e.type == SDL_KEYDOWN)
        break;
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied before placing food.
    if (!snake.SnakeCell(x, y) && !IsObstacleRow(y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::InitialiseObstacles(std::size_t grid_width,
                               std::size_t grid_height) {
  const int separation = grid_height / kObstacleCount;
  const int obstacleWidth = grid_width / 3;

  std::lock_guard<std::mutex> lock(obstacleMutex);
  for (int i = 0; i < kObstacleCount; ++i) {
    SDL_Rect rect;
    rect.x = (grid_width - obstacleWidth) / 2;
    rect.y = (i * separation) + (separation / 2);
    rect.h = 1;
    rect.w = obstacleWidth;

    MovingObstacle::Direction dir = (i % 2 == 0)
                                        ? MovingObstacle::Direction::LEFT
                                        : MovingObstacle::Direction::RIGHT;

    obstacles.push_back({rect, dir});
  }
}

bool Game::IsObstacleCell(int x, int y) {
  SDL_Point p{x, y};
  std::lock_guard<std::mutex> lock(obstacleMutex);
  for (const auto &obstacle : obstacles) {
    if (SDL_PointInRect(&p, &obstacle.rect)) {
      return true;
    }
  }

  return false;
}

bool Game::IsObstacleRow(int y) {
  std::lock_guard<std::mutex> lock(obstacleMutex);
  for (const auto &obstacle : obstacles) {
    if (y == obstacle.rect.y) {
      return true;
    }
  }

  return false;
}

void Game::Update() {
  const bool isTextInputActive = SDL_IsTextInputActive();
  if (!snake.alive) {
    state.store(GameState::GAME_OVER);
    return;
  } else if (state.load() == GameState::START_SCREEN && !isTextInputActive) {
    SDL_StartTextInput(); // Start typing
  } else if (state.load() == GameState::RUNNING) {
    if (isTextInputActive) {
      SDL_StopTextInput(); // Done typing
    }
    if (!obstacleThread.joinable()) {
      // Fire up obstacle movement thread
      obstacleThread = std::thread(&Game::UpdateObstacles, this, grid_width);
    }

    snake.Update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y) {
      score++;
      PlaceFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }

    // Check if snake has collided with an obstacle
    if (IsObstacleCell(snake.head_x, snake.head_y) ||
        std::any_of(snake.body.begin(), snake.body.end(),
                    [this](const SDL_Point &p) {
                      return this->IsObstacleCell(p.x, p.y);
                    })) {
      snake.alive = false;
      return;
    }
  }
}

void Game::UpdateObstacles(int grid_width) {
  const int speed = 1;
  const int sleepMs = 50;

  while (state.load() == GameState::RUNNING) {
    {
      std::lock_guard<std::mutex> lock(obstacleMutex);
      for (MovingObstacle &obstacle : obstacles) {
        if (obstacle.dir == MovingObstacle::Direction::LEFT) {
          obstacle.rect.x -= speed;
          if (obstacle.rect.x <= 0) {
            obstacle.rect.x = 0;
            obstacle.dir = MovingObstacle::Direction::RIGHT;
          }
        } else {
          obstacle.rect.x += speed;
          if (obstacle.rect.x + obstacle.rect.w >= grid_width) {
            obstacle.rect.x = grid_width - obstacle.rect.w;
            obstacle.dir = MovingObstacle::Direction::LEFT;
          }
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

std::string Game::GetHighScorePath() const {
  char *path = SDL_GetPrefPath(kOrg, Game::Name.c_str());
  std::string fullPath = std::string(path) + "highscore.txt";
  SDL_free(path);
  return fullPath;
}

std::pair<std::string, int> Game::ReadHighScore() const {
  const std::string highScoresPath = GetHighScorePath();
  std::ifstream inFile(highScoresPath);
  std::string line;
  if (std::getline(inFile, line)) {
    std::stringstream ss(line);
    std::string name;
    std::string scoreStr;

    if (std::getline(ss, name, ',') && std::getline(ss, scoreStr)) {
      try {
        return std::make_pair(name, std::stoi(scoreStr));
      } catch (...) {
        std::cerr << "Error parsing high score line: " << line << "\n";
      }
    }
  }

  return std::pair<std::string, int>();
}

void Game::WriteHighScore(const std::pair<std::string, int> &highScore) const {
  const std::string highScoresPath = GetHighScorePath();
  std::ofstream outFile(highScoresPath, std::ios::trunc); // overwrite file

  if (outFile) {
    outFile << highScore.first << "," << highScore.second;
  } else {
    std::cerr << "Failed to write high score to: " << highScoresPath << '\n';
  }
}
