#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include <stdint.h>
#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/interrupts.h"
#include "drivers/driver.h"


namespace myos {
    namespace hardwarecommunication {
        enum BaseAddressRegisterType {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister {
        public:
            bool prefetchable;
            uint8_t* address;
            uint32_t size;
            BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor {
        public:
            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        
            uint32_t portBase;
            uint8_t bar0_type;
            uint32_t bar0_adress;
            uint32_t interrupt;

            uint8_t bus;
            uint8_t device;
            uint8_t function;

            uint16_t device_id;
            uint16_t vendor_id;

            uint8_t class_id;
            uint8_t subclass_id;
            uint8_t interface_id;
            uint8_t revision;
        };

        class PeripheralComponentInterconnectController {
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            uint32_t Read(uint8_t bus,
                uint8_t device,
                uint8_t function,
                uint8_t registeroffset);

            void Write(uint8_t bus,
                uint8_t device,
                uint8_t function,
                uint8_t registeroffset,
                uint32_t value);

            bool DeviceHasFunctions(uint8_t bus, uint8_t device);
            
            void SelectDrivers(myos::drivers::DriverManger* driverManger, myos::hardwarecommunication::InterruptManager* interrupts);
            myos::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, myos::hardwarecommunication::InterruptManager* interrupts);

            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(uint8_t bus,
                uint8_t device,
                uint8_t function);
            BaseAddressRegister GetBaseAddressRegister(uint8_t bus,
                uint8_t device,
                uint8_t function,
                uint8_t bar);
        private:
            Port32Bit dataPort;
            Port32Bit commandPort;
        };
    }
}


#endif