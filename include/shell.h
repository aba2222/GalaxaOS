#ifndef __MYOS__SHELL_H
#define __MYOS__SHELL_H

#include "memorymanager.h"
#include "common/types.h"

namespace myos {
    class Shell {
    public:
        Shell();
        ~Shell();

        void ShellPrintf(const char* str);        
        common::uint8_t* GetShellText();
        void CleanText();       
    protected:
        common::uint8_t* shellText;
        common::uint8_t x = 0, y = 0;
    };
}

#endif