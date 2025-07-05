#include "syscalls.h"
#include "memorymanager.h"
#include "hardwarecommunication/multitasking.h"

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
        case 1:  // exit
            printf("Process exited with code %d\n", cpu->ebx);
            TaskManager::nowTaskManager->TerminateTask();
            break;

        case 3:  // read
            // 从设备/输入模拟读取，例如从键盘缓冲区读取
            // 这里只是个假设，实际应该有 keyboard driver
            ((char*)cpu->ebx)[0] = 'A'; // 假设返回一个字符 A
            cpu->eax = 1;               // 返回读取的字节数
            break;

        case 4:  // write
            printf((const char*)cpu->ebx);
            break;

        case 5:  // malloc
            cpu->eax = (uint32_t)MemoryManager::activeMemoryManager->malloc(cpu->ebx);
            break;

        case 6:  // free
            MemoryManager::activeMemoryManager->free((void*)cpu->ebx);
            break;

        default:
            printf("Unknown syscall: %d\n", cpu->eax);
            break;
    }

    return esp;
}
