#pragma once

#include "model/Cell.h"
#include "model/Difficulty.h"
#include <vector>

class Board {
private:
    int _rows;
    int _cols;
    int _mines;
    bool _mines_placed = false;

    std::vector<std::vector<Cell>> _grid;

    void calculate_adjacent();
    void flood_fill(int row, int col);
    bool in_bounds(int row, int col) const;

public:

    static Board create(Difficulty difficulty);
    
    Board(int rows, int cols, int mines);

    void place_mines(int first_row, int first_col);
    void reveal(int row, int col);
    void toggle_mark(int row, int col);

    const Cell& at(int row, int col) const;
    int rows() const;
    int cols() const;
    int mines() const;
    bool mines_placed() const;
    bool all_revealed() const;
};