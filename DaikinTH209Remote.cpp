// vim: syntax=c expandtab tabstop=4 shiftwidth=4 softtabstop=4:

#include <Arduino.h>
#include "DaikinTH209Remote.h"

DaikinTH209Remote::DaikinTH209Remote() {
}

void DaikinTH209Remote::setOperationMode(operation_mode_t mode) {
}

void DaikinTH209Remote::setFanMode(fan_mode_t fanMode) {
}

void DaikinTH209Remote::setTime(time_struct_t time) {
}

void DaikinTH209Remote::setOnTimer(timer_struct_t onTimer) {
}

void DaikinTH209Remote::setOffTimer(timer_struct_t offTimer) {
}

void DaikinTH209Remote::setTemperature(int temperature) {
}

void DaikinTH209Remote::setSleep(bool active) {
}

void DaikinTH209Remote::setSwing(bool active) {
}

unsigned int *DaikinTH209Remote::getRawTimings(bool powerButton) {
}

// reverse the bit order of a nibble
byte DaikinTH209Remote::reverseNibble (byte nibble, bool leftNibble = false)
{
    if (!leftNibble)
    {
        return reverseNibbleLookup[nibble & 0xF];
    }

    return reverseNibbleLookup[(nibble >> 4)] << 4;
}
