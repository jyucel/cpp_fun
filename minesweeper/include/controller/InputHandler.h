#pragma once

#include "Action.h"
#include <termios.h>

// Hanterar terminalinmatning i raw mode för att kunna läsa enstaka tangenttryckningar
// utan att behöva trycka Enter. Destruktorn återställer terminalen till sitt ursprungsläge.
class InputHandler {
private:
    // Sparas vid konstruktion så att terminalen kan återställas vid destruktion
    struct termios _original_termios;

    void enable_raw_mode();
    void disable_raw_mode();

public:
    InputHandler();
    ~InputHandler();

    Action read() const;
};