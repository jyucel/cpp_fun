

#pragma once

#include <string>
#include <vector>

namespace MenuUtils {
    // Textfärg — kommentera in önskad:
    // inline std::string THEME_COLOR = "\033[32m";              // Grön
    // inline std::string THEME_COLOR = "\033[33m";              // Gul
    // inline std::string THEME_COLOR = "\033[36m";              // Cyan
    inline std::string THEME_COLOR = "\033[38;2;255;160;0m";    // Amber (CRT)

    inline std::string RESET = "\033[0m";

    // Rensar terminalen.
    void clearScreen();

    // Ritar en menybox med titel, underrubrik och alternativ.
    void drawMenu(const std::string& title,
                  const std::string& subtitle,
                  const std::vector<std::string>& options);

    // Läser ett heltal från användaren med felhantering.
    int readInt();

    // Läser en rad från användaren.
    std::string readLine();
}
