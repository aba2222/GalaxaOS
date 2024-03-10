#ifndef __MYOS__HARDWARECOMMUNICATION__TIME_H
#define __MYOS__HARDWARECOMMUNICATION__TIME_H
#define CURRENT_YEAR        2024                            // Change this each year!

#include "common/types.h"
#include "hardwarecommunication/port.h"

namespace myos {
    namespace hardwarecommunication {
        class Times {
        public:
            Times(common::uint8_t cmos_address, common::uint8_t cmos_data);
            ~Times();

            int GetUpdateInProgressFlag();
            common::uint8_t GetRTCRegister(int reg);
            void ReadRtc();

            common::uint8_t second;
            common::uint8_t minute;
            common::uint8_t hour;
            common::uint8_t day;
            common::uint8_t month;
            common::uint8_t year;
            common::uint8_t century;
            common::uint8_t century_register;
        private:
            Port8Bit CMOSAddress;
            Port8Bit CMOSData;
            //common::uint8_t cmosAddress;
            //common::uint8_t cmosData;
        };
    }
}


#endif