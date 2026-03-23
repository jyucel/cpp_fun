

#include "MenuUtils.hpp"
#include <iostream>

namespace MenuUtils {

    void clearScreen() {
        system("clear");
    }

    // Hjälpfunktion för att räkna synliga tecken (Unicode-aware, enkel version)
    auto visibleLength = [](const std::string& s) {
        size_t count = 0;
        for (size_t i = 0; i < s.size(); ) {
            unsigned char c = s[i];
            if ((c & 0x80) == 0) { // ASCII
                ++i;
            } else if ((c & 0xE0) == 0xC0) { // 2-byte UTF-8
                i += 2;
            } else if ((c & 0xF0) == 0xE0) { // 3-byte UTF-8
                i += 3;
            } else if ((c & 0xF8) == 0xF0) { // 4-byte UTF-8
                i += 4;
            } else {
                ++i; // fallback
            }
            ++count;
        }
        return count;
    };

    void drawMenu(const std::string& title,
                  const std::string& subtitle,
                  const std::vector<std::string>& options) {
        int width = 34;
        std::string top    = "╔" + std::string(width, '=') + "╗";
        std::string mid    = "╠" + std::string(width, '=') + "╣";
        std::string bottom = "╚" + std::string(width, '=') + "╝";

        // Lambda-funktion för att centrera och formatera text i menyn
        auto padRow = [&](const std::string& text) {
            size_t len = visibleLength(text);
            int padding = width - static_cast<int>(len);
            if (padding < 2) padding = 2;
            return "║  " + text + std::string(padding - 2, ' ') + "║";
        };

        clearScreen();
        std::cout << THEME_COLOR;
        std::cout << top << std::endl;
        std::cout << padRow(title) << std::endl;
        if (!subtitle.empty()) {
            std::cout << padRow(subtitle) << std::endl;
        }
        std::cout << mid << std::endl;
        for (const auto& option : options) {
            std::cout << padRow(option) << std::endl;
        }
        std::cout << bottom << std::endl;
        std::cout << RESET;
        std::cout << "> ";
    }

    std::string readLine() {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    int readInt() {
        while (true) {
            std::string line = readLine();
            try {
                return std::stoi(line);
            } catch (...) {
                std::cout << "Ogiltigt val, försök igen: ";
            }
        }
    }

}
