#pragma once

#include "./food.h"
#include "./snake.h"
#include "include/raylib/raylib.h"
#include <iostream>

#define SCR_WIDTH 600
#define SCR_HEIGHT 600

class Game {
  public:
    Game();
    ~Game();
    void init();
    void update();
    void draw();
    bool quit();

    typedef enum { TITLE = 0, TIMER, GAME_PLAY, PAUSED, GAME_OVER } Game_Screen;
    Game_Screen curr_screen = TITLE;

  private:
    // initialize variables
    int frame_count = 0;
    int score = 0;
    int seconds_to_start = 0;
    bool close = false;

    Color title_bg = {141, 201, 198, 255};
    Color game_bg = {173, 204, 96, 255};
    Food *food;
    Snake *snake;
    Texture2D title_tex;
    Texture2D start_tex;
    Texture2D oops_tex;
    Texture2D restart_tex;
    Texture2D quit_tex;
    Texture2D paused_tex;

    Music title_music;
    Sound crunch;
    Sound oof;

    Rectangle start_btn_bound;
    Rectangle restart_btn_bound;
    Rectangle quit_btn_bound;

    void draw_title();
    void draw_timer();
    void draw_gameplay();
    void draw_paused();
    void draw_gameover();
    void reset();

    bool eat();
};