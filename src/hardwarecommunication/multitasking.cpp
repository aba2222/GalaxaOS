#include "hardwarecommunication/multitasking.h"
#include "hardwarecommunication/interrupts.h"
#include "memorymanager.h"

using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos;
void printf(const char*, ...);
#define SLEEPING 2
#define PAUSED 3
#define TERMINATED 4

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
    sleepExpiry = 0;
}

Task::~Task() {}

TaskManager::TaskManager(GlobalDescriptorTable* gdt) 
    : thisTcb(nullptr),
      nowTask(nullptr),
      status(0),
      IRQDisableCounter(0),
      PostponeTaskSwitchesCounter(0),
      cleanerTaskCreated(0),
      cleanerTask(new Task(gdt, CleanerTask)) {
    if(nowTaskManager == nullptr) {
        nowTaskManager = this;
    }
}

TaskManager::~TaskManager() {}

bool TaskManager::AddTask(Task* task) {
    if (thisTcb == nullptr) {
        thisTcb = task;
        thisTcb->nextTask = task;
        thisTcb->headTask = task;
    } else {  
        task->nextTask = thisTcb->headTask->nextTask;
        thisTcb->headTask->nextTask = task;
        thisTcb = task;
    }

    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate) {
    timeSinceBoot += timeBetweenTicks;

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
    Task* runTask = nullptr;
    do {
        if(nowTask->sleepExpiry <= timeSinceBoot && nowTask->sleepExpiry != 0) {
            // Task needs to be woken up
            nowTask->sleepExpiry = 0;
            UnblockTask(nowTask);
        }
        if(runTask == nullptr && nowTask->nextTask->taskState == 1) {
            runTask = nowTask->nextTask;
        }
        nowTask = nowTask->nextTask;  // 切换到下一个任务
    } while (nowTask != startTask);  // 找到可运行任务或遍历一轮
    if (runTask != nullptr) {
        nowTask = runTask;
    }
    
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

void TaskManager::LockStuff(void) {
    #ifndef SMP
    if(nowIntManagerM != nullptr) {
        nowIntManagerM->Deactivate();
    }
    else asm("cli");

    IRQDisableCounter++;
    PostponeTaskSwitchesCounter++;
    #endif
}
    
void TaskManager::UnlockStuff(void) {
    #ifndef SMP
    PostponeTaskSwitchesCounter--;
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
    //Schedule(nowTask->cpustate);
    UnLockScheduler();
}

void TaskManager::UnblockTask(Task* task) {
    LockScheduler();
    
    task->taskState = 1;

    /*if (nowTask == nullptr) {
        nowTask = task;
    } else {
        task->nextTask = nowTask->nextTask;
        nowTask->nextTask = task;
    }*/

    UnLockScheduler();
}

void TaskManager::TerminateTask() {
    // Note: Can do any harmless stuff here (close files, free memory in user-space, ...) but there's none of that yet
    LockStuff();

    // Put this task on the terminated task list
    /*LockScheduler();
    current_task_TCB->next = terminated_task_list;
    terminated_task_list = current_task_TCB;
    UnLockScheduler();*/
    
    // Block this task (note: task switch will be postponed until scheduler lock is released)
    BlockTask(TERMINATED);
    
    // Make sure the cleaner task isn't paused
    if(cleanerTaskCreated) {
        UnblockTask(cleanerTask);
    } else {
        AddTask(cleanerTask);
        cleanerTaskCreated = 1;
    }
    
    // Unlock the scheduler's lock 
    UnlockStuff();
}

void TaskManager::CleanerTask() {
    TaskManager::nowTaskManager->LockStuff();

    Task* task = TaskManager::nowTaskManager->nowTask;
    Task* startTask = task;
    do {
        if(task->taskState == TERMINATED) {
            task->headTask->nextTask = task->nextTask;
            task->nextTask->headTask = task->headTask;

            MemoryManager::activeMemoryManager->free(task);
        }
        task = task->nextTask;  // 切换到下一个任务
    } while (task != startTask);  // 找到可运行任务或遍历一轮

    TaskManager::nowTaskManager->BlockTask(PAUSED);
    TaskManager::nowTaskManager->UnlockStuff();
}


void TaskManager::NanoSleep(common::uint64_t nanoseconds) {
    NanoSleepUntil(timeSinceBoot + nanoseconds);
}

void TaskManager::NanoSleepUntil(common::uint64_t when) {
    LockStuff();

    // Make sure "when" hasn't already occured
    if(when < timeSinceBoot) {
        UnLockScheduler();
        return;
    }

    // Set time when task should wake up
    nowTask->sleepExpiry = when;

    // Add task to the start of the unsorted list of sleeping tasks
    UnlockStuff();

    // Find something else for the CPU to do
    BlockTask(SLEEPING);
}