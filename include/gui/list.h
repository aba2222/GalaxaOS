#ifndef __MYOS__GUI__LIST_H
#define __MYOS__GUI__LIST_H

#include <stdint.h>
#include <string.h>
#include "gui/widget.h"
#include "gui/scrollbar.h"

namespace myos {
    namespace gui {
        class List : public CompositeWidget {
        public:
            List(Widget* parent, int32_t x, int32_t y,
                 uint32_t w, uint32_t h, uint8_t r,
                 uint8_t g, uint8_t b, uint8_t classNum, String* stringText);
            ~List();

            void AddItem(String* itemText);
            void Draw(common::SuperGraphicsContext* gc) override;
            void OnMouseDown(int32_t x, int32_t y, uint8_t button);
        private:
            Scrollbar* scrollbar;
        };
    }
}

#endif
