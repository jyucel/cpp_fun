#pragma once

#include "model/Board.h"
#include "model/GameState.h"
#include "model/Difficulty.h"

class Game {
private:
    Board _board;
    GameState _state;
    Difficulty _difficulty;
    int _flagged_count;
    int _cursor_row;
    int _cursor_col;

public:
    Game(Difficulty difficulty);

    void reveal(int row, int col);
    void toggle_mark(int row, int col);

    void move_cursor(int row, int col);
    void reveal_at_cursor();
    void toggle_mark_at_cursor();

    const Board& board() const;
    GameState state() const;
    Difficulty difficulty() const;
    int flagged_count() const;
    int remaining_mines() const;
    int cursor_row() const;
    int cursor_col() const;

    void reset();
};