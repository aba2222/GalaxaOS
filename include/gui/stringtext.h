#ifndef __MYOS__GUI__STRINGTEXT_H
#define __MYOS__GUI__STRINGTEXT_H

#include <common/types.h>
#include <gui/widget.h>
#include <stdint.h>

namespace myos {
    namespace gui {
        class StringText : public Widget {
        public:
            StringText(Widget* parent, int32_t x, int32_t y,
                uint32_t w, uint32_t h, uint8_t r,
                uint8_t g, uint8_t b, common::String* stringText);
            void Draw(common::SuperGraphicsContext* gc);

            common::String* thisStringText;
        };
    }
}

#endif