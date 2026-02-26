#pragma once

#include "model/Game.h"

class GuiRenderer {
public:
    static constexpr int CELL_SIZE     = 36;
    static constexpr int PADDING       = 8;
    static constexpr int HEADER_HEIGHT = 50;

    int window_width(const Board& board) const;
    int window_height(const Board& board) const;

    void render(const Game& game) const;

private:
    void render_header(const Game& game) const;
    void render_board(const Game& game) const;
    void render_cell(const Cell& cell, int x, int y, bool is_hover, bool reveal_all) const;
};
