#pragma once

#include "model/Game.h"

class Renderer {
private:
    void render_cell(const Cell& cell, bool is_cursor, bool reveal_all) const;
    void render_status(const Game& game) const;
    void clear_screen() const;

public:
    void render(const Game& game) const;
};