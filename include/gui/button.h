#ifndef __MYOS__GUI__BUTTON_H
#define __MYOS__GUI__BUTTON_H

#include <stdint.h>
#include "gui/widget.h"

namespace myos {
    namespace gui {
        class Button : public Widget {
        public:
            Button(Widget* parent, int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b, common::String* text);
            
            void SetOnClick(void (*handler)());
            virtual void Draw(common::SuperGraphicsContext* gc) override;
            virtual void OnMouseDown(int32_t x, int32_t y, uint8_t button) override;
            virtual void Animate() override;

        private:
            void (*onClick)();
        };
    }
}

#endif
