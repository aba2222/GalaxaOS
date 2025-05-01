#ifndef __MYOS__GUI__DESKTOP_H
#define __MYOS__GUI__DESKTOP_H

#include <stdint.h>
#include <string.h>
#include "gui/widget.h"
#include "gui/window.h"
#include "gui/stringtext.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"

namespace myos {
    namespace gui {
        class Desktop : public CompositeWidget, public drivers::MouseEventHandler, public drivers::KeyBoardEventHandler {
        public:
            Desktop(uint32_t w, uint32_t h, uint8_t r,
                    uint8_t g, uint8_t b, common::SuperGraphicsContext* gc, String* shellText);
            ~Desktop();
            
            void OnMouseDown(uint8_t button);
            void OnMouseUp(uint8_t button);
            void OnMouseMove(int8_t x, int8_t y);

            void Draw();

            void OnKeyDown(char);
            void OnKeyUp(char);
            
        protected:
            uint32_t MouseX;
            uint32_t MouseY;
            common::SuperGraphicsContext* gc;

            StringText shellGfx;
        };
    }
}

#endif