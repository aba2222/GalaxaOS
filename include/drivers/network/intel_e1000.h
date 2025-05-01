#ifndef __MYOS__DRIVERS_NETWORK_INTEL_E1000_H
#define __MYOS__DRIVERS_NETWORK_INTEL_E1000_H

#include "common/types.h"
#include "drivers/driver.h"
#include "hardwarecommunication/pci.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/mmioutils.h"

#define REG_EEPROM      0x0014

namespace myos {
    namespace drivers {
        class intel_e1000 : public Driver {
        public:
            intel_e1000(hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor* dev);
            ~intel_e1000();
            virtual void Activate();
            const virtual char* GetDriverName();
        private:
            bool eerpromExists;
            uint8_t bar_type;
            uint64_t mem_base;
            uint8_t mac [6];
            hardwarecommunication::Port32Bit dataPort;

            uint32_t ReadCommand(uint16_t p_address);
            void WriteCommand(uint16_t p_address, uint32_t p_value);
            bool detectEEProm();
            uint32_t eepromRead(uint8_t addr);
            bool ReadMACAddress();
            
        };
    }
}

#endif