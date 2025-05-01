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
            SuperVideoGraphicsArray(uint8_t* screen, uint32_t width, uint32_t height, uint32_t pitch, uint8_t depth, uint32_t pixelwidth);
            ~SuperVideoGraphicsArray();

            void PsfInit();

            void PutPixel(uint32_t x, uint32_t y, uint32_t color);
            void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

            void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
            void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);


            void PutLine(int x0, int y0, int x1, int y1);

            void PutChar(uint8_t c, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg);
            void PutString(const uint8_t* c, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg, uint16_t w);
            void PutString(const common::String c, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg, uint16_t w);

            uint32_t GetWidth() const;
            uint32_t GetHeight() const;

            void Redraw();
        private:
            uint8_t* screen;
            uint32_t width;	//how many pixels you have on a horizontal line
            uint32_t height;	//how many horizontal lines of pixels are present
            uint32_t pitch;	//how many bytes of VRAM you should skip to go one pixel down
            uint8_t depth;	//how many bits of color you have
            uint32_t pixelwidth;//how many bytes of VRAM you should skip to go one pixel right.

            //uint8_t* GetFrameBufferSegment();

            //uint8_t BackBuffer[1280 * 720 * 3];
            uint8_t* BackBuffer;
            uint16_t* unicode;
        };
    }
}

#endif