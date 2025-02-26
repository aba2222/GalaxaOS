#ifndef __MYOS__COMMON__MATH_H
#define __MYOS__COMMON__MATH_H

#include "common/types.h"

namespace myos {
    namespace common {
        int log2(uint32_t x) {
            int result = -1;
            while (x > 0) {
                result++;
                x >>= 1;
            }
            return result;
        }
    }
}

#endif