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
        common::String GetShellText();
        void CleanText();       
    protected:
        common::String shellText;
        common::uint8_t x;
    };
}

#endif