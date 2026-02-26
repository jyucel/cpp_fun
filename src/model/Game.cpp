#include "model/Game.h"

Game::Game(Difficulty difficulty)
    : _board(Board::create(difficulty))
    , _state(GameState::WaitingFirstClick)
    , _difficulty(difficulty)
    , _flagged_count(0)
    , _cursor_row(0)
    , _cursor_col(0)
{
}

void Game::reveal(int row, int col) {
    if (_state != GameState::WaitingFirstClick && _state != GameState::Playing)
        return;

    _board.reveal(row, col);

    if (_state == GameState::WaitingFirstClick)
        _state = GameState::Playing;

    const Cell& cell = _board.at(row, col);
    if (cell.is_revealed && cell.is_mine) {
        _state = GameState::Lost;
        return;
    }

    if (_board.all_revealed())
        _state = GameState::Won;
}

void Game::toggle_mark(int row, int col) {
    if (_state != GameState::Playing && _state != GameState::WaitingFirstClick)
        return;

    MarkState before = _board.at(row, col).mark;
    _board.toggle_mark(row, col);
    MarkState after = _board.at(row, col).mark;

    if (before != MarkState::Flagged && after == MarkState::Flagged)
        _flagged_count++;
    else if (before == MarkState::Flagged && after != MarkState::Flagged)
        _flagged_count--;
}

void Game::move_cursor(int row, int col) {
    if (row >= 0 && row < _board.rows() &&
        col >= 0 && col < _board.cols())
    {
        _cursor_row = row;
        _cursor_col = col;
    }
}

void Game::reveal_at_cursor() {
    reveal(_cursor_row, _cursor_col);
}

void Game::toggle_mark_at_cursor() {
    toggle_mark(_cursor_row, _cursor_col);
}

void Game::reset() {
    _board = Board::create(_difficulty);
    _state = GameState::WaitingFirstClick;
    _flagged_count = 0;
    _cursor_row = 0;
    _cursor_col = 0;
}

const Board& Game::board()           const { return _board; }
GameState    Game::state()           const { return _state; }
Difficulty   Game::difficulty()      const { return _difficulty; }
int          Game::flagged_count()   const { return _flagged_count; }
int          Game::remaining_mines() const { return _board.mines() - _flagged_count; }
int          Game::cursor_row()      const { return _cursor_row; }
int          Game::cursor_col()      const { return _cursor_col; }