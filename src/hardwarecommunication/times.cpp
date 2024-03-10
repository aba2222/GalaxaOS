#include "hardwarecommunication/times.h"
using namespace myos::common;
using namespace myos::hardwarecommunication;

Times::Times(common::uint8_t cmos_address, common::uint8_t cmos_data) 
      : CMOSAddress(cmos_address),
        CMOSData(cmos_data),
		century_register(0) {}

Times::~Times() {}

int Times::GetUpdateInProgressFlag() {
	CMOSAddress.Write(0x0A);
	return CMOSData.Read() & 0x80;
}

uint8_t Times::GetRTCRegister(int reg) {
	CMOSAddress.Write(reg);
	return CMOSData.Read();
}

void Times::ReadRtc() {
 	uint8_t century;
    uint8_t last_second;
    uint8_t last_minute;
    uint8_t last_hour;
    uint8_t last_day;
    uint8_t last_month;
    uint8_t last_year;
    uint8_t last_century;
    uint8_t registerB;
 
    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates
 
    while (GetUpdateInProgressFlag());                // Make sure an update isn't in progress
    second = GetRTCRegister(0x00);
    minute = GetRTCRegister(0x02);
    hour = GetRTCRegister(0x04);
    day = GetRTCRegister(0x07);
    month = GetRTCRegister(0x08);
    year = GetRTCRegister(0x09);
    if(century_register != 0) {
        century = GetRTCRegister(century_register);
    }
 
    do {
        last_second = second;
        last_minute = minute;
        last_hour = hour;
        last_day = day;
        last_month = month;
        last_year = year;
        last_century = century;
 
        while (GetUpdateInProgressFlag());           // Make sure an update isn't in progress
        second = GetRTCRegister(0x00);
        minute = GetRTCRegister(0x02);
        hour = GetRTCRegister(0x04);
        day = GetRTCRegister(0x07);
        month = GetRTCRegister(0x08);
        year = GetRTCRegister(0x09);
        if(century_register != 0) {
            century = GetRTCRegister(century_register);
        }
    } while((last_second != second) || (last_minute != minute) || (last_hour != hour) ||
        	(last_day != day) || (last_month != month) || (last_year != year) ||
        	(last_century != century) );
 
    registerB = GetRTCRegister(0x0B);
 
    // Convert BCD to binary values if necessary
 
	if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
        if(century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }
 
    // Convert 12 hour clock to 24 hour clock if necessary
 
    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }
 
    // Calculate the full (4-digit) year
 
    if(century_register != 0) {
        year += century * 100;
    } else {
        year += (CURRENT_YEAR / 100) * 100;
        if(year < CURRENT_YEAR) year += 100;
    }
}