#include "./game.h"
#include "include/raylib/raymath.h"

Game::Game() {}

Game::~Game() {
    delete food;
    delete snake;

    UnloadTexture(title_tex);
    UnloadTexture(start_tex);
    UnloadTexture(oops_tex);
    UnloadTexture(restart_tex);
    UnloadTexture(quit_tex);
    UnloadTexture(paused_tex);

    UnloadSound(crunch);
    UnloadSound(oof);
    UnloadMusicStream(title_music);

    CloseAudioDevice();
    CloseWindow();
}

void Game::init() {
    // init window
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Snake");
    SetExitKey(0);
    SetTargetFPS(60);
    SetRandomSeed(time(0));
    InitAudioDevice();

    // create objects
    snake = new Snake();
    food = new Food(snake->body);

    // load textures
    title_tex = LoadTexture("./assets/texture/title.png");
    start_tex = LoadTexture("./assets/texture/start.png");
    oops_tex = LoadTexture("./assets/texture/oops.png");
    restart_tex = LoadTexture("./assets/texture/restart.png");
    quit_tex = LoadTexture("./assets/texture/quit.png");
    paused_tex = LoadTexture("./assets/texture/paused.png");

    // load sound
    title_music = LoadMusicStream("./assets/sound/snek-title.mp3");
    crunch = LoadSound("./assets/sound/crunch.mp3");
    oof = LoadSound("./assets/sound/oof.mp3");

    start_btn_bound = {234.0f, 400.0f, 132.0f, 40.0f};
    restart_btn_bound = {225.0f, 350.0f, 150.0f, 32.0f};
    quit_btn_bound = {260.0f, 420.0f, 80.0f, 38.0f};

    SetMusicVolume(title_music, 10.0f);
    PlayMusicStream(title_music);
}


// game logic
void Game::update() {
    switch (this->curr_screen) {
    case TITLE: {
        frame_count++;

        UpdateMusicStream(title_music);
        if (CheckCollisionPointRec(GetMousePosition(), start_btn_bound) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            curr_screen = TIMER;
        }
    } break;

    case TIMER: {
        frame_count++;

        // count to 3 seconds before starting gameplay
        if (((frame_count / 60) % 2) == 1) {
            seconds_to_start++;
            frame_count = 0;
            if (seconds_to_start == 4) {
                seconds_to_start = 0;
                frame_count = 0;
                curr_screen = GAME_PLAY;
            }
        }
    } break;

    case GAME_PLAY: {
        StopMusicStream(title_music);
        snake->update_movement();
        if (snake->should_move()) {
            snake->update_body();
        }
        if (eat()) {
            PlaySound(crunch);
            score++;
            food->position = food->get_random_pos(snake->body);
            snake->growth = true;
        }
        if (IsKeyPressed(KEY_P)) {
            curr_screen = PAUSED;
        }
        if (snake->is_out_of_screen()) {
            PlaySound(oof);
            curr_screen = GAME_OVER;
        }
        if (snake->hit_tail()) {
            PlaySound(oof);
            curr_screen = GAME_OVER;
        }

    } break;

    case PAUSED: {
        frame_count++;
        if (IsKeyPressed(KEY_P)) {
            curr_screen = GAME_PLAY;
            frame_count = 0;
        }
    } break;

    case GAME_OVER: {
        frame_count++;
        if (CheckCollisionPointRec(GetMousePosition(), restart_btn_bound)
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            reset();
        }
    }

    default:
        break;
    }
}

bool Game::eat() {
    if (Vector2Equals(snake->body[0], food->position)) {
        return true;
    }
    else {
        return false;
    }
}

void Game::draw_title() {
    ClearBackground(title_bg);
    DrawTexture(title_tex, 0, 0, WHITE);
    DrawTexture(start_tex, 234, 400, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), start_btn_bound)) {
        DrawTexture(start_tex, 234, 400, {0, 0, 0, 110});
    }
}

void Game::draw_timer() {
    ClearBackground(game_bg);
    DrawText("score: ", 20, 10, 30, DARKGREEN);
    DrawText(TextFormat("%i", score), 120, 10, 30 ,DARKGREEN);
    food->draw();
    snake->draw();
    DrawRectangle(0, 0, SCR_WIDTH, SCR_HEIGHT,
                  {0, 0, 0, 125});
    if (seconds_to_start >= 1) {
        DrawText(
            TextFormat("%i", seconds_to_start),
            (SCR_WIDTH / 2) -
                (MeasureText(TextFormat("%i", seconds_to_start), 80) / 2),
            200, 70, YELLOW);
    }
}

void Game::draw_gameplay() {
    ClearBackground(game_bg);
    DrawText("score: ", 20, 10, 30, DARKGREEN);
    DrawText(TextFormat("%i", score), 120, 10, 30 ,DARKGREEN);
    food->draw();
    snake->draw();
}

void Game::draw_paused() {
    ClearBackground(game_bg);
    DrawText("score: ", 20, 10, 30, DARKGREEN);
    DrawText(TextFormat("%i", score), 120, 10, 30 ,DARKGREEN);
    food->draw();
    snake->draw();

    DrawRectangle(0, 0, SCR_WIDTH, SCR_HEIGHT,
                  {0, 0, 0, 125});
    DrawTexture(paused_tex, 205, 100, WHITE);
}

void Game::draw_gameover() {
    ClearBackground(game_bg);
    food->draw();
    if ((frame_count / 30) % 2) {
        snake->draw();
    }
    DrawRectangle(0, 0, SCR_WIDTH, SCR_HEIGHT,
                  {0, 0, 0, 125});
    DrawTexture(oops_tex, 177, 100, WHITE);

    DrawText(TextFormat("score: %i", score),
             (SCR_WIDTH / 2) -
                 (MeasureText(TextFormat("score: %i", score), 40) / 2),
             250, 40, LIGHTGRAY);

    DrawTexture(restart_tex, 225, 350, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), restart_btn_bound)) {
        DrawTexture(restart_tex, 225, 350, {0, 0, 0, 110});
    }

    DrawTexture(quit_tex, 260, 420, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), quit_btn_bound)) {
        DrawTexture(quit_tex, 260, 420, {0, 0, 0, 110});
    }
}


// drawing all the scenes
void Game::draw() {
    switch (curr_screen) {
    case TITLE: {
        draw_title();
    } break;

    case TIMER: {
        draw_timer();
    } break;

    case GAME_PLAY: {
        draw_gameplay();
    } break;

    case PAUSED: {
        draw_paused();
    } break;

    case GAME_OVER: {
        draw_gameover();
    }

    default:
        break;
    }
}

void Game::reset() {
    snake->is_going_right = true;
    snake->is_going_left = false;
    snake->is_going_up = false;
    snake->is_going_down = false;
    snake->direction = {1, 0};
    snake->body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    food->position = food->get_random_pos(snake->body);
    score = 0;
    frame_count = 0;
    curr_screen = TIMER;
}

bool Game::quit() {
    if (curr_screen == GAME_OVER &&
        CheckCollisionPointRec(GetMousePosition(), quit_btn_bound) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
    }
    else {
        return false;
    }
}
