#pragma once

#include "model/Game.h"

// Grafisk renderare byggd med raylib. Fönstrets storlek beror på brädesstorlek,
// så main.cpp måste anropa window_width/window_height för att sätta rätt fönsterstorlek
// innan spelstart.
class GuiRenderer {
public:
    // Varje cell är CELL_SIZE x CELL_SIZE pixlar; PADDING finns runt hela brädet
    static constexpr int CELL_SIZE     = 36;
    static constexpr int PADDING       = 8;
    static constexpr int HEADER_HEIGHT = 50;

    int window_width(const Board& board) const;
    int window_height(const Board& board) const;

    void render(const Game& game) const;

private:
    void render_header(const Game& game) const;
    void render_board(const Game& game) const;
    // x, y är pixelkoordinater för cellens övre vänstra hörn
    void render_cell(const Cell& cell, int x, int y, bool is_hover, bool reveal_all) const;
};
