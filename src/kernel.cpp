#include "common/types.h"
#include "gdt.h"
#include "memorymanager.h"
#include "syscalls.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/pci.h"
#include "hardwarecommunication/multitasking.h"
#include "hardwarecommunication/times.h"
#include "hardwarecommunication/sound.h"
#include "drivers/driver.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/vga.h"
#include "drivers/svga.h"
#include "gui/desktop.h"
#include "gui/stringtext.h"
#include "drivers/amd_am79c973.h"
#include "drivers/ata.h"
#include "filesystem/dospart.h"
#include "filesystem/fat.h"

using namespace myos;
using namespace myos::common;
using namespace myos::gui;
using namespace myos::drivers;
using namespace myos::filesystem;
using namespace myos::hardwarecommunication;

//#define GMODE1
#define GMODE2

static String shellText = "";

void printf(const char* format, ...) {
    char buffer[256];
    int i = 0, j = 0;
    int* args = (int*)&format + 1;

    while (format[i]) {
        if (format[i] == '%' && format[i + 1]) {
            ++i;
            switch (format[i]) {
                case 'c': {
                    buffer[j++] = (char)args[0];
                    args++;
                    break;
                }
                case 's': {
                    const char* s = (const char*)args[0];
                    args++;
                    while (*s) {
                        buffer[j++] = *s++;
                    }
                    break;
                }
                case 'd': {
                    int d = args[0];
                    args++;
                    char numStr[10];
                    int k = 0;
                    if (d < 0) {
                        buffer[j++] = '-';
                        d = -d;
                    }
                    do {
                        numStr[k++] = (d % 10) + '0';
                        d /= 10;
                    } while (d);
                    while (k > 0) {
                        buffer[j++] = numStr[--k];
                    }
                    break;
                }
                default:
                    buffer[j++] = format[i];
                    break;
            }
        } else {
            buffer[j++] = format[i];
        }
        ++i;
    }
    buffer[j] = '\0';

    shellText = shellText + buffer;
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

void SysCallPrint(const char* str) {
    asm("int $0x80" : : "a" (4), "b" (str));
}

//more task
void TaskA() {
    while (1) {
        //shell1.ShellPrintf((const char*)"A");
        SysCallPrint((const char*)"A");
    }
}

void TaskB() {
    while (1) {
        SysCallPrint((const char*)"B");
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

extern "C" void kernelMain(multiboot_info_t* multiboot_structure, uint32_t magicnumber){
    GlobalDescriptorTable gdt;

    TaskManager taskmanager;
    //Task task1(&gdt, TaskA);
    //Task task2(&gdt, TaskB);
    //taskmanager.AddTask(&task1);
    //taskmanager.AddTask(&task2);

    //mmanager
    size_t heap = 10 * 1024 * 1024;
    uint32_t* memupper = (uint32_t*)((size_t)multiboot_structure + 8);

    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("magicnumber: %d ", magicnumber);
    printf("vbe_mode: %d ", multiboot_structure->vbe_mode);
    printf("vbe_mode_info->width: %d\n", multiboot_structure->vbe_mode_info);
    printf("memupper: %d ", *memupper);
    printf("heap: %d", heap);

    printf("\n-----init1-----\n");

    InterruptManager interrupts(0x20, &gdt, &taskmanager);
    nowIntManagerM = &interrupts;
    SysCallHandler syscalls(&interrupts, 0x80);

    DriverManger drvManger;

    #ifdef GMODE1
        VideoGraphicsArray vga;
    #endif
    #ifdef GMODE2
        uint32_t pixelwidth = multiboot_structure->framebuffer_bpp / 8;
        SuperVideoGraphicsArray svga((uint8_t*)multiboot_structure->framebuffer_addr, multiboot_structure->framebuffer_width, 
                                      multiboot_structure->framebuffer_height, multiboot_structure->framebuffer_pitch,
                                      multiboot_structure->framebuffer_bpp, pixelwidth);
    #endif

    #ifdef GMODE1
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    #ifdef GMODE2
        Desktop desktop(multiboot_structure->framebuffer_width, multiboot_structure->framebuffer_height, 0x87, 0xCE, 0xEB, &svga, &shellText);
    #endif

    #ifdef GMODE1
        KeyBoardDriver keyboard(&interrupts, &desktop);
        MouseDriver mouse(&interrupts, &desktop);
    #else
        #ifdef GMODE2
            KeyBoardDriver keyboard(&interrupts, &desktop);
            MouseDriver mouse(&interrupts, &desktop);
        #else
            PrintKeyboardEventHandler kbhandler;
            MouseToConsole mousehandler;
            KeyBoardDriver keyboard(&interrupts, &kbhandler);
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
    #endif
    drvManger.AddDriver(&keyboard);
    drvManger.AddDriver(&mouse);
    
    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManger, &interrupts);

    Times timeControl(0x70, 0x71);

    drvManger.ActivateAll(); 

    //amd_am79c973* eth0 = (amd_am79c973*)(drvManger.drivers[2]);
    //eth0->Send((uint8_t*)"Hello Network", 13);

    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(true, 0x1F0);
    ata0m.Identify();
    
    printf("   S-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(false, 0x1F0);
    ata0s.Identify();

    PartitionManger partManger;
    DOSPartitonTable::ReadPartitionTable(&ata0s, &partManger);
    FatPartition* part1 = partManger.GetPartitionList(1);
    DirectoriesFat32 part1Dirent[32] = {0};
    part1->GetFatFileList((DirectoriesFat32*)&part1Dirent);
    char* filename = part1->ReadFileName(part1Dirent[1]);
    part1->ReadTxtFile(part1Dirent[1]);
    //AdvancedTechnologyAttachment ata1m(true, 0x170);
    //ata1m.Identify();
    //AdvancedTechnologyAttachment ata1s(false, 0x170);
    //ata1s.Identify();
    // third: 0x1E8
    // fourth: 0x168

    printf("\n");

    interrupts.Activate();

    Sounds soundManager;
    //soundManager.play_sound(10);

    printf("\n-----Init Done-----\n");
    
    #ifdef GMODE1
        vga.SetMode(320, 200, 8);
        Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif
    #ifdef GMODE2
        String toolName1 = "tool";
        CompositeWidget tool1(&desktop, 0, 0, multiboot_structure->framebuffer_width, 30, 128, 128, 128, 0, &toolName1);
        desktop.AddChild(&tool1);
        String timeString = "xx/xx xx:xx:xx";
        StringText timeStringText(&tool1, 10, 3, 330, 200, 0xFF, 0xFF, 0xFF, &timeString);
        tool1.AddChild(&timeStringText);

        String windowsName1 = "Window 1";
        Window win1(&desktop, 114, 230, 350, 230, 0xFF, 0x00, 0x00, &windowsName1);
        desktop.AddChild(&win1);
        String windowsName2 = "Window 2";
        Window win2(&desktop, 568, 230, 200, 100, 0x00, 0xAA, 0x00, &windowsName2);
        desktop.AddChild(&win2);
    #endif

    while(1) {
        #ifdef GMODE2
        desktop.Draw();
        
        timeControl.ReadRtc();
        timeString[0] = timeControl.month/10 + 48;
        timeString[1] = timeControl.month%10 + 48;

        timeString[3] = timeControl.day/10 + 48;
        timeString[4] = timeControl.day%10 + 48;

        timeString[6] = timeControl.hour/10 + 48;
        timeString[7] = timeControl.hour%10 + 48;

        timeString[9] = timeControl.minute/10 + 48;
        timeString[10] = timeControl.minute%10 + 48;

        timeString[12] = timeControl.second/10 + 48;
        timeString[13] = timeControl.second%10 + 48;
        #endif
    }
}