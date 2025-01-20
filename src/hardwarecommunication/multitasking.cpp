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
    } else {
        task->nextTask = thisTcb->headTask;
        thisTcb->nextTask = task;
        thisTcb = task;
    }

    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate) {
    if (thisTcb == nullptr) {
        return cpustate;  // 没有任务可调度，返回当前CPU状态
    }

    if (nowTask != nullptr) {
        nowTask->cpustate = cpustate;  // 保存当前任务状态
    } else {
        nowTask = thisTcb;  // 初始化当前任务为第一个任务
    }

    // 查找下一个可运行的任务
    Task* startTask = nowTask;
    do {
        nowTask = nowTask->nextTask;  // 切换到下一个任务
    } while (nowTask->taskState != 1 && nowTask != startTask);  // 找到可运行任务或遍历一轮

    // 如果所有任务都阻塞，返回当前任务的状态
    if (nowTask->taskState != 1) {
        return cpustate;
    }

    return nowTask->cpustate;  // 返回新任务的CPU状态
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
            else asm("sti");
        }
    #endif
}

void TaskManager::BlockTask(int reason) {
    LockScheduler();
    nowTask->taskState = reason;
    Schedule(nowTask->cpustate);
    UnLockScheduler();
}

//不可用
void TaskManager::UnblockTask(Task* task) {
    LockScheduler();
    
    task->taskState = 1;

    if (nowTask == nullptr) {
        nowTask = task;
    } else {
        task->nextTask = nowTask->nextTask;
        nowTask->nextTask = task;
    }

    UnLockScheduler();
}