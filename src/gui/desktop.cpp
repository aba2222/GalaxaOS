#include <gui/desktop.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;

String Desktop::shellText = " ";

void printDesk(String str) {
    Desktop::shellText = Desktop::shellText + str;
}

void printDesk(char str) {
    String stri = " ";
    stri[0] = str;
    Desktop::shellText = Desktop::shellText + stri;
}

Desktop::Desktop(common::uint32_t w, common::uint32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b, common::SuperGraphicsContext* gc)
:   CompositeWidget(0,0,0, w,h,r,g,b, 0, nullptr),
    MouseEventHandler(),
    KeyBoardEventHandler(),
    gc(gc),
    shellGfx(this,10,30,w,h,0x00,0x00,0x00,"Hello! shell." ) {
    MouseX = 512;
    MouseY = 384;
    AddChild(&shellGfx);
    printDesk("Desk init done.");
}

Desktop::~Desktop() {
}

void Desktop::Draw() {
    shellGfx.thisStringText = shellText;
    gc->Redraw();
    CompositeWidget::Draw(gc);

    int cursorSize = 5;
    int borderSize = 3;

    int drawX = (MouseX < cursorSize + borderSize) ? (cursorSize + borderSize) : 
                (MouseX > w - (cursorSize + borderSize)) ? (w - (cursorSize + borderSize)) : MouseX;
    int drawY = (MouseY < cursorSize + borderSize) ? (cursorSize + borderSize) : 
                (MouseY > h - (cursorSize + borderSize)) ? (h - (cursorSize + borderSize)) : MouseY;

    for (int i = -cursorSize - borderSize; i <= cursorSize + borderSize; i++) {
        gc->PutPixel(drawX + i, drawY, 0x00, 0x00, 0x00);
        gc->PutPixel(drawX, drawY + i, 0x00, 0x00, 0x00);
    }
    for (int i = -cursorSize; i <= cursorSize; i++) {
        gc->PutPixel(drawX + i, drawY, 0xFF, 0xFF, 0xFF);
        gc->PutPixel(drawX, drawY + i, 0xFF, 0xFF, 0xFF);
    }
    
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            if (x * x + y * y <= 4) {
                gc->PutPixel(drawX + x, drawY + y, 0xFF, 0xFF, 0xFF); // 圆形部分
            }
        }
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
    printDesk(str);
    CompositeWidget::OnKeyDown(str);
}

void Desktop::OnKeyUp(char str) {
    CompositeWidget::OnKeyDown(str);
}