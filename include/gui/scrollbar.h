#ifndef __MYOS__GUI__SCROLLBAR_H
#define __MYOS__GUI__SCROLLBAR_H

#include "gui/widget.h"

namespace myos {
    namespace gui {
        class Scrollbar : public Widget {
        public:
            Scrollbar(Widget* parent, common::int32_t x, common::int32_t y,
                      common::uint32_t w, common::uint32_t h, common::uint8_t r,
                      common::uint8_t g, common::uint8_t b);
            ~Scrollbar();

            void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button) override;
            void Draw(common::SuperGraphicsContext* gc) override;

            common::int32_t position;
        };
    }
}

#endif
