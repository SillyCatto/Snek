#include "./snake.h"
#include "./include/raylib/raymath.h"
#include "include/raylib/raylib.h"
#include <deque>
using namespace std;

Snake::Snake() {
    head_up = LoadTexture("./assets/texture/head_up.png");
    head_down = LoadTexture("./assets/texture/head_down.png");
    head_left = LoadTexture("./assets/texture/head_left.png");
    head_right = LoadTexture("./assets/texture/head_right.png");

    body_vertical = LoadTexture("./assets/texture/body_vertical.png");
    body_horizontal = LoadTexture("./assets/texture/body_horizontal.png");

    top_left = LoadTexture("./assets/texture/top_left.png");
    top_right = LoadTexture("./assets/texture/top_right.png");
    bottom_left = LoadTexture("./assets/texture/bottom_left.png");
    bottom_right = LoadTexture("./assets/texture/bottom_right.png");

    tail_up = LoadTexture("./assets/texture/tail_up.png");
    tail_down = LoadTexture("./assets/texture/tail_down.png");
    tail_left = LoadTexture("./assets/texture/tail_left.png");
    tail_right = LoadTexture("./assets/texture/tail_right.png");
}

Snake::~Snake() {
    UnloadTexture(head_up);
    UnloadTexture(head_down);
    UnloadTexture(head_left);
    UnloadTexture(head_right);

    UnloadTexture(body_horizontal);
    UnloadTexture(body_vertical);

    UnloadTexture(top_left);
    UnloadTexture(top_right);
    UnloadTexture(bottom_left);
    UnloadTexture(bottom_right);

    UnloadTexture(tail_up);
    UnloadTexture(tail_down);
    UnloadTexture(tail_left);
    UnloadTexture(tail_right);
}

bool Snake::should_move() {
    double current_time = GetTime();
    if (current_time - last_update >= 0.17) {
        last_update = current_time;
        return true;
    }
    return false;
}

bool Snake::pos_on_body(Vector2 food_pos, std::deque<Vector2> body)
{
    for (unsigned int i = 0; i < body.size(); i++)
    {
        if (Vector2Equals(body[i], food_pos))
        {
            return true;
        }
    }
    return false;
}

void Snake::update_body() {
    body.push_front(Vector2Add(body[0], direction));
    if (growth) {
        growth = false;
    }
    else {
        body.pop_back();
    }
}

bool Snake::is_out_of_screen() {
    if (this->body[0].x == GRID_COUNT || this->body[0].x == -1 ||
        this->body[0].y == GRID_COUNT || this->body[0].y == -1) {
        return true;
    }
    return false;
}

bool Snake::hit_tail() {
    std::deque<Vector2> headless = body;
    headless.pop_front();
    if (pos_on_body(body[0], headless)) {
        return true;
    }
    return false;
}

void Snake::update_movement() {
    if (IsKeyPressed(KEY_UP) && !is_going_down) {
        this->direction = {0, -1};
        is_going_right = false;
        is_going_left = false;
        is_going_up = true;
        is_going_down = false;
    }
    if (IsKeyPressed(KEY_DOWN) && !is_going_up) {
        this->direction = {0, 1};
        is_going_right = false;
        is_going_left = false;
        is_going_up = false;
        is_going_down = true;
    }
    if (IsKeyPressed(KEY_RIGHT) && !is_going_left) {
        this->direction = {1, 0};
        is_going_right = true;
        is_going_left = false;
        is_going_up = false;
        is_going_down = false;
    }
    if (IsKeyPressed(KEY_LEFT) && !is_going_right) {
        this->direction = {-1, 0};
        is_going_right = false;
        is_going_left = true;
        is_going_up = false;
        is_going_down = false;
    }
}

void Snake::update_head() {
    Vector2 head_relation = Vector2Subtract(body[1], body[0]);
    if (Vector2Equals(head_relation, Vector2{1, 0})) {
        this->head = head_left;
    }
    else if (Vector2Equals(head_relation, Vector2{-1, 0})) {
        this->head = head_right;
    }
    if (Vector2Equals(head_relation, Vector2{0, 1})) {
        this->head = head_up;
    }
    if (Vector2Equals(head_relation, Vector2{0, -1})) {
        this->head = head_down;
    }
}

void Snake::update_tail() {
    Vector2 tail_relation =
        Vector2Subtract(body[body.size() - 2], body[body.size() - 1]);
    if (Vector2Equals(tail_relation, Vector2{1, 0})) {
        this->tail = tail_left;
    }
    else if (Vector2Equals(tail_relation, Vector2{-1, 0})) {
        this->tail = tail_right;
    }
    if (Vector2Equals(tail_relation, Vector2{0, 1})) {
        this->tail = tail_up;
    }
    if (Vector2Equals(tail_relation, Vector2{0, -1})) {
        this->tail = tail_down;
    }
}

void Snake::draw() {
    update_head();
    update_tail();
    for (unsigned int i = 0; i < body.size(); i++) {
        Vector2 segment_pos =
            Vector2{static_cast<float>(body[i].x * GRID_SIZE),
                    static_cast<float>(body[i].y * GRID_SIZE)};

        if (i == 0) {
            DrawTextureV(this->head, segment_pos, WHITE);
        }
        else if (i == body.size() - 1) {
            DrawTextureV(this->tail, segment_pos, WHITE);
        }
        else {
            prev_seg = Vector2Subtract(body[i + 1], body[i]);
            next_seg = Vector2Subtract(body[i - 1], body[i]);
            if (prev_seg.x == next_seg.x) {
                DrawTextureV(body_vertical, segment_pos, WHITE);
            }
            else if (prev_seg.y == next_seg.y) {
                DrawTextureV(body_horizontal, segment_pos, WHITE);
            }
            else {
                if ((prev_seg.x == -1 && next_seg.y == -1) ||
                    (prev_seg.y == -1 && next_seg.x == -1)) {
                    DrawTextureV(top_left, segment_pos, WHITE);
                }
                else if ((prev_seg.x == -1 && next_seg.y == 1) ||
                         (prev_seg.y == 1 && next_seg.x == -1)) {
                    DrawTextureV(bottom_left, segment_pos, WHITE);
                }
                else if ((prev_seg.x == 1 && next_seg.y == -1) ||
                         (prev_seg.y == -1 && next_seg.x == 1)) {
                    DrawTextureV(top_right, segment_pos, WHITE);
                }
                else {
                    DrawTextureV(bottom_right, segment_pos, WHITE);
                }
            }
        }
    }
}