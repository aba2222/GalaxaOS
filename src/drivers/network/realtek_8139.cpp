#include "drivers/network/realtek_8139.h"

using namespace myos::drivers;
using namespace myos::common;
using namespace myos::hardwarecommunication;

//  EEPROM_Ctrl bits
#define EE_SHIFT_CLK  0x04  // EEPROM shift clock
#define EE_CS         0x08  // EEPROM chip select
#define EE_DATA_WRITE 0x02  // EEPROM chip data in
#define EE_WRITE_0    0x00
#define EE_WRITE_1    0x02
#define EE_DATA_READ  0x01  // EEPROM chip data out
#define EE_ENB       (0x80 | EE_CS)

// Delay between EEPROM clock transitions.
// No extra delay is needed with 33Mhz PCI, but 66Mhz may change this.

#define eeprom_delay()  inpd(ee_addr)

// The EEPROM commands include the alway-set leading bit

#define EE_WRITE_CMD  (5)
#define EE_READ_CMD   (6)
#define EE_ERASE_CMD  (7)

void printf(const char*, ...);

realtek_8139::realtek_8139(PeripheralComponentInterconnectDeviceDescriptor* dev, InterruptManager* interrupts) 
    : Driver(),
      InterruptHandler(dev->interrupt + interrupts->HardwareInterruptOffset(), interrupts),
      portBase(dev->portBase),
      rxBufPort(dev->portBase + 0x30),
      chipCmdPort(dev->portBase + 0x37),
      intrMaskPort(dev->portBase + 0x3C),
      rxConfigPort(dev->portBase + 0x44),
      eepromPort(dev->portBase + 0x50), 
      config1(dev->portBase + 0x52) {
    config1.Write(0x0);
    int addr_len = ReadEeprom(portBase, 0, 8) == 0x8129 ? 8 : 6;
    for (int i = 0; i < 3; i++) {
        ((unsigned short *)(MAC))[i] = ReadEeprom(portBase, i + 7, addr_len);
    }
    printf("\nMAC address: ");
    for(int i = 0; i < 6; i++) {
        printf("%x:", MAC[i]);
    }
    printf("\n");
}

realtek_8139::~realtek_8139() {}

void realtek_8139::Activate() {
    chipCmdPort.Write(0x10);
    while( (chipCmdPort.Read() & 0x10) != 0) { }
    rxBufPort.Write((uint32_t)&recvBufferDescrMemory);
    intrMaskPort.Write(0x0005);
    rxConfigPort.Write(0xf);
    chipCmdPort.Write(0x0C);
}

const char* realtek_8139::GetDriverName() {
    return "Realtek 8139 Driver";
}

int realtek_8139::ReadEeprom(long ioaddr, int location, int addr_len) {
    int i;
    uint32_t retval = 0;
    int read_cmd = location | (EE_READ_CMD << addr_len);
  
    eepromPort.Write(EE_ENB & ~EE_CS);
    eepromPort.Write(EE_ENB);
  
    // Shift the read command bits out
    for (i = 4 + addr_len; i >= 0; i--) {
        int dataval = (read_cmd & (1 << i)) ? EE_DATA_WRITE : 0;
        eepromPort.Write(EE_ENB | dataval);
        //eeprom_delay();
        eepromPort.Write(EE_ENB | dataval | EE_SHIFT_CLK);
        //eeprom_delay();
    }
    eepromPort.Write(EE_ENB);
    //eeprom_delay();
  
    for (i = 16; i > 0; i--) {
        eepromPort.Write(EE_ENB | EE_SHIFT_CLK);
        //eeprom_delay();
        retval = (retval << 1) | ((eepromPort.Read() & EE_DATA_READ) ? 1 : 0);
        eepromPort.Write(EE_ENB);
        //eeprom_delay();
    }
  
    // Terminate the EEPROM access
    eepromPort.Write(~EE_CS);
    return retval;
}

uint32_t realtek_8139::HandleInterrupt(uint32_t esp) {
}