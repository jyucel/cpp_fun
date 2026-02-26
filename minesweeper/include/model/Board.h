#pragma once

#include "model/Cell.h"
#include "model/Difficulty.h"
#include <vector>

// Spelplanen för spelet. Här sparas alla rutor och minor.
// Minorna placeras först när du klickar första gången. Då kan du aldrig förlora direkt.
class Board {
private:
    int _rows;
    int _cols;
    int _mines;
    // Blir true efter att place_mines() har körts, alltså efter första draget.
    bool _mines_placed = false;

    std::vector<std::vector<Cell>> _grid;

    void calculate_adjacent();
    // En funktion som automatiskt öppnar alla tomma rutor som ligger bredvid varandra.
    void flood_fill(int row, int col);
    bool in_bounds(int row, int col) const;

public:

    // Skapar en spelplan med rätt storlek och antal minor beroende på vilken svårighetsgrad du valt.
    static Board create(Difficulty difficulty);

    Board(int rows, int cols, int mines);

    // Körs när du avslöjar en ruta för första gången. Den rutan får aldrig en mina.
    void place_mines(int first_row, int first_col);
    void reveal(int row, int col);
    // Byter markering på en ruta: ingen markering → flagga → frågetecken → ingen markering.
    void toggle_mark(int row, int col);

    const Cell& at(int row, int col) const;
    int rows() const;
    int cols() const;
    int mines() const;
    bool mines_placed() const;
    // Returnerar true om alla rutor utan minor är öppnade. Då har du vunnit spelet.
    bool all_revealed() const;
};