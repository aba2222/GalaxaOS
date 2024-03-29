#include "hardwarecommunication/multitasking.h"
#include "hardwarecommunication/interrupts.h"

using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos;

void printf(const char* str);

Task::Task(GlobalDescriptorTable* gdt, void entrypoint()) {
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector() << 3;
    cpustate->eflags = 0x202;
    taskState = 1;
}

Task::~Task() {}

TaskManager::TaskManager() 
    : thisTcb(nullptr),
      nowTask(nullptr),
      status(0),
      IRQDisableCounter(0) {
}

TaskManager::~TaskManager() {}

bool TaskManager::AddTask(Task* task) {
    if(thisTcb == nullptr) {
        thisTcb = task;
        thisTcb->nextTask = task;
        thisTcb->headTask = task;
        return true;
    }

    thisTcb->nextTask = task;
    task->headTask = thisTcb->headTask;
    task->nextTask = thisTcb->headTask;
    thisTcb = task;

    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate) {
    if(thisTcb == nullptr) {
        return cpustate;
    }
    if(nowTask == nullptr) {
        nowTask = thisTcb;
    }

    if(nowTask != 0 && status != 0) {
        nowTask->cpustate = cpustate;
    }
    if(status == 0) {
        status = 1;
    }

    nowTask = nowTask->nextTask;
    return nowTask->cpustate;
}

void TaskManager::LockScheduler(void) {
    #ifndef SMP
        if(nowIntManagerM != nullptr) {
            nowIntManagerM->Deactivate();
        }
        else asm("cli");
        IRQDisableCounter++;
    #endif
}
 
void TaskManager::UnLockScheduler(void) {
    #ifndef SMP
        IRQDisableCounter--;
        if(IRQDisableCounter == 0) {
            if(nowIntManagerM != nullptr) {
                nowIntManagerM->Activate();
            }
            else asm("cli");
        }
    #endif
}

void TaskManager::BlockTask(int reason) {
    LockScheduler();
    nowTask->taskState = reason;
    //Schedule(nowTask->cpustate);
    UnLockScheduler();
}

//不可用
void TaskManager::UnblockTask(Task* task) {
    LockScheduler();
    if(nowTask == nullptr) { 
        //TODO
        //switch_to_task(task);
        return;
    } else {
        //task->nextTask = nowTask->nextTask;
        //nowTask->nextTask = task;
        //nowTask = task;t
        task->taskState = 1;
    }
    UnLockScheduler();
}