#include "gui/list.h"
#include "gui/stringtext.h"
#include "gui/scrollbar.h"

using namespace myos::common;
using namespace myos::gui;

List::List(Widget* parent, int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b, uint8_t classNum, String* stringText)
    : CompositeWidget(parent, x, y, w, h, r, g, b, classNum, stringText), scrollbar(new Scrollbar(this, w - 20, 0, 20, h, 200, 200, 200)) {
    AddChild(scrollbar);
}

List::~List() {
    delete scrollbar;
}

void List::AddItem(String* itemText) {
    if (numChildren >= 100) return;
    StringText* item = new StringText(this, 0, numChildren * 20, w, 20, r, g, b, itemText);
    AddChild(item);
}

void List::Draw(SuperGraphicsContext* gc) {
    CompositeWidget::Draw(gc);
}

void List::OnMouseDown(int32_t x, int32_t y, uint8_t button) {
    CompositeWidget::OnMouseDown(x, y, button);
    scrollbar->OnMouseDown(x, y, button);
}