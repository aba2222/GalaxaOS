#ifndef __MYOS__COMMON__GRSPHICSCONTEXT_H
#define __MYOS__COMMON__GRSPHICSCONTEXT_H

#include "drivers/vga.h"
#include "drivers/svga.h"

namespace myos {
    namespace common {
        typedef myos::drivers::VideoGraphicsArray GraphicsContext;
        typedef myos::drivers::SuperVideoGraphicsArray SuperGraphicsContext;
    }
}

#endif