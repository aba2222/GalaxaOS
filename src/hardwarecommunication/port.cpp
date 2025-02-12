#include "hardwarecommunication/port.h"

using namespace myos::common;
using namespace myos::hardwarecommunication;

Port::Port(uint16_t portnumber)
    : portnumber(portnumber) {}

Port::~Port() {}

Port8Bit::Port8Bit(uint16_t portnumber)
    : Port(portnumber) {}

Port8Bit::~Port8Bit() {}

void Port8Bit::Write(uint8_t data, uint8_t offset) {
    Write8(portnumber + offset, data);
}

uint8_t Port8Bit::Read(uint8_t offset) {
    return Read8(portnumber + offset);
}

Port8BitSlow::Port8BitSlow(uint16_t portnumber)
    : Port8Bit(portnumber) {}

Port8BitSlow::~Port8BitSlow() {}

void Port8BitSlow::Write(uint8_t data, uint8_t offset) {
    Write8Slow(portnumber + offset, data);
}

Port16Bit::Port16Bit(uint16_t portnumber)
    : Port(portnumber) {}

Port16Bit::~Port16Bit() {}

void Port16Bit::Write(uint16_t data, uint8_t offset) {
    Write16(portnumber + offset, data);
}

uint16_t Port16Bit::Read(uint8_t offset) {
    return Read16(portnumber + offset);
}

Port32Bit::Port32Bit(uint16_t portnumber)
    : Port(portnumber) {}

Port32Bit::~Port32Bit() {}

void Port32Bit::Write(uint32_t data, uint8_t offset) {
    Write32(portnumber + offset, data);
}

uint32_t Port32Bit::Read(uint8_t offset) {
    return Read32(portnumber + offset);
}