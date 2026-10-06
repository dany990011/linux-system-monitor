#pragma once
#include <termios.h>


class Terminal{
    private:
        termios oldSettings;
    
    public:
        Terminal();
        ~Terminal();
        char readKey();
};