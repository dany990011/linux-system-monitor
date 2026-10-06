#include "Terminal.hpp"
#include <unistd.h>

Terminal::Terminal()
{
    termios newSettings;

    tcgetattr(STDIN_FILENO, &oldSettings);

    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    newSettings.c_cc[VMIN] = 0;
    newSettings.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
}

Terminal::~Terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
}

char Terminal::readKey()
{
    char key;

    if(read(STDIN_FILENO, &key, 1) > 0 ){
        return key;
    }

    return '\0';

}
