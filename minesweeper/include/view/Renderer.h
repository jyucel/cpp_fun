#pragma once

#include "model/Game.h"

// Terminalbaserad renderare som ritar brädet med ANSI-färgkoder och Unicode-ramar.
// Ritar om hela skärmen vid varje anrop till render() för enkelhetens skull.
class Renderer {
private:
    // reveal_all används när spelet är förlorat — visar alla bomber och felaktiga flaggor
    void render_cell(const Cell& cell, bool is_cursor, bool reveal_all) const;
    void render_status(const Game& game) const;
    void clear_screen() const;

public:
    void render(const Game& game) const;
};