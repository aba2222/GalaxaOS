#ifndef __MYOS__GUI__STRINGTEXT_H
#define __MYOS__GUI__STRINGTEXT_H

#include <common/types.h>
#include <gui/widget.h>

namespace myos {
    namespace gui {
        class StringText : public Widget {
        public:
            StringText(Widget* parent, common::int32_t x, common::int32_t y,
                common::uint32_t w, common::uint32_t h, common::uint8_t r,
                common::uint8_t g, common::uint8_t b, common::String stringText);
            void Draw(common::SuperGraphicsContext* gc);
            common::String stringText;
        };
    }
}

#endif