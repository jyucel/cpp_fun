#include "view/Renderer.h"
#include <iostream>
#include <string>

// ANSI escape codes för terminal-färgsättning.
// Formaten \033[48;5;Nm = bakgrundsfärg nr N, \033[38;5;Nm = textfärg nr N (256-färgspaletten).
// \033[Xm använder de 16 standardfärgerna.
namespace Color {
    const char* RESET        = "\033[0m";
    const char* BG_DARK_GREY = "\033[48;5;235m";
    const char* BG_YELLOW    = "\033[48;5;226m";
    const char* BG_RED       = "\033[48;5;196m";
    const char* BG_CELL      = "\033[46m";           // cyan bg för oupptäckta rutor
    const char* BG_FLAG      = "\033[48;5;202m";     // orange bg för flaggade rutor
    const char* BG_QUEST     = "\033[48;5;220m";     // gul bg för frågemark-rutor
    const char* FG_GREY      = "\033[38;5;240m";
    const char* FG_YELLOW    = "\033[38;5;226m";
    const char* FG_BLUE      = "\033[34m";
    const char* FG_GREEN     = "\033[32m";
    const char* FG_RED       = "\033[31m";
    const char* FG_DARK_BLUE = "\033[34;1m";
    const char* FG_DARK_RED  = "\033[31;1m";
    const char* FG_CYAN      = "\033[36m";
    const char* FG_BLACK     = "\033[30m";
    const char* FG_WHITE     = "\033[37m";
}

void Renderer::clear_screen() const {
    // \033[2J rensar skärmen, \033[H flyttar markören till övre vänstra hörnet
    std::cout << "\033[2J\033[H";
}

void Renderer::render_cell(const Cell& cell, bool is_cursor, bool reveal_all) const {
    // Fullbreddstecken (2 kolumner/tecken) kompenserar terminalens 2:1 höjd/bredd-ratio → kvadratiska celler
    // Markören markeras med inverterade färger (reverse video)
    const char* CUR   = is_cursor ? "\033[7m" : "";
    // Fullbreddssiffror U+FF11–FF18, index 1–8
    static const char* FW_DIGIT[] = { "", "１","２","３","４","５","６","７","８" };

    if (!cell.is_revealed) {
        if (reveal_all && cell.is_mine) {
            std::cout << CUR << Color::FG_RED << "＊" << Color::RESET;
            return;
        }
        if (reveal_all && cell.mark == MarkState::Flagged && !cell.is_mine) {
            std::cout << CUR << Color::FG_RED << "Ｘ" << Color::RESET;
            return;
        }
        switch (cell.mark) {
            case MarkState::None:
                std::cout << CUR << Color::BG_CELL << Color::FG_WHITE << "＃" << Color::RESET;
                break;
            case MarkState::Flagged:
                std::cout << CUR << Color::BG_FLAG << Color::FG_WHITE << "Ｆ" << Color::RESET;
                break;
            case MarkState::Questioned:
                std::cout << CUR << Color::BG_QUEST << Color::FG_BLACK << "？" << Color::RESET;
                break;
        }
        return;
    }

    if (cell.is_mine) {
        std::cout << CUR << Color::FG_RED << "＊" << Color::RESET;
        return;
    }

    if (cell.adjacent_mines == 0) {
        std::cout << CUR << "  " << Color::RESET;   // 2 spaces = 2 kolumner
        return;
    }

    // Sifferfärger följer klassisk Minesweeper-konvention: 1=blå, 2=grön, 3=röd, osv.
    const char* fg;
    switch (cell.adjacent_mines) {
        case 1: fg = Color::FG_BLUE;      break;
        case 2: fg = Color::FG_GREEN;     break;
        case 3: fg = Color::FG_RED;       break;
        case 4: fg = Color::FG_DARK_BLUE; break;
        case 5: fg = Color::FG_DARK_RED;  break;
        case 6: fg = Color::FG_CYAN;      break;
        case 7: fg = Color::FG_WHITE;     break;
        case 8: fg = Color::FG_GREY;      break;
        default: fg = Color::RESET;       break;
    }
    std::cout << CUR << fg << FW_DIGIT[cell.adjacent_mines] << Color::RESET;
}

void Renderer::render_status(const Game& game) const {
    switch (game.state()) {
        case GameState::WaitingFirstClick:
        case GameState::Playing:
            std::cout << "\n Enter: öppna  F: flagga  R: reset  Q: avsluta\n";
            break;
        case GameState::Won:
        case GameState::Lost:
            std::cout << "\n R: spela igen  Q: avsluta\n";
            break;
    }
}

void Renderer::render(const Game& game) const {
    clear_screen();

    const Board& board = game.board();
    const int cols = board.cols();
    // Inre bredd: 2 kolumner/cell (fullbredd) + 1 separator mellan celler
    const int inner = 3 * cols - 1;

    // Rubrik — titel och spelstatus i en ram som sitter ihop med brädet
    std::cout << " ┌";
    for (int i = 0; i < inner; i++) std::cout << "─";
    std::cout << "┐\n";

    std::string left = " MINES";
    std::string right;
    switch (game.state()) {
        case GameState::Won:  right = "Vann! "; break;
        case GameState::Lost: right = "Over! "; break;
        default: right = "Kvar:" + std::to_string(game.remaining_mines()) + " "; break;
    }
    int gap = inner - (int)left.size() - (int)right.size();
    std::cout << " │" << left;
    for (int i = 0; i < (gap > 0 ? gap : 1); i++) std::cout << " ";
    std::cout << right << "│\n";

    // Rubrikens nederkant kopplar till brädet
    std::cout << " ├";
    for (int i = 0; i < inner; i++) std::cout << "─";
    std::cout << "┤\n";

    // Brädet — │ mellan kolumner, tunn grå separator mellan rader
    bool reveal_all = (game.state() == GameState::Lost);
    for (int r = 0; r < board.rows(); r++) {
        std::cout << " │";
        for (int c = 0; c < cols; c++) {
            bool is_cursor = (r == game.cursor_row() && c == game.cursor_col());
            render_cell(board.at(r, c), is_cursor, reveal_all);
            if (c < cols - 1) std::cout << "│";
        }
        std::cout << "│\n";

        if (r < board.rows() - 1) {
            std::cout << Color::FG_GREY << " ├";
            for (int c = 0; c < cols; c++) {
                std::cout << "──";
                if (c < cols - 1) std::cout << "┼";
            }
            std::cout << "┤" << Color::RESET << "\n";
        }
    }

    // Brädets underkant
    std::cout << " └";
    for (int i = 0; i < inner; i++) std::cout << "─";
    std::cout << "┘\n";

    render_status(game);
    // Tvinga ut buffertens innehåll direkt — utan detta kan skärmen dröja med att uppdateras
    std::cout << std::flush;
}