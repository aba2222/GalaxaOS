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
}

#endif