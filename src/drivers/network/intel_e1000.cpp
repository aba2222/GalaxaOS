#include <drivers/network/intel_e1000.h>

using namespace myos::drivers;
using namespace myos::common;
using namespace myos::hardwarecommunication;

void printf(const char*, ...);

intel_e1000::intel_e1000(PeripheralComponentInterconnectDeviceDescriptor* dev) 
    : Driver(),
    eerpromExists(false),
    bar_type(dev->bar0_type),
    dataPort(dev->portBase & ~1),
    mem_base(dev->bar0_adress & ~3 + 10 * 1024 * 1024) {

}

intel_e1000::~intel_e1000() {

}

void intel_e1000::WriteCommand(uint16_t p_address, uint32_t p_value) {
    if (bar_type == 0) {
        MMIOUtils::write32(mem_base + p_address,p_value);
    }
    else {
        dataPort.Write(p_address);
        dataPort.Write(p_value, 4);
    }
}

uint32_t intel_e1000::ReadCommand(uint16_t p_address) {
    if (bar_type == 0) {
        return MMIOUtils::read32(mem_base + p_address);
    }
    else {
        dataPort.Write(p_address);
        return dataPort.Read(4);
    }
}

bool intel_e1000::detectEEProm() {
    uint32_t val = 0;
    WriteCommand(REG_EEPROM, 0x1); 

    for(int i = 0; i < 1000 && ! eerpromExists; i++) {
            val = ReadCommand(REG_EEPROM);
            if(val & 0x10)
            eerpromExists = true;
            else
            eerpromExists = false;
    }

    return eerpromExists;
}

uint32_t intel_e1000::eepromRead(uint8_t addr) {
	uint16_t data = 0;
	uint32_t tmp = 0;
    if (eerpromExists) {
        WriteCommand( REG_EEPROM, (1) | ((uint32_t)(addr) << 8) );
        while( !((tmp = ReadCommand(REG_EEPROM)) & (1 << 4)) );
    }
    else{
        WriteCommand( REG_EEPROM, (1) | ((uint32_t)(addr) << 2) );
        while( !((tmp = ReadCommand(REG_EEPROM)) & (1 << 1)) );
    }
	data = (uint16_t)((tmp >> 16) & 0xFFFF);
	return data;
}

bool intel_e1000::ReadMACAddress() {
    if (eerpromExists) {
        uint32_t temp;
        temp = eepromRead( 0);
        mac[0] = temp &0xff;
        mac[1] = temp >> 8;
        temp = eepromRead( 1);
        mac[2] = temp &0xff;
        mac[3] = temp >> 8;
        temp = eepromRead( 2);
        mac[4] = temp &0xff;
        mac[5] = temp >> 8;
    }
    else {
        uint8_t * mem_base_mac_8 = (uint8_t *) (mem_base+0x5400);
        uint32_t * mem_base_mac_32 = (uint32_t *) (mem_base+0x5400);
        if ( mem_base_mac_32[0] != 0 ) {
            for(int i = 0; i < 6; i++) {
                mac[i] = mem_base_mac_8[i];
            }
        }
        else return false;
    }
    return true;
}

void intel_e1000::Activate() {
    detectEEProm();
    ReadMACAddress();
    //printMac();
    //startLink();
    
    /*for(int i = 0; i < 0x80; i++) {
        writeCommand(0x5200 + i*4, 0);
    }
    if ( interruptManager->registerInterrupt(IRQ0+pciConfigHeader->getIntLine(),this)) {
        enableInterrupt();
        rxinit();
        txinit();
    }*/
}

const char* intel_e1000::GetDriverName() {
    return "Intel E1000 Network Driver";
}