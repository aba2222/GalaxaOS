#ifndef __MULTITASKING_H
#define __MULTITASKING_H

#include <stdint.h>
#include "gdt.h"

namespace myos {
    namespace hardwarecommunication {
        struct CPUState {
            uint32_t eax;
            uint32_t ebx;
            uint32_t ecx;
            uint32_t edx;

            uint32_t esi;
            uint32_t edi;
            uint32_t ebp;

            uint32_t error;

            uint32_t eip;
            uint32_t cs;
            uint32_t eflags;
            uint32_t esp;
            uint32_t ss;
        } __attribute__((packed));

        class Task {
            friend class TaskManager;
        public:
            Task(GlobalDescriptorTable* gdt, void entrypoint());
            ~Task();
            
        private:
            uint8_t stack[4096];
            CPUState* cpustate;
            Task* headTask;
            Task* nextTask;
            int taskState;
            uint64_t sleepExpiry;
        };

        class TaskManager {
        public:
            TaskManager(GlobalDescriptorTable* gdt);
            ~TaskManager();
            bool AddTask(Task* task);
            CPUState* Schedule(CPUState* cpustate);
            void LockScheduler(void);
            void UnLockScheduler(void);
            void LockStuff(void);
            void UnlockStuff(void);
            void BlockTask(int reason);
            void UnblockTask(Task* task);
            void TerminateTask();
            void NanoSleep(uint64_t nanoseconds);
            void NanoSleepUntil(uint64_t when);
            static void CleanerTask();
            static TaskManager* nowTaskManager;
    
        private:
            Task* thisTcb;
            Task* nowTask;
            Task* cleanerTask;
            bool cleanerTaskCreated;
            bool status;
            int IRQDisableCounter;
            int PostponeTaskSwitchesCounter;
            uint64_t timeSinceBoot = 0;
            uint64_t timeBetweenTicks = 1000000;  // 1000 Hz = 1 ms between ticks = 1000000 nanoseconds between ticks
        };
    }
}

#endif