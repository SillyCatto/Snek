#pragma once

#include "./include/raylib/raylib.h"
#include <deque>

#define GRID_SIZE 30
#define GRID_COUNT 20
#define DIR_RIGHT -90
#define DIR_LEFT 90
#define DIR_DOWN 0
#define DIR_UP 180

class Snake {
  public:
    Snake();
    ~Snake();

    bool growth = false;
    bool is_going_right = true;
    bool is_going_left = false;
    bool is_going_up = false;
    bool is_going_down = false;
    std::deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};

    void draw();
    void update_body();
    void update_movement();
    bool should_move();
    bool is_out_of_screen();
    bool hit_tail();
    Vector2 direction = {1, 0};


  private:
    Vector2 prev_seg, next_seg;
    Texture2D head;
    Texture2D head_up, head_down, head_left, head_right;
    Texture2D body_vertical, body_horizontal;
    Texture2D top_left, top_right, bottom_left, bottom_right;
    Texture2D tail;
    Texture2D tail_up, tail_down, tail_left, tail_right;


    double last_update = 0;


    void update_head();
    void update_tail();
    bool pos_on_body(Vector2 food_pos, std::deque<Vector2> body);
};