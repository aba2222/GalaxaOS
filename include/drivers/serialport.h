#ifndef __MYOS__DRIVERS__SERIALPORTS_H
#define __MYOS__DRIVERS__SERIALSPRTS_H

#include "string.h"
#include "hardwarecommunication/port.h"

namespace myos {
    namespace drivers {
        enum PortAddresses {
            COM1 = 0x3F8,
            COM2 = 0x2F8,
            COM3 = 0x3E8,
            COM4 = 0x2E8
        };

        class SerialPort {
        public:
            SerialPort(PortAddresses addresse);
            int IsTransmitEmpty();
            void WriteSerial(char a);
            void WriteSerial(String a);
        private:
            hardwarecommunication::Port8Bit dataPort;
        };
    }
}

#endif