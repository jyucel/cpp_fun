#include "controller/InputHandler.h"
#include <unistd.h>
#include <stdexcept>

InputHandler::InputHandler() {
    enable_raw_mode();
}

InputHandler::~InputHandler() {
    disable_raw_mode();
}

void InputHandler::enable_raw_mode() {
    // Spara originalinställningarna för att kunna återställa dem vid destruktion
    if (tcgetattr(STDIN_FILENO, &_original_termios) == -1)
        throw std::runtime_error("Failed to get terminal attributes");

    struct termios raw = _original_termios;

    // Stäng av echo och canonical mode — utan detta måste spelaren trycka Enter
    // efter varje tangent och tecknen syns i terminalen
    raw.c_lflag &= ~(ECHO | ICANON);

    // Läs ett tecken i taget, ingen timeout
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;

    // TCSAFLUSH väntar tills utdatabufferten är tömd innan inställningarna träder i kraft
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        throw std::runtime_error("Failed to set terminal attributes");
}

void InputHandler::disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_original_termios);
}

Action InputHandler::read() const {
    char c;
    if (::read(STDIN_FILENO, &c, 1) == -1)
        return Action::Unknown;

    // Piltangenter skickas som escape sequences: ESC [ A/B/C/D
    // Vi läser ytterligare två byte för att skilja på ESC-sekvenser och ett enkelt ESC-tryck
    if (c == '\033') {
        char seq[2];
        if (::read(STDIN_FILENO, &seq[0], 1) == -1) return Action::Unknown;
        if (::read(STDIN_FILENO, &seq[1], 1) == -1) return Action::Unknown;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return Action::MoveUp;
                case 'B': return Action::MoveDown;
                case 'C': return Action::MoveRight;
                case 'D': return Action::MoveLeft;
            }
        }
        return Action::Unknown;
    }

    // Både \r (CR) och \n (LF) accepteras för Enter — beroende på terminalkonfiguration
    switch (c) {
        case '\r':
        case '\n': return Action::Reveal;
        case 'f':
        case 'F':  return Action::ToggleMark;
        case 'r':
        case 'R':  return Action::Reset;
        case 'q':
        case 'Q':  return Action::Quit;
        default:   return Action::Unknown;
    }
}