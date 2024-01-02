#include "include/raylib/raylib.h"
#include "include/raylib/raymath.h"
#include "./food.h"
#include "./snake.h"

Food::Food(std::deque<Vector2> snake_body) {
    red_apple = LoadTexture("./assets/texture/red_apple.png");
    position = get_random_pos(snake_body);
}

Food::~Food() {
    UnloadTexture(red_apple);
}

bool Food::pos_on_body(Vector2 food_pos, std::deque<Vector2> body)
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

Vector2 Food::get_random_grid() {
    float x = GetRandomValue(0, GRID_COUNT - 1);
    float y = GetRandomValue(0, GRID_COUNT - 1);
    return Vector2{x, y};
}


Vector2 Food::get_random_pos(std::deque<Vector2> snake_body) {
    Vector2 position = get_random_grid();
    while (pos_on_body(position, snake_body))
    {
        position = get_random_grid();
    }
    return position;
}

void Food::draw() {
    DrawTexture(
        red_apple,
        (GRID_SIZE * this->position.x), (GRID_SIZE * this->position.y),
        WHITE);

}

void Food::update_position() {}

