#ifndef __MYOS__GUI__DESKTOP_H
#define __MYOS__GUI__DESKTOP_H

#include "gui/widget.h"
#include "gui/window.h"
#include "drivers/mouse.h"

namespace myos {
    namespace gui {
        class Desktop : public CompositeWidget, public drivers::MouseEventHandler {
        public:
            Desktop(common::uint32_t w, common::uint32_t h, common::uint8_t r,
                    common::uint8_t g, common::uint8_t b);
            ~Desktop();

            void OnMouseDown(common::uint8_t button);
            void OnMouseUp(common::uint8_t button);
            void OnMouseMove(common::int8_t x, common::int8_t y);

            void Draw(common::GraphicsContext* gc);

        protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;
        };
    }
}

#endif