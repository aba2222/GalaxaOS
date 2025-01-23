#ifndef __MYOS__GUI__BUTTON_H
#define __MYOS__GUI__BUTTON_H

#include "gui/widget.h"

namespace myos {
    namespace gui {
        class Button : public Widget {
        public:
            Button(Widget* parent, common::int32_t x, common::int32_t y, common::uint32_t w, common::uint32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b, common::String* text);
            
            void SetOnClick(void (*handler)());
            virtual void Draw(common::SuperGraphicsContext* gc) override;
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button) override;
            virtual void Animate() override;

        private:
            void (*onClick)();
        };
    }
}

#endif
