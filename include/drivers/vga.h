#ifndef __MYOS__DRIVERS__VGA_H
#define __MYOS__DRIVERS__VGA_H

#include <stdint.h>
#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"
#include "memorymanager.h"

namespace myos {  
    namespace drivers {
        class VideoGraphicsArray {
        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);
            bool SetMode(uint32_t width, uint32_t height, uint32_t colordepth);
            void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
            void PutPixel(uint32_t x, uint32_t y, uint8_t colorindex);

            void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r,
                                uint8_t g, uint8_t b);

            void Redraw();
        private:
            hardwarecommunication::Port8Bit miscPort;
            hardwarecommunication::Port8Bit crtcIndexPort;
            hardwarecommunication::Port8Bit crtcDataPort;
            hardwarecommunication::Port8Bit sequencerIndexPort;
            hardwarecommunication::Port8Bit sequencerDataPort;
            hardwarecommunication::Port8Bit graphicsControllerIndexPort;
            hardwarecommunication::Port8Bit graphicsControllerDataPort;
            hardwarecommunication::Port8Bit attributeControllerIndexPort;
            hardwarecommunication::Port8Bit attributeControllerReadPort;
            hardwarecommunication::Port8Bit attributeControllerWritePort;
            hardwarecommunication::Port8Bit attributeControllerResetPort;


            void WriteRegisters(uint8_t* registers);
            uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);
            uint8_t* GetFrameBufferSegment();

            uint8_t videoBuffer[320 * 200 * 8];
        };
    }
}

#endif