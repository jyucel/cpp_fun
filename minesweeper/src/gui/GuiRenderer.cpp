#include "gui/GuiRenderer.h"
#include "model/GameState.h"
#include "raylib.h"
#include <string>

// Färgpaletten för det grafiska gränssnittet, samlad i ett namespace för tydlighet
namespace C {
    const Color BG           = {30,  30,  30,  255};
    const Color HEADER_BG    = {20,  20,  20,  255};
    const Color HIDDEN       = {70,  130, 180, 255};
    // Ljusare blå när musen hovrar över en ruta
    const Color HIDDEN_HOVER = {95,  155, 205, 255};
    // Ljus och mörk kant skapar ett upphöjt 3D-utseende på oupptäckta celler
    const Color HIDDEN_LIGHT = {115, 175, 225, 255};
    const Color HIDDEN_DARK  = {45,  90,  130, 255};
    const Color REVEALED     = {185, 185, 185, 255};
    const Color REVEALED_EDGE= {130, 130, 130, 255};
    const Color MINE_BG      = {200, 50,  50,  255};
    const Color FLAG         = {255, 150, 0,   255};
    const Color QUESTION     = {200, 210, 50,  255};
    const Color TEXT         = {225, 225, 225, 255};
}

static const Color NUM_COLORS[9] = {
    {255, 255, 255, 255},   // 0 (används ej)
    {60,  100, 255, 255},   // 1 blå
    {40,  160, 40,  255},   // 2 grön
    {210, 40,  40,  255},   // 3 röd
    {0,   0,   160, 255},   // 4 mörkblå
    {160, 0,   0,   255},   // 5 mörkröd
    {0,   160, 160, 255},   // 6 teal
    {40,  40,  40,  255},   // 7 nästan svart
    {110, 110, 110, 255},   // 8 grå
};

int GuiRenderer::window_width(const Board& board) const {
    return PADDING * 2 + board.cols() * CELL_SIZE;
}

int GuiRenderer::window_height(const Board& board) const {
    return PADDING + HEADER_HEIGHT + board.rows() * CELL_SIZE + PADDING;
}

void GuiRenderer::render(const Game& game) const {
    ClearBackground(C::BG);
    render_header(game);
    render_board(game);
}

void GuiRenderer::render_header(const Game& game) const {
    DrawRectangle(0, 0, GetScreenWidth(), PADDING + HEADER_HEIGHT, C::HEADER_BG);

    std::string mines = std::to_string(game.remaining_mines()) + " minor kvar";
    DrawText(mines.c_str(), PADDING + 4, PADDING + 14, 20, C::TEXT);

    const char* status;
    Color sc = C::TEXT;
    switch (game.state()) {
        case GameState::Won:
            status = "Du vann!  [R] spela igen";
            sc = {80, 220, 80, 255};
            break;
        case GameState::Lost:
            status = "Game over!  [R] spela igen";
            sc = {220, 80, 80, 255};
            break;
        default:
            status = "[R] reset   [Q] avsluta";
            break;
    }
    int tw = MeasureText(status, 18);
    DrawText(status, GetScreenWidth() - tw - PADDING - 4, PADDING + 15, 18, sc);
}

void GuiRenderer::render_board(const Game& game) const {
    const Board& board = game.board();

    // Räkna ut vilken cell musen befinner sig över med hjälp av pixelkoordinater
    Vector2 mouse = GetMousePosition();
    int hover_col = ((int)mouse.x - PADDING) / CELL_SIZE;
    int hover_row = ((int)mouse.y - PADDING - HEADER_HEIGHT) / CELL_SIZE;

    bool reveal_all = (game.state() == GameState::Lost);
    for (int r = 0; r < board.rows(); r++) {
        for (int c = 0; c < board.cols(); c++) {
            int x = PADDING + c * CELL_SIZE;
            int y = PADDING + HEADER_HEIGHT + r * CELL_SIZE;
            bool is_hover = (r == hover_row && c == hover_col);
            render_cell(board.at(r, c), x, y, is_hover, reveal_all);
        }
    }
}

