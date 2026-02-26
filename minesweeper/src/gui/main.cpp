#include "raylib.h"
#include "model/Game.h"
#include "model/Difficulty.h"
#include "model/GameState.h"
#include "gui/GuiRenderer.h"
#include <optional>

// Applikationens övergripande tillstånd: antingen väljer spelaren svårighetsgrad eller spelar
enum class AppState { SelectDifficulty, Playing };

int main() {
    const int MENU_W = 430;
    const int MENU_H = 290;

    InitWindow(MENU_W, MENU_H, "Minesweeper");
    SetTargetFPS(60);

    AppState app_state = AppState::SelectDifficulty;
    GuiRenderer renderer;
    // std::optional används eftersom inget spel finns förrän spelaren valt svårighetsgrad
    std::optional<Game> game;

    while (!WindowShouldClose()) {

        if (app_state == AppState::SelectDifficulty) {
            Difficulty chosen = Difficulty::Beginner;
            bool selected = false;

            if (IsKeyPressed(KEY_ONE)   || IsKeyPressed(KEY_KP_1)) { chosen = Difficulty::Beginner;     selected = true; }
            if (IsKeyPressed(KEY_TWO)   || IsKeyPressed(KEY_KP_2)) { chosen = Difficulty::Intermediate; selected = true; }
            if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) { chosen = Difficulty::Expert;       selected = true; }

            if (selected) {
                game.emplace(chosen);
                // Anpassa fönstrets storlek till det valda brädet innan spelläget aktiveras
                SetWindowSize(renderer.window_width(game->board()),
                              renderer.window_height(game->board()));
                app_state = AppState::Playing;
            }

            BeginDrawing();
            ClearBackground({30, 30, 30, 255});

            const Color HL = {70,  130, 180, 255};
            const Color TX = {220, 220, 220, 255};
            const Color DM = {140, 140, 140, 255};

            const char* title = "MINESWEEPER";
            // Centrera titeln horisontellt med MeasureText
            DrawText(title, MENU_W/2 - MeasureText(title, 28)/2, 35, 28, HL);

            DrawText("1  -  Nybörjare       9 x 9,   10 minor", 30, 110, 17, TX);
            DrawText("2  -  Mellannivå    16 x 16,  40 minor",  30, 148, 17, TX);
            DrawText("3  -  Expert        30 x 16,  99 minor",  30, 186, 17, TX);

            const char* hint = "Tryck 1, 2 eller 3 för att börja";
            DrawText(hint, MENU_W/2 - MeasureText(hint, 15)/2, 245, 15, DM);

            EndDrawing();

        } else {
            if (IsKeyPressed(KEY_Q))      break;
            // Escape tar tillbaka spelaren till menyn och återställer fönstrets ursprungsstorlek
            if (IsKeyPressed(KEY_ESCAPE)) {
                SetWindowSize(MENU_W, MENU_H);
                app_state = AppState::SelectDifficulty;
                continue;
            }
            if (IsKeyPressed(KEY_R)) game->reset();

            // Musklick hanteras bara när spelet faktiskt pågår
            GameState gs = game->state();
            if (gs == GameState::Playing || gs == GameState::WaitingFirstClick) {
                // Räkna om muspixlar till rad/kolumn med samma formel som GuiRenderer använder
                Vector2 mouse = GetMousePosition();
                int col = ((int)mouse.x - GuiRenderer::PADDING) / GuiRenderer::CELL_SIZE;
                int row = ((int)mouse.y - GuiRenderer::PADDING - GuiRenderer::HEADER_HEIGHT) / GuiRenderer::CELL_SIZE;
                bool in_board = row >= 0 && row < game->board().rows() &&
                                col >= 0 && col < game->board().cols();

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)  && in_board) game->reveal(row, col);
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && in_board) game->toggle_mark(row, col);
            }

            BeginDrawing();
            renderer.render(*game);
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
