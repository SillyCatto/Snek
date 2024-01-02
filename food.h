#pragma once

#include "include/raylib/raylib.h"
#include <deque>

#define GRID_SIZE 30
#define GRID_COUNT 20

class Food {
  public:
    Food(std::deque<Vector2> snake_body);
    ~Food();

    Vector2 position;

    void draw();
    void update_position();
    Vector2 get_random_pos(std::deque<Vector2> snake_body);

  private:
    Texture2D red_apple;
    Texture2D golden_apple;
    bool pos_on_body(Vector2 element, std::deque<Vector2> body);
    Vector2 get_random_grid();
};