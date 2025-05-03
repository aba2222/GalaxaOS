#include "gui/textbox.h"
#include <string.h>

using namespace myos::gui;

// 手写strlen实现
static int my_strlen(const char* s) {
    int len = 0;
    while (s && *s++) ++len;
    return len;
}

TextBox::TextBox(CompositeWidget* parent, int32_t x, int32_t y, uint32_t w, uint32_t h,
                 uint8_t r, uint8_t g, uint8_t b, String* text)
    : Widget(parent, x, y, w, h, r, g, b, 3, text), cursorPos(0) {
    if (stringText)
        cursorPos = my_strlen(stringText->c_str());
}

void TextBox::Draw(myos::common::SuperGraphicsContext* gc) {
    int X = 0, Y = 0;
    ModelToScreen(X, Y);
    // 绘制背景
    gc->FillRectangle(X, Y, w, h, r, g, b);
    // 绘制文本
    if (stringText)
        gc->PutString((const uint8_t*)stringText->c_str(), X + 2, Y + 2, 0x00, 0xFFFFFFFF, w - 4);
    // 绘制光标
    int textLen = stringText ? my_strlen(stringText->c_str()) : 0;
    int cursorX = X + 2 + textLen * 8; // 假设每字符宽8像素
    gc->PutLine(cursorX, Y + 2, cursorX, Y + h - 2);
}

void TextBox::OnKeyDown(char str) {
    if (!stringText) return;
    char* buf = (char*)stringText->c_str();
    int len = my_strlen(buf);

    if (str == 8) { // Backspace
        if (cursorPos > 0) {
            // 左移内容
            for (size_t i = cursorPos - 1; i < (size_t)len; ++i) {
                buf[i] = buf[i + 1];
            }
            cursorPos--;
        }
    } else if (str >= 32 && str <= 126) { // 可见字符
        if (len < 127) { // 防止越界
            // 右移内容
            for (int i = len; i >= (int)cursorPos; --i) {
                buf[i + 1] = buf[i];
            }
            buf[cursorPos] = str;
            cursorPos++;
        }
    }
}

void TextBox::OnKeyUp(char str) {
    // 可选实现
}
