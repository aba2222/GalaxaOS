#include "gui/window.h"
#include "gui/stringtext.h"

using namespace myos::common;
using namespace myos::gui;

Window::Window(Widget* parent,
            int32_t x, int32_t y, uint32_t w, uint32_t h,
            uint8_t r, uint8_t g, uint8_t b, String stringText)
: CompositeWidget(parent, x,y,w,h, r,g,b, 1, stringText) {
    Dragging = false;
    StringText* toolTab = new StringText(this,2,0,w,h,128,128,128,&stringText);
    this->AddChild(toolTab);
}

Window::~Window() {
}

void Window::OnMouseDown(int32_t x, int32_t y, uint8_t button) {
    Dragging = 1;
    button = 1;
    CompositeWidget::OnMouseDown(x,y,button);
}

void Window::OnMouseUp(int32_t x, int32_t y, uint8_t button) {
    Dragging = false;
    CompositeWidget::OnMouseUp(x,y,button);
}

void Window::OnMouseMove(int32_t ox, int32_t oy, int32_t nx, int32_t ny) {
    if (Dragging) {
        if((this->x + (nx - ox)) > 0 && (this->x + (nx - ox) + w) < 1024) this->x += (nx - ox);
        if((this->y + (ny - oy)) > 0 && (this->y + (ny - oy) + h) < 768) this->y += (ny - oy);
    }
    CompositeWidget::OnMouseMove(ox,oy,nx, ny);

}