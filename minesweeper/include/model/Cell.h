#pragma once

enum class MarkState {
    None,
    Flagged,
    Questioned
};

struct Cell {
    bool      is_mine       = false;
    bool      is_revealed   = false;
    int       adjacent_mines = 0;
    MarkState mark          = MarkState::None;
};