void GuiRenderer::render_cell(const Cell& cell, int x, int y, bool is_hover, bool reveal_all) const {
    const int S = CELL_SIZE;
    const int b = 3;  // kantbredd i pixlar för det upphöjda 3D-utseendet

    if (!cell.is_revealed) {
        // Visa oupptäckt bomb
        if (reveal_all && cell.is_mine) {
            DrawRectangle(x, y, S, S, C::MINE_BG);
            int mw = MeasureText("*", 22);
            DrawText("*", x + (S - mw)/2, y + (S - 22)/2, 22, WHITE);
            return;
        }
        // Visa felaktig flagga
        if (reveal_all && cell.mark == MarkState::Flagged && !cell.is_mine) {
            Color wrong_bg = {160, 50, 50, 255};
            DrawRectangle(x, y, S, S, wrong_bg);
            int xw = MeasureText("X", 20);
            DrawText("X", x + (S - xw)/2, y + (S - 20)/2, 20, WHITE);
            return;
        }

        // Rita cellens kropp med upphöjda kanter: ljus överdel/vänsterkant + mörk nederdel/högerkant
        Color base = is_hover ? C::HIDDEN_HOVER : C::HIDDEN;
        DrawRectangle(x + b, y + b, S - b*2, S - b*2, base);
        DrawRectangle(x,     y,     S,       b,       C::HIDDEN_LIGHT);  // top
        DrawRectangle(x,     y + b, b,       S - b,   C::HIDDEN_LIGHT);  // left
        DrawRectangle(x + b, y+S-b, S - b,   b,       C::HIDDEN_DARK);   // bottom
        DrawRectangle(x+S-b, y + b, b,       S - b,   C::HIDDEN_DARK);   // right

        // Centrera textens symbol på cellen med MeasureText för horisontell justering
        if (cell.mark == MarkState::Flagged) {
            const int fs = 24;
            int fw = MeasureText("F", fs);
            int tx = x + (S - fw)/2, ty = y + (S - fs)/2;
            DrawText("F", tx + 1, ty,     fs, C::FLAG);
            DrawText("F", tx,     ty + 1, fs, C::FLAG);
            DrawText("F", tx,     ty,     fs, C::FLAG);
        } else if (cell.mark == MarkState::Questioned) {
            const int fs = 24;
            int qw = MeasureText("?", fs);
            int tx = x + (S - qw)/2, ty = y + (S - fs)/2;
            DrawText("?", tx + 1, ty,     fs, C::QUESTION);
            DrawText("?", tx,     ty + 1, fs, C::QUESTION);
            DrawText("?", tx,     ty,     fs, C::QUESTION);
        }

    } else if (cell.is_mine) {
        DrawRectangle(x, y, S, S, C::MINE_BG);
        int mw = MeasureText("*", 22);
        DrawText("*", x + (S - mw)/2, y + (S - 22)/2, 22, WHITE);

    } else {
        // Avslöjd säker ruta: tunn mörkare ram runt en ljusare yta ger ett nedsänkt utseende
        DrawRectangle(x, y, S, S, C::REVEALED_EDGE);
        DrawRectangle(x + 1, y + 1, S - 2, S - 2, C::REVEALED);

        if (cell.adjacent_mines > 0) {
            std::string num = std::to_string(cell.adjacent_mines);
            Color nc = NUM_COLORS[cell.adjacent_mines];
            const int fs = 24;
            int nw = MeasureText(num.c_str(), fs);
            int tx = x + (S - nw)/2;
            int ty = y + (S - fs)/2;
            // Rita tre gånger med förskjutning i x och y för kraftigare fetstilseffekt
            DrawText(num.c_str(), tx + 1, ty,     fs, nc);
            DrawText(num.c_str(), tx,     ty + 1, fs, nc);
            DrawText(num.c_str(), tx,     ty,     fs, nc);
        }
    }
}
