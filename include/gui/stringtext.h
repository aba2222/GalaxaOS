#ifndef __MYOS__GUI__STRINGTEXT_H
#define __MYOS__GUI__STRINGTEXT_H

#include <stdint.h>
#include <string.h>
#include <gui/widget.h>

namespace myos {
    namespace gui {
        class StringText : public Widget {
        public:
            StringText(Widget* parent, int32_t x, int32_t y,
                uint32_t w, uint32_t h, uint8_t r,
                uint8_t g, uint8_t b, String* stringText);
            void Draw(common::SuperGraphicsContext* gc);

            String* thisStringText;
        };
    }
}

#endif