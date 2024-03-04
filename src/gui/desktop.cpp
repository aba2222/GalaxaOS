#include <gui/desktop.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;


Desktop::Desktop(common::uint32_t w, common::uint32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b)
:   CompositeWidget(0,0,0, w,h,r,g,b, 0, nullptr),
    MouseEventHandler() {
    MouseX = 512;
    MouseY = 384;
}

Desktop::~Desktop() {
}

void Desktop::Draw(common::SuperGraphicsContext* gc) {
    gc->Redraw();
    CompositeWidget::Draw(gc);
    
    for(int i = 0; i < 4; i++)
    {
        gc -> PutPixel(MouseX-i, MouseY, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX+i, MouseY, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX, MouseY-i, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX, MouseY+i, 0xFF, 0xFF, 0xFF);
    }
}
            
void Desktop::OnMouseDown(myos::common::uint8_t button) {
    CompositeWidget::OnMouseDown(MouseX, MouseY, button);
}

void Desktop::OnMouseUp(myos::common::uint8_t button) {
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);
}

void Desktop::OnMouseMove(int8_t x, int8_t y) {
    int32_t newMouseX = MouseX + x;
    if(newMouseX < 0) newMouseX = 0;
    if(newMouseX >= w) newMouseX = w - 1;
    
    int32_t newMouseY = MouseY + y;
    if(newMouseY < 0) newMouseY = 0;
    if(newMouseY >= h) newMouseY = h - 1;
    
    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);
    
    MouseX = newMouseX;
    MouseY = newMouseY;
}

void Desktop::OnKeyDown(char str) {
    //CompositeWidget::children[0]->OnKeyDown(str);
    CompositeWidget::OnKeyDown(str);
}

void Desktop::OnKeyUp(char str) {
    CompositeWidget::OnKeyDown(str);
}