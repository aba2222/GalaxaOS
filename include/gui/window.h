#ifndef __MYOS__GUI__WINDOW_H
#define __MYOS__GUI__WINDOW_H

#include <stdint.h>
#include "gui/widget.h"
#include "gui/desktop.h"

namespace myos {
    namespace gui {
        class Window : public CompositeWidget {
        public:
            Window(Widget* parent, int32_t x, int32_t y,
                    uint32_t w, uint32_t h, uint8_t r,
                    uint8_t g, uint8_t b, common::String* stringText);
            ~Window();

            void OnMouseDown(int32_t x, int32_t y, uint8_t button);
            void OnMouseUp(int32_t x, int32_t y, uint8_t button);
            void OnMouseMove(int32_t x, int32_t y, int32_t nx, int32_t ny);
        private:
            bool Dragging;
        };
    }
}

#endif