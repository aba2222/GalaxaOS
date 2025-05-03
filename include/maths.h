#ifndef __MYOS__GUI__DESKTOP_H
#define __MYOS__GUI__DESKTOP_H

#include "gui/widget.h"
#include "gui/window.h"
#include "drivers/mouse.h"

namespace myos {
    int abs(int value) {
        if(value < 0) {
            value * -1;
        } 
        return value;
    }
    
    int log2(uint32_t x) {
        int result = -1;
        while (x > 0) {
            result++;
            x >>= 1;
        }
        return result;
    }
}

#endif