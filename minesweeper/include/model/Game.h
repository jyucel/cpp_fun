#pragma once

#include "model/Board.h"
#include "model/GameState.h"
#include "model/Difficulty.h"

// Spelets övergripande logik: knyter ihop brädet med spelstatus och markörposition.
// Ansvarar för att övergå mellan tillstånd (t.ex. WaitingFirstClick -> Playing -> Lost/Won).
class Game {
private:
    Board _board;
    GameState _state;
    // Svårighetsgraden sparas så att reset() kan skapa ett nytt bräde av samma typ
    Difficulty _difficulty;
    // Räknar aktiva flaggor för att visa kvarvarande minor i gränssnittet
    int _flagged_count;
    int _cursor_row;
    int _cursor_col;

public:
    Game(Difficulty difficulty);

    void reveal(int row, int col);
    void toggle_mark(int row, int col);

    // Markörnavigering för terminalläget
    void move_cursor(int row, int col);
    void reveal_at_cursor();
    void toggle_mark_at_cursor();

    const Board& board() const;
    GameState state() const;
    Difficulty difficulty() const;
    int flagged_count() const;
    // Totalt antal minor minus antalet flaggor — kan bli negativt om spelaren flaggar för mycket
    int remaining_mines() const;
    int cursor_row() const;
    int cursor_col() const;

    // Återställer brädets och spelets tillstånd utan att byta svårighetsgrad
    void reset();
};