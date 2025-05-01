#ifndef __MYOS__GUI__SCROLLBAR_H
#define __MYOS__GUI__SCROLLBAR_H

#include <stdint.h>
#include "gui/widget.h"

namespace myos {
    namespace gui {
        class Scrollbar : public Widget {
        public:
            Scrollbar(Widget* parent, int32_t x, int32_t y,
                      uint32_t w, uint32_t h, uint8_t r,
                      uint8_t g, uint8_t b);
            ~Scrollbar();

            void Draw(common::SuperGraphicsContext* gc) override;
        };
    }
}

#endif
