#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H

#include <stdint.h>
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace myos {
    namespace drivers {
        class MouseEventHandler {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(uint8_t button);
            virtual void OnMouseUp(uint8_t button);
            virtual void OnMouseMove(int8_t x, int8_t y);
        };

        class MouseDriver : public myos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            MouseDriver(myos::hardwarecommunication::InterruptManager* manger, MouseEventHandler* handler);
            ~MouseDriver();
            virtual uint32_t HandleInterrupt(uint32_t esp);
            virtual void Activate();
            const virtual char* GetDriverName();
        private:
            myos::hardwarecommunication::Port8Bit dataport;
            myos::hardwarecommunication::Port8Bit commandport;

            uint8_t buffer[3];
            uint8_t offset;
            uint8_t buttons;

            int8_t x,y;
            MouseEventHandler* handler;
        };
    }
}


#endif