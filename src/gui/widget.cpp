#include "gui/widget.h"

using namespace myos::common;
using namespace myos::gui;


Widget::Widget(Widget* parent, int32_t x, int32_t y,
        uint32_t w, uint32_t h, uint8_t r,
        uint8_t g, uint8_t b) 
        : parent(parent),
          x(x),
          y(y),
          w(w),
          h(h),
          r(r),
          g(g),
          b(b) { }

void Widget::GetFocus(Widget* widget) {
    if (parent != 0) parent->GetFocus(widget);
}

        /*class Widget {
        public:
            virtual ~Widget();

            
            virtual void ModelToScreen(int32_t &x, int32_t &y);

            virtual void Draw(GraphicsContext gc);
            virtual void OnMouseDown(int32_t x, uint32_t y);
            virtual void OnMouseUp(int32_t x, int32_t y);
            virtual void OnMouseMove(int32_t x, int32_t y, int32_t nx, int32_t ny);

            virtual void OnKeyDown(int32_t x, int32_t y);
            virtual void OnKeyUp(int32_t x, int32_t y);

        private:
            Widget* parent;
            uint32_t x, y, w, h;
            uint8_t r, g, b;
            bool Focussable;
        };

        class CompositeWidget : public Widget {
        public:
            CompositeWidget(Widget* parent, int32_t x, :int32_t y,
                    uint32_t w, uint32_t h, uint8_t r,
                    uint8_t g, uint8_t b);
            virtual ~CompositeWidget();

            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(int32_t &x, int32_t &y);

            virtual void Draw(GraphicsContext gc);
            virtual void OnMouseDown(int32_t x, int32_t y);
            virtual void OnMouseUp(int32_t x, int32_t y);
            virtual void OnMouseMove(int32_t x, int32_t y, int32_t nx, int32_t ny);

            virtual void OnKeyDown(int32_t x, int32_t y);
            virtual void OnKeyUp(int32_t x, int32_t y);

        private:
            Widget* children[100];
            int numChildren;
            Widget* foucssedChild;
        };*/