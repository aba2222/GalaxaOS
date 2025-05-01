#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include <stdint.h>
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/multitasking.h"

namespace myos {
    class SysCallHandler : public hardwarecommunication::InterruptHandler {
    public:
        SysCallHandler(myos::hardwarecommunication::InterruptManager* manger, uint8_t interruptNumber);
        ~SysCallHandler();

        virtual uint32_t HandleInterrupt(uint32_t esp);
    };
}

#endif