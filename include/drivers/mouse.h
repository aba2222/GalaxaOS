#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace myos {
    namespace drivers {
        class MouseEventHandler {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(common::uint8_t button);
            virtual void OnMouseUp(common::uint8_t button);
            virtual void OnMouseMove(common::int8_t x, common::int8_t y);
        };

        class MouseDriver : public myos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            MouseDriver(myos::hardwarecommunication::InterruptManager* manger, MouseEventHandler* handler);
            ~MouseDriver();
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
            virtual void Activate();
            const virtual char* GetDriverName();
        private:
            myos::hardwarecommunication::Port8Bit dataport;
            myos::hardwarecommunication::Port8Bit commandport;

            myos::common::uint8_t buffer[3];
            myos::common::uint8_t offset;
            myos::common::uint8_t buttons;

            myos::common::int8_t x,y;
            MouseEventHandler* handler;
        };
    }
}


#endif