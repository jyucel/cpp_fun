#include "model/Game.h"
#include "model/Difficulty.h"
#include "view/Renderer.h"
#include "controller/InputHandler.h"
#include <iostream>

// Visar en meny och väntar på att du väljer nivå. Alla andra tecken ignoreras.
Difficulty select_difficulty() {
    std::cout << "\033[2J\033[H";  // clear screen
    std::cout << "╔═════════════════════════════════════╗\n";
    std::cout << "║             MINESWEEPER             ║\n";
    std::cout << "╠═════════════════════════════════════╣\n";
    std::cout << "║  1.  Beginner        9x9, 10 minor  ║\n";
    std::cout << "║  2.  Intermediate  16x16, 40 minor  ║\n";
    std::cout << "║  3.  Expert        30x16, 99 minor  ║\n";
    std::cout << "╚═════════════════════════════════════╝\n";
    std::cout << "Välj nivå (1-3): ";

    char c;
    while (true) {
        std::cin.get(c);
        switch (c) {
            case '1': return Difficulty::Beginner;
            case '2': return Difficulty::Intermediate;
            case '3': return Difficulty::Expert;
            default: break;
        }
    }
}

int main() {
    Difficulty difficulty = select_difficulty();

    Game game(difficulty);
    Renderer renderer;
    // När InputHandler skapas aktiveras "raw mode" i terminalen. När den tas bort återställs terminalen.
    InputHandler input;

    // Rita ut spelplanen en gång innan spelet börjar.
    renderer.render(game);

    while (true) {
        Action action = input.read();

        switch (action) {
            case Action::MoveUp:
                game.move_cursor(game.cursor_row() - 1, game.cursor_col());
                break;
            case Action::MoveDown:
                game.move_cursor(game.cursor_row() + 1, game.cursor_col());
                break;
            case Action::MoveLeft:
                game.move_cursor(game.cursor_row(), game.cursor_col() - 1);
                break;
            case Action::MoveRight:
                game.move_cursor(game.cursor_row(), game.cursor_col() + 1);
                break;
            case Action::Reveal:
                game.reveal_at_cursor();
                break;
            case Action::ToggleMark:
                game.toggle_mark_at_cursor();
                break;
            case Action::Reset:
                game.reset();
                break;
            case Action::Quit:
                // Avsluta spelet direkt. Terminalen återställs automatiskt.
                return 0;
            case Action::Unknown:
                break;
        }

        // Rita ut spelplanen igen efter varje sak du gör, även om inget ändrades.
        renderer.render(game);
    }

    return 0;
}