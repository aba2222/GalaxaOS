#ifndef __MYOS__HARDWARECOMMUNICATION__TIME_H
#define __MYOS__HARDWARECOMMUNICATION__TIME_H
#define CURRENT_YEAR        2024                            // Change this each year!

#include <stdint.h>
#include "hardwarecommunication/port.h"

namespace myos {
    namespace hardwarecommunication {
        class Times {
        public:
            Times(uint8_t cmos_address, uint8_t cmos_data);
            ~Times();

            int GetUpdateInProgressFlag();
            uint8_t GetRTCRegister(int reg);
            void ReadRtc();

            uint8_t second;
            uint8_t minute;
            uint8_t hour;
            uint8_t day;
            uint8_t month;
            uint8_t year;
            uint8_t century;
            uint8_t century_register;
        private:
            Port8Bit CMOSAddress;
            Port8Bit CMOSData;
            //uint8_t cmosAddress;
            //uint8_t cmosData;
        };
    }
}


#endif