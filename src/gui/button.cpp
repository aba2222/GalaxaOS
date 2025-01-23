#include "gui/button.h"

using namespace myos::common;
using namespace myos::gui;

Button::Button(Widget* parent, int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b, String* text)
    : Widget(parent, x, y, w, h, r, g, b, 0, text), onClick(0) {}

void Button::SetOnClick(void (*handler)()) {
    onClick = handler;
}

void Button::Draw(SuperGraphicsContext* gc) {
    Widget::Draw(gc);
    int X=0;
    int Y=0;
    ModelToScreen(X, Y);
    if (stringText) {
        gc->PutString((const uint8_t*)stringText->c_str(), X, Y, 0x00, (r << 24) + (g << 16) + (b << 8), w);
    }
}

void Button::OnMouseDown(int32_t x, int32_t y, uint8_t button) {
    if (onClick) {
        onClick();
    }
}

void Button::Animate() {
    // 实现动画逻辑
}
