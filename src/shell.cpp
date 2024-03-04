#include "shell.h"
using namespace myos;
using namespace myos::common;

Shell::Shell() {
    shellText = (uint8_t*)(new uint8_t* [80 * 25 * sizeof(uint8_t)]);
    this->CleanText();
}

Shell::~Shell() {}

void Shell::ShellPrintf(const char* str) {
    for(int i = 0; str[i]; i++){
        switch(str[i]) {
        //case '\n':
            //y++;
            //x = 0;
            //break;
        default:
            shellText[80 * y + x] = str[i];
            x++;
            break;
        }

        if (y >=25) {
            CleanText();
        }
    }
    shellText[80 * y + x + 1] = '\0';
}

void Shell::CleanText() {
    for (y = 0; y < 25; y++) {
        for(x = 0; x < 80;x++) {
            if(x == 79) {
                shellText[80 * y + x] = '\n';
            }
            shellText[80 * y + x] = ' ';
        }
    }
    x = 0, y = 0;
}

uint8_t* Shell::GetShellText() {
    return shellText;
}