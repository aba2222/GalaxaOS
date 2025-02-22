#ifndef __MYOS__DRIVERS_NETWORK_REALTEK_8139_H
#define __MYOS__DRIVERS_NETWORK_REALTEK_8139_H

#include "common/types.h"
#include "drivers/driver.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/pci.h"

namespace myos {
    namespace drivers {
        class realtek_8139 : public Driver, public hardwarecommunication::InterruptHandler {
        public:
            realtek_8139(hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor* dev, 
                         hardwarecommunication::InterruptManager* interrupts);
            ~realtek_8139();
            int ReadEeprom(long ioaddr, int location, int addr_len);
            common::uint32_t HandleInterrupt(common::uint32_t esp);

            void Activate();
            const char* GetDriverName();
        private:

            common::uint32_t portBase;
            common::uint8_t MAC[6];
            common::uint8_t recvBufferDescrMemory[8192 + 16];
            hardwarecommunication::Port32Bit rxBufPort;
            hardwarecommunication::Port8Bit chipCmdPort;
            hardwarecommunication::Port16Bit intrMaskPort;
            hardwarecommunication::Port32Bit rxConfigPort;
            hardwarecommunication::Port8Bit eepromPort;
            hardwarecommunication::Port8Bit config1;
        };
    }
}

#endif