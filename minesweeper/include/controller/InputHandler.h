#pragma once

#include "Action.h"
#include <termios.h>

class InputHandler {
private:
    struct termios _original_termios;

    void enable_raw_mode();
    void disable_raw_mode();

public:
    InputHandler();
    ~InputHandler();

    Action read() const;
};