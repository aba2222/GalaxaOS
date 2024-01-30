#ifndef __MYOS__GUI__WIDGET_H
#define __MYOS__GUI__WIDGET_H

#include "common/types.h"
#include "common/graphicscontext.h"

namespace myos {
    namespace gui {
        class Widget {
        public:
            Widget(Widget* parent, common::int32_t x, common::int32_t y,
                    common::uint32_t w, common::uint32_t h, common::uint8_t r,
                    common::uint8_t g, common::uint8_t b);
            virtual ~Widget();

            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(common::int32_t &x, common::int32_t &y);

            virtual void Draw(common::GraphicsContext gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y);
            virtual void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny);

            virtual void OnKeyDown(common::int32_t x, common::int32_t y);
            virtual void OnKeyUp(common::int32_t x, common::int32_t y);

        private:
            Widget* parent;
            common::uint32_t x, y, w, h;
            common::uint8_t r, g, b;
            bool Focussable;
        };

        class CompositeWidget : public Widget {
        public:
            CompositeWidget(Widget* parent, common::int32_t x, common::int32_t y,
                    common::uint32_t w, common::uint32_t h, common::uint8_t r,
                    common::uint8_t g, common::uint8_t b);
            virtual ~CompositeWidget();

            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(common::int32_t &x, common::int32_t &y);

            virtual void Draw(common::GraphicsContext gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y);
            virtual void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny);

            virtual void OnKeyDown(common::int32_t x, common::int32_t y);
            virtual void OnKeyUp(common::int32_t x, common::int32_t y);

        private:
            Widget* children[100];
            int numChildren;
            Widget* foucssedChild;
        };
    }
 }

#endif