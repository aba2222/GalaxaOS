#ifndef __MYOS__GUI__WIDGET_H
#define __MYOS__GUI__WIDGET_H

#include "common/types.h"
#include "drivers/svga.h"
#include "common/graphicscontext.h"

namespace myos {
    namespace gui {
        class Widget {
        public:
            Widget(Widget* parent, common::int32_t x, common::int32_t y,
                    common::uint32_t w, common::uint32_t h, common::uint8_t r,
                    common::uint8_t g, common::uint8_t b, common::uint8_t classNum, common::String stringText);
            ~Widget();

            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(common::int32_t &x, common::int32_t &y);

            virtual void Draw(common::SuperGraphicsContext* gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny);

            virtual void OnKeyDown(char str);
            virtual void OnKeyUp(char str);
            
            virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);

            common::int32_t x, y;
            common::uint32_t w, h;
            common::uint8_t r, g, b;
            common::String stringText;
            common::uint8_t classNum; //dektop = 0 window = 1 string = 2
        protected:
            Widget* parent;
            bool Focussable;
        };

        class CompositeWidget : public Widget {
        public:
            CompositeWidget(Widget* parent, common::int32_t x, common::int32_t y,
                    common::uint32_t w, common::uint32_t h, common::uint8_t r,
                    common::uint8_t g, common::uint8_t b, common::uint8_t classNum, common::String stringText);
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);

            virtual void Draw(common::SuperGraphicsContext* gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseMove(common::int32_t ox, common::int32_t oy, common::int32_t nx, common::int32_t ny);

            virtual void OnKeyDown(char str);
            virtual void OnKeyUp(char str);
            
            virtual bool AddChild(Widget* child);
        private:
            Widget* children[100];
            int numChildren;
            Widget* focussedChild;
        };
    }
 }

#endif