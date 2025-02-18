#ifndef __MYOS__DRIVERS_NETWORK_REALTEK_8139_H
#define __MYOS__DRIVERS_NETWORK_REALTEK_8139_H

#include "common/types.h"
#include "drivers/driver.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/pci.h"

namespace myos {
    namespace drivers {
        class realtek_8139 : public Driver {
        public:
            realtek_8139(hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor* dev);
            ~realtek_8139();
            int ReadEeprom(long ioaddr, int location, int addr_len);

            void Activate();
            const char* GetDriverName();
        private:
            common::uint8_t MAC[6];
            hardwarecommunication::Port8Bit eepromPort;
            hardwarecommunication::Port8Bit config1;
        };
    }
}

#endif