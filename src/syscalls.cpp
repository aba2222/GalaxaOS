#include "syscalls.h"

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;

void printf(const char*, ...);

SysCallHandler::SysCallHandler(InterruptManager* manger, uint8_t interruptNumber)
            : InterruptHandler(interruptNumber + manger->HardwareInterruptOffset(), manger) {}

SysCallHandler::~SysCallHandler() {}

uint32_t SysCallHandler::HandleInterrupt(uint32_t esp) {
    CPUState* cpu = (CPUState*) esp;

    switch(cpu->eax) {
        case 4:
            printf((const char*)cpu->ebx);
            break;
        
        default:
            break;
    }

    return esp;
}