#include "view/Renderer.h"
#include <iostream>

// ANSI escape codes för terminal-färgsättning.
// Formaten \033[48;5;Nm = bakgrundsfärg nr N, \033[38;5;Nm = textfärg nr N (256-färgspaletten).
// \033[Xm använder de 16 standardfärgerna.
namespace Color {
    const char* RESET        = "\033[0m";
    const char* BG_DARK_GREY = "\033[48;5;235m";
    const char* BG_YELLOW    = "\033[48;5;226m";
    const char* BG_RED       = "\033[48;5;196m";
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
    // Markörpositionen får gul färg så att spelaren lätt ser var den befinner sig
    const char* block_color  = is_cursor ? Color::FG_YELLOW : Color::FG_CYAN;
    const char* bg_revealed  = is_cursor ? Color::BG_YELLOW : Color::BG_DARK_GREY;

    if (!cell.is_revealed) {
        // reveal_all är sant när spelet förloras — visa alla bomber och felaktiga flaggor
        if (reveal_all && cell.is_mine) {
            std::cout << Color::BG_RED << Color::FG_WHITE << "  *  " << Color::RESET;
            return;
        }
        if (reveal_all && cell.mark == MarkState::Flagged && !cell.is_mine) {
            // Felaktigt flaggad ruta — X markerar att spelaren hade fel
            std::cout << block_color << "█" << Color::FG_RED << " X " << block_color << "█" << Color::RESET;
            return;
        }
        switch (cell.mark) {
            case MarkState::None:
                std::cout << block_color << "█████" << Color::RESET;
                break;
            case MarkState::Flagged:
                std::cout << block_color << "█" << Color::FG_GREEN << " F " << block_color << "█" << Color::RESET;
                break;
            case MarkState::Questioned:
                std::cout << block_color << "█" << Color::FG_CYAN << " ? " << block_color << "█" << Color::RESET;
                break;
        }
        return;
    }

    if (cell.is_mine) {
        std::cout << Color::BG_RED << Color::FG_WHITE << "  *  " << Color::RESET;
        return;
    }

    if (cell.adjacent_mines == 0) {
        std::cout << bg_revealed << "     " << Color::RESET;
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
        case 7: fg = Color::FG_BLACK;     break;
        case 8: fg = Color::FG_WHITE;     break;
        default: fg = Color::RESET;       break;
    }
    std::cout << bg_revealed << fg << "  " << cell.adjacent_mines << "  " << Color::RESET;
}

void Renderer::render_status(const Game& game) const {
    std::cout << "\n";
    std::cout << " Minor kvar: " << game.remaining_mines();

    switch (game.state()) {
        case GameState::WaitingFirstClick:
            std::cout << "  |  Flytta med piltangenter, Enter for att oppna";
            break;
        case GameState::Playing:
            std::cout << "  |  Enter: oppna   F: flagga   R: reset   Q: avsluta";
            break;
        case GameState::Won:
            std::cout << "  |  Du vann! Tryck R for att spela igen";
            break;
        case GameState::Lost:
            std::cout << "  |  Game over! Tryck R for att spela igen";
            break;
    }
    std::cout << "\n";
}

void Renderer::render(const Game& game) const {
    clear_screen();

    const Board& board = game.board();
    const int cols = board.cols();

    // Översta raden
    std::cout << " ┌";
    for (int c = 0; c < cols; c++) {
        std::cout << "─────";
        if (c < cols - 1) std::cout << "┬";
    }
    std::cout << "┐\n";

    // Avslöja alla rutor om spelet är förlorat, så att spelaren ser vad som gömde sig
    bool reveal_all = (game.state() == GameState::Lost);
    for (int r = 0; r < board.rows(); r++) {
        // Cellrad
        std::cout << " │";
        for (int c = 0; c < cols; c++) {
            bool is_cursor = (r == game.cursor_row() && c == game.cursor_col());
            render_cell(board.at(r, c), is_cursor, reveal_all);
            std::cout << "│";
        }
        std::cout << "\n";

        // Separator eller understa raden
        if (r < board.rows() - 1) {
            std::cout << " ├";
            for (int c = 0; c < cols; c++) {
                std::cout << "─────";
                if (c < cols - 1) std::cout << "┼";
            }
            std::cout << "┤\n";
        }
    }

    // Understa raden
    std::cout << " └";
    for (int c = 0; c < cols; c++) {
        std::cout << "─────";
        if (c < cols - 1) std::cout << "┴";
    }
    std::cout << "┘\n";

    render_status(game);
    // Tvinga ut buffertens innehåll direkt — utan detta kan skärmen dröja med att uppdateras
    std::cout << std::flush;
}