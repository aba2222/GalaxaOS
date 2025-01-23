#include "drivers/driver.h"
#include "common/types.h"

using namespace myos::common;
using namespace myos::drivers;

Driver::Driver() {}
Driver::~Driver() {}

void Driver::Activate() {}

int Driver::Reset() {
    return 0;
}

const char* Driver::GetDriverName() {
    return "Unnamed Driver";
}

void Driver::Deactivate() {}

DriverManger::DriverManger()
    : numDrivers(0) {
}

void DriverManger::AddDriver(Driver* drv) {
    drivers[numDrivers++] = drv;
}

void DriverManger::ActivateAll() {
    for(uint16_t i = 0; i < numDrivers; i++){
        drivers[i]->Activate();
    }
}