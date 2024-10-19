#ifndef __MYOS__DRIVERS__SVGA_H
#define __MYOS__DRIVERS__SVGA_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "memorymanager.h"
#include "gui/psf.h"

namespace myos {  
    namespace drivers {
        class SuperVideoGraphicsArray {
        public:
            SuperVideoGraphicsArray(common::uint8_t* screen, common::uint32_t width, common::uint32_t height, common::uint32_t pitch, common::uint8_t depth, common::uint32_t pixelwidth);
            ~SuperVideoGraphicsArray();

            void PsfInit();

            void PutPixel(common::uint32_t x, common::uint32_t y, common::uint32_t color);
            void PutPixel(common::uint32_t x, common::uint32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b);

            void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h, common::uint32_t color);
            void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);


            void PutLine(int x0, int y0, int x1, int y1);

            void PutChar(common::uint8_t c, common::uint32_t cx, common::uint32_t cy, common::uint32_t fg, common::uint32_t bg);
            void PutString(const common::uint8_t* c, common::uint32_t cx, common::uint32_t cy, common::uint32_t fg, common::uint32_t bg, common::uint16_t w);
            void PutString(const common::String c, common::uint32_t cx, common::uint32_t cy, common::uint32_t fg, common::uint32_t bg, common::uint16_t w);

            common::uint32_t GetWidth() const;
            common::uint32_t GetHeight() const;

            void Redraw();
        private:
            common::uint8_t* screen;
            common::uint32_t width;	//how many pixels you have on a horizontal line
            common::uint32_t height;	//how many horizontal lines of pixels are present
            common::uint32_t pitch;	//how many bytes of VRAM you should skip to go one pixel down
            common::uint8_t depth;	//how many bits of color you have
            common::uint32_t pixelwidth;//how many bytes of VRAM you should skip to go one pixel right.

            //common::uint8_t* GetFrameBufferSegment();

            //common::uint8_t BackBuffer[1280 * 720 * 3];
            common::uint8_t* BackBuffer;
            common::uint16_t* unicode;
        };
    }
}

#endif