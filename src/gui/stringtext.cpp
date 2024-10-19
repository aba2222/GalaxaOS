#include<gui/stringtext.h>
#include<common/graphicscontext.h>
#include <gui/stringtext.h>
using namespace myos::gui;
using namespace myos::common;

StringText::StringText(Widget *parent, int32_t x, int32_t y, uint32_t w, uint32_t h, 
                                    uint8_t r, uint8_t g, uint8_t b, String stringText)
        : Widget(parent, x,y,w,h, r,g,b, 114, "1"),
          thisStringText(stringText) {
}

void StringText::Draw(SuperGraphicsContext *gc) {
    int X=0;
    int Y=0;
    ModelToScreen(X, Y);
    gc->PutString(thisStringText, X, Y, 0, (r << 24) + (g << 16) + (b << 8), this->w);
}