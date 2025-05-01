#ifndef __MYOS__GUI__WIDGET_H
#define __MYOS__GUI__WIDGET_H

#include "common/types.h"
#include "drivers/svga.h"
#include "common/graphicscontext.h"

namespace myos {
    namespace gui {
        class Widget {
        public:
            Widget(Widget* parent, int32_t x, int32_t y,
                    uint32_t w, uint32_t h, uint8_t r,
                    uint8_t g, uint8_t b, uint8_t classNum, common::String* stringText);
            ~Widget();

            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(int32_t &x, int32_t &y);

            virtual void Draw(common::SuperGraphicsContext* gc);
            virtual void OnMouseDown(int32_t x, int32_t y, uint8_t button);
            virtual void OnMouseUp(int32_t x, int32_t y, uint8_t button);
            virtual void OnMouseMove(int32_t x, int32_t y, int32_t nx, int32_t ny);

            virtual void OnKeyDown(char str);
            virtual void OnKeyUp(char str);
            
            virtual bool ContainsCoordinate(int32_t x, int32_t y);

            virtual void Animate();

            int32_t x, y;
            uint32_t w, h;
            uint8_t r, g, b;
            common::String* stringText;
            uint8_t classNum; //dektop = 0 window = 1 string = 2
        protected:
            Widget* parent;
            bool Focussable;
        };

        class CompositeWidget : public Widget {
        public:
            CompositeWidget(Widget* parent, int32_t x, int32_t y,
                    uint32_t w, uint32_t h, uint8_t r,
                    uint8_t g, uint8_t b, uint8_t classNum, common::String* stringText);
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);

            virtual void Draw(common::SuperGraphicsContext* gc);
            virtual void OnMouseDown(int32_t x, int32_t y, uint8_t button);
            virtual void OnMouseUp(int32_t x, int32_t y, uint8_t button);
            virtual void OnMouseMove(int32_t ox, int32_t oy, int32_t nx, int32_t ny);

            virtual void OnKeyDown(char str);
            virtual void OnKeyUp(char str);
            
            virtual bool AddChild(Widget* child);
        protected:
            int numChildren;
            Widget* children[100];
        private:
            Widget* focussedChild;
        };
    }
 }

#endif