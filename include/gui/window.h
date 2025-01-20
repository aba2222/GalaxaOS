#ifndef __MYOS__GUI__WINDOW_H
#define __MYOS__GUI__WINDOW_H

#include "gui/widget.h"
#include "gui/desktop.h"

namespace myos {
    namespace gui {
        class Window : public CompositeWidget {
        public:
            Window(Widget* parent, common::int32_t x, common::int32_t y,
                    common::uint32_t w, common::uint32_t h, common::uint8_t r,
                    common::uint8_t g, common::uint8_t b, common::String* stringText);
            ~Window();

            void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny);
        private:
            bool Dragging;
        };
    }
}

#endif