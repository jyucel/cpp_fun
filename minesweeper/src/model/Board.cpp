#include <algorithm>
#include <random>
#include <stdexcept>

#include "model/Board.h"
#include "model/Difficulty.h"

Board::Board(int rows, int cols, int mines)
    : _rows(rows), _cols(cols), _mines(mines)
{
    // Kontroll: Om det är lika många minor som rutor, finns det inga säkra rutor kvar.
    if (mines >= rows * cols)
        throw std::invalid_argument("Too many mines for board size");

    _grid.assign(rows, std::vector<Cell>(cols));
}

Board Board::create(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Beginner:     return Board(9,  9,  10);
        case Difficulty::Intermediate: return Board(16, 16, 40);
        case Difficulty::Expert:       return Board(30, 16, 99);
        default: return Board(9, 9, 10);
    }
}

bool Board::in_bounds(int row, int col) const {
    return row >= 0 && row < _rows && col >= 0 && col < _cols;
}

void Board::place_mines(int first_row, int first_col) {
    const int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};

    // Den ruta du klickar på först, och alla rutor bredvid, får aldrig en mina.
    // Då kan du alltid öppna ett säkert område på första draget.
    std::vector<std::pair<int,int>> excluded;
    excluded.push_back({first_row, first_col});
    for (int d = 0; d < 8; d++) {
        int nr = first_row + dr[d];
        int nc = first_col + dc[d];
        if (in_bounds(nr, nc))
            excluded.push_back({nr, nc});
    }

    // Bygg en lista över alla tillåtna positioner
    std::vector<std::pair<int,int>> candidates;
    candidates.reserve(_rows * _cols);

    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            auto pos = std::make_pair(r, c);
            if (std::find(excluded.begin(), excluded.end(), pos) == excluded.end())
                candidates.push_back(pos);
        }
    }

    // Slumpa om kandidatpositionerna och ta de första _mines som minplatser
    std::mt19937 rng{std::random_device{}()};
    std::shuffle(candidates.begin(), candidates.end(), rng);

    for (int i = 0; i < _mines; i++)
        _grid[candidates[i].first][candidates[i].second].is_mine = true;

    calculate_adjacent();
    _mines_placed = true;
}
void Board::calculate_adjacent() {
    const int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};

    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            if (_grid[r][c].is_mine) continue;

            int count = 0;
            for (int d = 0; d < 8; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (in_bounds(nr, nc) && _grid[nr][nc].is_mine)
                    count++;
            }
            _grid[r][c].adjacent_mines = count;
        }
    }
}
void Board::flood_fill(int row, int col) {
    if (!in_bounds(row, col)) return;

    Cell& cell = _grid[row][col];
    if (cell.is_revealed || cell.is_mine) return;
    // Rutor med flagga öppnas inte automatiskt. Spelaren har markerat dem med flit.
    if (cell.mark != MarkState::None) return;

    cell.is_revealed = true;

    // Fortsätt bara öppna rutor om det inte finns någon mina bredvid.
    // Om det finns minor runt rutan, stannar öppningen där.
    if (cell.adjacent_mines == 0) {
        const int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
        const int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};
        for (int d = 0; d < 8; d++)
            flood_fill(row + dr[d], col + dc[d]);
    }
}

void Board::reveal(int row, int col) {
    if (!in_bounds(row, col)) return;

    Cell& cell = _grid[row][col];
    if (cell.is_revealed) return;
    // Vänsterklick på en flaggad ruta tar bort flaggan — ett extra klick krävs för att avslöja.
    if (cell.mark == MarkState::Flagged) {
        cell.mark = MarkState::None;
        return;
    }

    // När du öppnar en ruta för första gången placeras minorna. Den rutan är alltid säker.
    if (!_mines_placed)
        place_mines(row, col);

    if (cell.is_mine) {
        // Om du öppnar en ruta med en mina avslöjas den direkt. Spelet ser detta och du förlorar.
        cell.is_revealed = true;
        return;
    }

    flood_fill(row, col);
}

void Board::toggle_mark(int row, int col) {
    if (!in_bounds(row, col)) return;

    Cell& cell = _grid[row][col];
    if (cell.is_revealed) return;

    switch (cell.mark) {
        case MarkState::None:       cell.mark = MarkState::Flagged;    break;
        case MarkState::Flagged:    cell.mark = MarkState::Questioned; break;
        case MarkState::Questioned: cell.mark = MarkState::None;       break;
    }
}

const Cell& Board::at(int row, int col) const {
    return _grid.at(row).at(col);
}

int Board::rows()         const { return _rows; }
int Board::cols()         const { return _cols; }
int Board::mines()        const { return _mines; }
bool Board::mines_placed() const { return _mines_placed; }

bool Board::all_revealed() const {
    // Minor räknas inte — de behöver aldrig avslöjas för att vinna
    for (int r = 0; r < _rows; r++)
        for (int c = 0; c < _cols; c++)
            if (!_grid[r][c].is_mine && !_grid[r][c].is_revealed)
                return false;
    return true;
}