#ifndef __MYOS__HARDWARECOMMUNICATION__MMIO_H
#define __MYOS__HARDWARECOMMUNICATION__MMIO_H

#include "common/types.h"

namespace myos {
    namespace hardwarecommunication {
        class MMIOUtils {
            public:
                static common::uint8_t read8 (common::uint64_t p_address);
                static common::uint16_t read16 (common::uint64_t p_address);
                static common::uint32_t read32 (common::uint64_t p_address);
                static common::uint64_t read64 (common::uint64_t p_address);
                static void write8 (common::uint64_t p_address, common::uint8_t p_value);
                static void write16 (common::uint64_t p_address, common::uint16_t p_value);
                static void write32 (common::uint64_t p_address, common::uint32_t p_value);
                static void write64 (common::uint64_t p_address, common::uint64_t p_value);
        };
    }
}

#endif