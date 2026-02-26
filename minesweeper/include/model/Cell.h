#pragma once

// En ruta kan markeras av spelaren.
// Du kan sätta en flagga om du tror att det är en mina där,
// eller ett frågetecken om du är osäker.
enum class MarkState {
    None,
    Flagged,
    Questioned
};

// Beskriver en ruta på spelplanen.
// adjacent_mines visar hur många minor som finns runt rutan.
// Detta räknas ut när minorna placeras och ändras inte senare.
struct Cell {
    bool      is_mine       = false;
    bool      is_revealed   = false;
    int       adjacent_mines = 0;
    MarkState mark          = MarkState::None;
};

