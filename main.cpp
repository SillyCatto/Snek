#include "include/raylib/raylib.h"
#include "./game.h"

int main() {
    Game *game = new Game();
    game->init();
    SetExitKey(0);

    // game loop
    while (!WindowShouldClose()) {
        if (game->quit()) {
            break;
        }
        // logic
        game->update();
        // drawing
        BeginDrawing();
        game->draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
