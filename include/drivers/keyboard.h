#ifndef __MYOS__DRIVERS__KEY_BOARD_H
#define __MYOS__DRIVERS__KEY_BOARD_H

#include <stdint.h>
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace myos {
    namespace drivers {
        class KeyBoardEventHandler {
        public:
            KeyBoardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyBoardDriver : public myos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            KeyBoardDriver(myos::hardwarecommunication::InterruptManager* manger, KeyBoardEventHandler* handler);
            ~KeyBoardDriver();
            virtual uint32_t HandleInterrupt(uint32_t esp);
            virtual void Activate();
            const virtual char* GetDriverName();
        private:
            myos::hardwarecommunication::Port8Bit dataport;
            myos::hardwarecommunication:: Port8Bit commandport;

            KeyBoardEventHandler* handler;
        };
    }
}


#endif