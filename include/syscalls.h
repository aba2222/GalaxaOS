#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/multitasking.h"

namespace myos {
    class SysCallHandler : public hardwarecommunication::InterruptHandler {
    public:
        SysCallHandler(myos::hardwarecommunication::InterruptManager* manger, myos::common::uint8_t interruptNumber);
        ~SysCallHandler();

        virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
    };
}

#endif