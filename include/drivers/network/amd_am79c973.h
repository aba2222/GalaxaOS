#ifndef __MYOS__DRIVERS_AMD_AM79C973_H
#define __MYOS__DRIVERS_AMD_AM79C973_H

#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/pci.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"
#include "common/types.h"

namespace myos {
    namespace drivers {
        class amd_am79c973 : public Driver, public hardwarecommunication::InterruptHandler {
        public:
            amd_am79c973(hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor* dev, 
                         hardwarecommunication::InterruptManager* interrupts);
            ~amd_am79c973();

            void Activate();
            int Reset();
            uint32_t HandleInterrupt(uint32_t esp);

            void Send(uint8_t* buffer, int size);
            void Receive();

        private:
            struct InitializationBlock {
                uint16_t mode;
                unsigned reserved1 : 4;
                unsigned numSendBuffers : 4;
                unsigned reserved2 : 4;
                unsigned numRecvBuffers : 4;
                uint64_t physicalAddress : 48;
                uint16_t reserved3;
                uint64_t logicalAddress;
                uint32_t recvBufferDescrAddress;
                uint32_t sendBufferDescrAddress;
            } __attribute__((packed));

            struct BufferDescriptor {
                uint32_t address;
                uint32_t flags;
                uint32_t flags2;
                uint32_t avail;
            } __attribute__((packed));

            hardwarecommunication::Port16Bit MacAddress0Port;
            hardwarecommunication::Port16Bit MacAddress2Port;
            hardwarecommunication::Port16Bit MacAddress4Port;
            hardwarecommunication::Port16Bit registerDataPort;
            hardwarecommunication::Port16Bit registerAddressPort;
            hardwarecommunication::Port16Bit resetPort;
            hardwarecommunication::Port16Bit busControlRegisterDataPort;

            InitializationBlock initBlock;

            BufferDescriptor* sendBufferDescr;
            uint8_t sendBufferDescrMemory[2048+15];
            uint8_t sendBuffers[2*1024+15][8];
            uint8_t currentSendBuffer;

            BufferDescriptor* recvBufferDescr;
            uint8_t recvBufferDescrMemory[2048+15];
            uint8_t recvBuffers[2*1024+15][8];
            uint8_t currentRecvBuffer;
        };
    }
}

#endif