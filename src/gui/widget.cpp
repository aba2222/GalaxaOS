#include "gui/widget.h"

using namespace myos::common;
using namespace myos::gui;


Widget::Widget(Widget* parent, int32_t x, int32_t y,
        uint32_t w, uint32_t h, uint8_t r,
        uint8_t g, uint8_t b, uint8_t classNum, uint8_t* stringText) 
        : parent(parent),
          x(x),
          y(y),
          w(w),
          h(h),
          r(r),
          g(g),
          b(b),
          Focussable(true),
          classNum(classNum),
          stringText(stringText) { }

Widget::~Widget() {}

void Widget::GetFocus(Widget* widget) {
    if (parent != 0) parent->GetFocus(widget);
}

void Widget::ModelToScreen(int32_t &toX, int32_t &toY) {
    if(parent != 0) parent->ModelToScreen(toX, toY);

    toX += this->x;
    toY += this->y;
}

void Widget::Draw(SuperGraphicsContext* gc) {
    int X=0;
    int Y=0;
    ModelToScreen(X, Y);
    switch (this->classNum) {
        case 0:
            gc->FillRectangle(X, Y, w, h, r, g, b);
            break;
        case 1:
            gc->FillRectangle(X, Y, w, h, r, g, b);

            gc->FillRectangle(X, Y, w, 20, 128, 128, 128);
            gc->PutString((uint8_t*)this->stringText, X, Y, 0, (128 << 24) + (128 << 16) + (128 << 8), this->w);

            gc->PutLine(X, Y, X + w, Y);
            gc->PutLine(X, Y + h, X + w, Y + h);
            gc->PutLine(X, Y, X, Y + h);
            gc->PutLine(X + w, Y, X + w, Y + h);
            break;
        case 2:
            gc->PutString((uint8_t*)this->stringText, X, Y, (r << 24) + (g << 16) + (b << 8), (parent->r << 24) + (parent->g << 16) + (parent->b << 8), this->w);
            break;
        default:
            break;
    }
}

void Widget::OnMouseDown(int32_t x, int32_t y, uint8_t button) {
    //if (Focussable) {
    //  GetFocus(this);
    //}
}
void Widget::OnMouseUp(int32_t x, int32_t y, uint8_t button) {}
void Widget::OnMouseMove(int32_t x, int32_t y, int32_t nx, int32_t ny) {}
void Widget::OnKeyDown(char str) {}
void Widget::OnKeyUp(char str) {}


bool Widget::ContainsCoordinate(int32_t x, int32_t y) {
    return this->x <= x && x < this->x + this->w
        && this->y <= y && y < this->y + this->h;
}


CompositeWidget::CompositeWidget(Widget* parent, int32_t x, int32_t y,
            uint32_t w, uint32_t h, uint8_t r,
            uint8_t g, uint8_t b, uint8_t classNum, uint8_t* stringText) 
            : Widget(parent, x, y, w, h, r, g, b, classNum, stringText),
              focussedChild(0),
              numChildren(0) {
}
CompositeWidget::~CompositeWidget() {}

void CompositeWidget::GetFocus(Widget* widget) {
    this->focussedChild = widget;
    if(parent != 0) {
      parent->GetFocus(this);
    }
}

bool CompositeWidget::AddChild(Widget* child) {
    if(numChildren >= 100)
        return false;
    children[numChildren++] = child;
    return true;
}

//void CompositeWidget::ModelToScreen(int32_t &x, int32_t &y) {}

void CompositeWidget::Draw(SuperGraphicsContext* gc) {
    Widget::Draw(gc);
    for(int i = numChildren-1; i >= 0; --i) {
        children[i]->Draw(gc);
    }
}

void CompositeWidget::OnMouseDown(int32_t x, int32_t y, common::uint8_t button) {
    for(int i = 0; i < numChildren; ++i) {
        if(children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
            children[i]->OnMouseDown(x - this->x, y - this->y, button);
            this->focussedChild = children[i];
            break;
        }
    }
}

void CompositeWidget::OnMouseUp(int32_t x, int32_t y, common::uint8_t button) {
    for(int i = 0; i < numChildren; ++i) {
        //if(children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
            children[i]->OnMouseUp(x - this->x, y - this->y, button);
            //break;
        //}
    }
}

void CompositeWidget::OnMouseMove(int32_t ox, int32_t oy, int32_t nx, int32_t ny) {
    int firstchild = -1;
    for(int i = 0; i < numChildren; ++i) {
        if(children[i]->ContainsCoordinate(ox - this->x, oy - this->y)) {
            children[i]->OnMouseMove(ox - this->x, oy - this->y, nx - this->x, ny - this->y);
            firstchild = i;
            break;
        }
    }

    //why???
    for(int i = 0; i < numChildren; ++i) {
        if(children[i]->ContainsCoordinate(nx - this->x, ny - this->y)) {
            if(firstchild != i) {
                children[i]->OnMouseMove(ox - this->x, oy - this->y, nx - this->x, ny - this->y);
            }
            break;
        }
    }     
}

void CompositeWidget::OnKeyDown(char str) {
    //Widget::stringText[0] = (uint8_t)str; 显示按下的字符
    if(this->focussedChild != 0) focussedChild->OnKeyDown(str);
}

void CompositeWidget::OnKeyUp(char str) {
    if(this->focussedChild != 0) focussedChild->OnKeyUp(str);
}