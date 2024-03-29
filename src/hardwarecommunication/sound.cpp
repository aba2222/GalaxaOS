#include "hardwarecommunication/sound.h"
using namespace myos::common;
using namespace myos::hardwarecommunication;

Sounds::Sounds() 
        : p61(0x61),
          p43(0x43),
          p42(0x42) {

}

Sounds::~Sounds() {}

//Play sound using built-in speaker
void Sounds::play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
    //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	p43.Write(0xb6);
 	p42.Write((uint8_t)(Div));
 	p42.Write((uint8_t)(Div >> 8));
 
    //And play the sound using the PC speaker
 	tmp = p61.Read();
  	if (tmp != (tmp | 3)) {
 		p61.Write(tmp | 3);
 	}
 }
 
//make it shut up
/*static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 	outb(0x61, tmp);
}
void beep() {
 	 play_sound(1000);
 	 timer_wait(10);
 	 nosound();
}*/