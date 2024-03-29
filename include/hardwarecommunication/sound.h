#ifndef __MYOS__HARDWARECOMMUNICATION__SOUND_H
#define __MYOS__HARDWARECOMMUNICATION__SOUND_H

#include "common/types.h"
#include "hardwarecommunication/port.h"

namespace myos {
    namespace hardwarecommunication {
        class Sounds {
        public:
            Sounds();
            ~Sounds();
            
            void play_sound(common::uint32_t nFrequence);

        private:
            Port8Bit p61;
            Port8Bit p43;
            Port8Bit p42;
        };
    }
}


#endif