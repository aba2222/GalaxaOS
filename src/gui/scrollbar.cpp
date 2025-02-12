#include "gui/scrollbar.h"

using namespace myos::common;
using namespace myos::gui;

Scrollbar::Scrollbar(Widget* parent, int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b)
    : Widget(parent, x, y, w, h, r, g, b, 0, nullptr) {}

Scrollbar::~Scrollbar() {}

void Scrollbar::Draw(SuperGraphicsContext* gc) {
    int X=0;
    int Y=0;
    ModelToScreen(X, Y);
    gc->FillRectangle(X, Y, w, h, r, g, b);
}
