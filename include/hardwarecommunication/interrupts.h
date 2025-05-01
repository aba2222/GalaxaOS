#ifndef __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <stdint.h>
#include "common/types.h"
#include "hardwarecommunication/multitasking.h"
#include "hardwarecommunication/port.h"
#include "gdt.h"

namespace myos {
    namespace hardwarecommunication {
        class InterruptManager;

        class InterruptHandler {
        public:
            virtual uint32_t HandleInterrupt(uint32_t esp);
        protected:
            InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();

            uint8_t interruptNumber;
            InterruptManager* interruptManager;
        };


        class InterruptManager {
            friend class InterruptHandler;
        public:
            InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager);
            ~InterruptManager();

            uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();

        protected:
            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];
            TaskManager* taskManager;

            struct GateDescriptor {
                uint16_t handlerAddressLowBits;
                uint16_t gdt_codeSegmentSelector;
                uint8_t reserved;
                uint8_t access;
                uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct  InterruptDescriptorTablePointer {
                uint16_t size;
                uint32_t base;
            } __attribute__((packed));

            static void SetInterruptDescriptorTableEntry(
                uint8_t interruptNumber,
                uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                uint8_t DescriptorPrivillegelLev,
                uint8_t DescriptorType
            );

            uint16_t hardwareInterruptOffset;

            static void InterruptIgnore();

            static uint32_t HandleInterrupt(uint8_t interruptNumber, uint32_t esp);
            uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleInterruptRequest0x80();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            myos::hardwarecommunication::Port8BitSlow picMasterCommand;
            myos::hardwarecommunication::Port8BitSlow picMasterData;
            myos::hardwarecommunication::Port8BitSlow picSlaveCommand;
            myos::hardwarecommunication::Port8BitSlow picSlaveData;
        };

        static InterruptManager* nowIntManagerM;
    }
}


#endif