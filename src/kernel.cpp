#include "common/types.h"
#include "gdt.h"
#include "multitasking.h"
#include "memorymanager.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/pci.h"
#include "drivers/driver.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/vga.h"
#include "gui/desktop.h"

using namespace myos;
using namespace myos::common;
using namespace myos::gui;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

#define GMODE

void printf(const char* str){
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;
    for(int i = 0; str[i]; i++){
        switch(str[i]) {
        case '\n':
            y++;
            x = 0;
            break;
        default:   
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }


        if(x >= 80) {
            x=0;
            y++;
        }

        if (y >=25) {
            for (y = 0; y < 25; y++) {
                for(x = 0; x < 80;x++) {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                }
            }
            x = 0, y = 0;
        }
    }
}

void printfHex(uint8_t key) {
    char* foo = (char*)"00";
    const char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0f];
    foo[1] = hex[key & 0x0f];
    printf((const char*)foo);
}

class PrintKeyboardEventHandler : public KeyBoardEventHandler {
public:
    void OnKeyDown(char c) {
        char* foo = (char*)" ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler {
public:
    MouseToConsole()
        : x(40),
          y(12) {
     }

    void OnActivate() {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
    }
    
    void OnMouseMove(int8_t nx, int8_t ny) {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0xf00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);

        x += nx;
        if(x < 0) x = 0;
        else if (x >= 80) x = 79;

        y += ny;
        if (y < 0) y = 0;
        else if (y >= 25) y = 24;

        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0xf00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
    }
private:
    int8_t x, y;
};

//more task
void TaskA() {
    while (1) {
        printf("A");
    }
}

void TaskB() {
    while (1) {
        printf("B");
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors(){
    for(constructor* i = &start_ctors; i != &end_ctors; i++){
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){
    GlobalDescriptorTable gdt;

    TaskManager taskmanager;
    //Task task1(&gdt, TaskA);
    //Task task2(&gdt, TaskB);
    //taskmanager.AddTask(&task1);
    //taskmanager.AddTask(&task2);

    //mmanager
    size_t heap = 10 * 1024 * 1024;
    uint32_t* memupper = (uint32_t*)((size_t)multiboot_structure + 8);
    printf("memupper: 0x");
    printfHex(((*memupper) >> 24) & 0xff);
    printfHex(((*memupper) >> 16) & 0xff);
    printfHex(((*memupper) >> 8) & 0xff);
    printfHex(((*memupper) >> 0) & 0xff);

    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("\nheap: 0x");
    printfHex((heap >> 24) & 0xff);
    printfHex((heap >> 16) & 0xff);
    printfHex((heap >> 8) & 0xff);
    printfHex((heap >> 0) & 0xff);

    printf("\n\n");

    InterruptManager interrupts(0x20, &gdt, &taskmanager);

    #ifdef GMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif

    DriverManger drvManger;
    PrintKeyboardEventHandler kbhandler;
    KeyBoardDriver keyboard(&interrupts, &kbhandler);
    drvManger.AddDriver(&keyboard);
    
    #ifdef GMODE
        MouseDriver mouse(&interrupts, &desktop);
    #else
        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
    #endif
    drvManger.AddDriver(&mouse);
    
    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManger, &interrupts);

    #ifdef GMODE  
        VideoGraphicsArray vga;
    #endif

    drvManger.ActivateAll(); 

    interrupts.Activate();
    
    #ifdef GMODE
        vga.SetMode(320, 200, 8);
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif

    while(1) {
        #ifdef GMODE
            desktop.Draw(&vga);
        #endif
    }
}