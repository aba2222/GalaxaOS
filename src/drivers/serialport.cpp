#include <drivers/serialport.h>
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

SerialPort::SerialPort(PortAddresses addresse) 
    : dataPort(addresse) {
    dataPort.Write(0x00, 1);    // Disable all interrupts
    dataPort.Write(0x80, 3);    // Enable DLAB (set baud rate divisor)
    dataPort.Write(0x03, 0);    // Set divisor to 3 (lo byte) 38400 baud
    dataPort.Write(0x00, 1);    //                  (hi byte)
    dataPort.Write(0x03, 3);    // 8 bits, no parity, one stop bit
    dataPort.Write(0xC7, 2);    // Enable FIFO, clear them, with 14-byte threshold
    dataPort.Write(0x0B, 4);    // IRQs enabled, RTS/DSR set
    dataPort.Write(0x1E, 4);    // Set in loopback mode, test the serial chip
    dataPort.Write(0xAE, 0);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if(dataPort.Read(0) != 0xAE) {
       return;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    dataPort.Write(0x0F, 4);
}

int SerialPort::IsTransmitEmpty() {
    return dataPort.Read(5) & 0x20;
 }
 
void SerialPort::WriteSerial(char a) {
    while (IsTransmitEmpty() == 0);
    dataPort.Write(a, 0);
 }