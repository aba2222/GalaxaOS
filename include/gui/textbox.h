#pragma once
#include "gui/widget.h"

namespace myos {
namespace gui {

class TextBox : public Widget {
public:
    TextBox(CompositeWidget* parent, int32_t x, int32_t y, uint32_t w, uint32_t h,
            uint8_t r, uint8_t g, uint8_t b, String* text);

    virtual void Draw(myos::common::SuperGraphicsContext* gc) override;
    virtual void OnKeyDown(char str) override;
    virtual void OnKeyUp(char str) override;

protected:
    size_t cursorPos;
};

} // namespace gui
} // namespace myos
