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
            common::uint8_t bar_type;
            common::uint64_t mem_base;
            common::uint8_t mac [6];
            hardwarecommunication::Port32Bit dataPort;

            common::uint32_t ReadCommand(common::uint16_t p_address);
            void WriteCommand(common::uint16_t p_address, common::uint32_t p_value);
            bool detectEEProm();
            common::uint32_t eepromRead(common::uint8_t addr);
            bool ReadMACAddress();
            
        };
    }
}

#endif