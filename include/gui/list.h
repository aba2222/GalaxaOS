#ifndef __MYOS__GUI__LIST_H
#define __MYOS__GUI__LIST_H

#include "gui/widget.h"
#include "gui/scrollbar.h"

namespace myos {
    namespace gui {
        class List : public CompositeWidget {
        public:
            List(Widget* parent, common::int32_t x, common::int32_t y,
                 common::uint32_t w, common::uint32_t h, common::uint8_t r,
                 common::uint8_t g, common::uint8_t b, common::uint8_t classNum, common::String* stringText);
            ~List();

            void AddItem(common::String* itemText);
            void Draw(common::SuperGraphicsContext* gc) override;
            void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
        private:
            Scrollbar* scrollbar;
        };
    }
}

#endif
