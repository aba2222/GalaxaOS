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
#include "drivers/serialport.h"
#include "gui/desktop.h"
#include "gui/stringtext.h"
#include "gui/window.h"
#include "gui/button.h"
#include "gui/list.h"
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
                case 'x': {
                    char* hex = "0123456789ABCDEF";
                    int n = args[0];
                    args++;
                    for (int k = sizeof(&args[0]); k >= 0; k -= 4) {
                        buffer[j++] = hex[(n >> k) & 0xF];
                    }
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

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors(){
    for(constructor* i = &start_ctors; i != &end_ctors; i++){
        (*i)();
    }
}

Desktop* globalDesktop;
Times* globalTimeControl;
String timeString = "xx/xx xx:xx:xx";
void RedrawDesktop() {
    while (true) {
        globalDesktop->Draw();
    }
}

void TimeLoop() {
    while (true) {   
        globalTimeControl->ReadRtc();
        timeString[0] = globalTimeControl->month/10 + 48;
        timeString[1] = globalTimeControl->month%10 + 48;
    
        timeString[3] = globalTimeControl->day/10 + 48;
        timeString[4] = globalTimeControl->day%10 + 48;
    
        timeString[6] = globalTimeControl->hour/10 + 48;
        timeString[7] = globalTimeControl->hour%10 + 48;
    
        timeString[9] = globalTimeControl->minute/10 + 48;
        timeString[10] = globalTimeControl->minute%10 + 48;
    
        timeString[12] = globalTimeControl->second/10 + 48;
        timeString[13] = globalTimeControl->second%10 + 48;
    }
}

void TaskA() {
    printf("A");
}

extern "C" void kernelMain(multiboot_info_t* multiboot_structure, uint32_t magicnumber){
    GlobalDescriptorTable gdt;

    TaskManager taskmanager;
    //mmanager
    size_t heap = 10 * 1024 * 1024;
    uint32_t* memupper = (uint32_t*)((size_t)multiboot_structure + 8);

    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("GalaxaOS 0.0.1\n");
    printf("vbe_mode_info->width: %d\n", multiboot_structure->vbe_mode_info->width);
    printf("vbe_mode_info->height: %d\n", multiboot_structure->vbe_mode_info->height);
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

    globalDesktop = &desktop;

    #ifdef GMODE1
        KeyBoardDriver keyboard(&interrupts, &desktop);
        MouseDriver mouse(&interrupts, &desktop);
    #endif
    #ifdef GMODE2
        KeyBoardDriver keyboard(&interrupts, &desktop);
        MouseDriver mouse(&interrupts, &desktop);
    #endif
    drvManger.AddDriver(&keyboard);
    drvManger.AddDriver(&mouse);
    
    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManger, &interrupts);

    Times timeControl(0x70, 0x71);
    globalTimeControl = &timeControl;
    SerialPort serialPort(COM1);
    drvManger.ActivateAll(); 

    //amd_am79c973* eth0 = (amd_am79c973*)(drvManger.drivers[2]);
    //eth0->Send((uint8_t*)"Hello Network", 13);

    /*printf("\nS-ATA primary master: ");
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
    part1->ReadTxtFile(part1Dirent[1]);*/
    
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
        StringText timeStringText(&tool1, 10, 3, 330, 200, 0xFF, 0xFF, 0xFF, &timeString);
        tool1.AddChild(&timeStringText);

        String windowsName1 = "Window 1";
        Window win1(&desktop, 114, 500, 350, 230, 0xFF, 0x00, 0x00, &windowsName1);
        List lit1(&win1, 0, 20, 350, 210, 0xFF, 0x00, 0x00, 0, new String("List 1"));
        for(int driverIndex = 0; driverIndex < drvManger.numDrivers; driverIndex++) {
            lit1.AddItem(new String(drvManger.drivers[driverIndex]->GetDriverName()));
        }
        win1.AddChild(&lit1);
        desktop.AddChild(&win1);

        String windowsName2 = "Window 2";
        Window win2(&desktop, 568, 500, 200, 100, 0x00, 0xAA, 0x00, &windowsName2);
        Button but1(&win2, 20, 30, 80, 30, 0xA8, 0xA8, 0xA8, new String("Button 1"));
        but1.SetOnClick(&TaskA);
        win2.AddChild(&but1);
        desktop.AddChild(&win2);
    #endif
    serialPort.WriteSerial('G');serialPort.WriteSerial('a'); serialPort.WriteSerial('l');
    serialPort.WriteSerial('a');serialPort.WriteSerial('x');serialPort.WriteSerial('a');
    serialPort.WriteSerial('O');serialPort.WriteSerial('S');

    Task redrawTask(&gdt, RedrawDesktop);
    taskmanager.AddTask(&redrawTask);
    Task timeLoopTask(&gdt, TimeLoop);
    taskmanager.AddTask(&timeLoopTask);
    while (true) {
    }
}