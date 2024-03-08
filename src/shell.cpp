#include "shell.h"
using namespace myos;
using namespace myos::common;

Shell::Shell() 
     : x(0) {
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
        case '\0':
            break;
        default:
            shellText[x] = str[i];
            x++;
            break;
        }
    }
    shellText[x] = '\0';
}

void Shell::CleanText() {
    //TODO:去掉了y会出问题
    for(int y = 0; y < 25; y++) {
        for(x = 0; x < 80; x++) {
            shellText[80 * y + x] = ' ';
        }
    }
    x = 0;
}

uint8_t* Shell::GetShellText() {
    return shellText;
}