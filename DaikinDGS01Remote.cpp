// vim: syntax=c autoindent smartindent expandtab tabstop=4 shiftwidth=4 softtabstop=4:

#include "DaikinDGS01Remote.h"

// Class Constructor
DaikinDGS01Remote::DaikinDGS01Remote() {
}

// Set the operation mode of bit
void DaikinDGS01Remote::setOperationMode(operation_mode_t operationMode) {
    settings.operationMode = operationMode;
}

void DaikinDGS01Remote::setFanMode(fan_mode_t fanMode) {
    settings.fanMode = fanMode;
}

void DaikinDGS01Remote::setTime(time_struct_t currentTime) {
    if (currentTime.hour <= 23 && currentTime.hour >= 0
        && currentTime.minute <= 59 && currentTime.minute >= 0)
    {
        settings.currentTime = currentTime;
    }
}

void DaikinDGS01Remote::setOnTimer(timer_struct_t onTimer) {
    if (onTimer.hour <= 23 && onTimer.hour >= 0
        && (onTimer.halfHour || !onTimer.halfHour))
    {
        settings.onTimer = onTimer;
    }
}

void DaikinDGS01Remote::setOffTimer(timer_struct_t offTimer) {
    if (offTimer.hour <= 23 && offTimer.hour >= 0)
    {
        settings.offTimer = offTimer;
    }
}

void DaikinDGS01Remote::setTemperature(int temperature) {
    if (temperature >= 16 && temperature <= 30)
    {
        settings.temperature = temperature;
    }
}

void DaikinDGS01Remote::setSleep(bool active) {
    settings.sleep = active;
}

void DaikinDGS01Remote::setSwing(bool active) {
    settings.sleep = active;
}

unsigned int *DaikinDGS01Remote::getRawTimings(bool powerButton = false) {
    // Storage for the raw timings (in microseconds) of the pulses we are
    // sending to the AC.
    unsigned int rawTimings[137];

    // This will keep track of the current index of the rawTimings array.
    unsigned int timingPosition = 0;

    // A temporary place to hold the data for the current nibble (nibble = 4
    // bits of data, or half a byte ... *chomp* LOL).
    byte tmpNibble;

    // Copy the powerButton boolean flag passed to us by the caller into the AC
    // settings structure.
    settings.powerButton = powerButton;

    // The "start" transmission signal comprises of alternating 3 pulses and 3
    // pauses. As far as I know this does not represent any binary value. It is
    // probably some kind of transmission start marker which confused the
    // IRremote library because this header has 3 pulses with pauses in between
    // and not just 1 pulse like in other protocols.
    for (int i = 0; i < sizeof(startTransmission); i++)
    {
        rawTimings[timingPosition++] = startTransmission[i];
    }

    //
    // The next 64 pulses (each followed by either a long or short pause)
    // represent binary data sent to the AC. A pulse followed by a long pause
    // represents binary 1 whereas a pulse followed by a short pause represents
    // a binary 0.
    //

    // This seems to be the binary data header which is 8 bits long. It seems
    // to be a binary representation of the ASCII character 'h' (probably just
    // short for "header")
    for (int i = 0; i < 8; i++)
    {
        // Temporarily store the right most bit after shifting all bits to the
        // right i times.
        tmpNibble = (((byte) settings.header) >> i) & 0b00000001;

        // Added it to the timing
        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = tmpNibble ? spaceLength0 : spaceLength1;
    }

    // 4-bit operation mode (4 pulses + 4 pauses)
    for (int i = 0; i < 4; i++)
    {
        tmpNibble = (((byte) settings.operationMode) >> i) & 0b00000001;

        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = tmpNibble ? spaceLength0 : spaceLength1;
    }

    // 4-bit fan mode (4 pulses + 4 pauses)
    for (int i = 0; i < 4; i++)
    {
        tmpNibble = (((byte) settings.fanMode) >> i) & 0b00000001;

        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = tmpNibble ? spaceLength0 : spaceLength1;
    }

    // 4-bit time minute right digit (1s) (4 pulses + 4 pauses)
    tmpNibble = (byte) (settings.currentTime.minute % 10);
    tmpNibble = reverseNibble(tmpNibble);
    for (int i = 0; i < 4; i++)
    {
        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = ((tmpNibble >> i) & 0b00000001) ? spaceLength0 : spaceLength1;
    }

    // 4-bit time minute left digit (10s) (4 pulses + 4 pauses)
    tmpNibble = (byte) (settings.currentTime.minute / 10);
    tmpNibble = reverseNibble(tmpNibble);
    for (int i = 0; i < 4; i++)
    {
        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = ((tmpNibble >> i) & 0b00000001) ? spaceLength0 : spaceLength1;
    }

    // 4-bit time hour right digit (1s) (4 pulses + 4 pauses)
    tmpNibble = (byte) (settings.currentTime.hour % 10);
    tmpNibble = reverseNibble(tmpNibble);
    for (int i = 0; i < 4; i++)
    {
        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = ((tmpNibble >> i) & 0b00000001) ? spaceLength0 : spaceLength1;
    }

    // 4-bit time hour left digit (10s) (4 pulses + 4 pauses)
    tmpNibble = (byte) (settings.currentTime.hour / 10);
    tmpNibble = reverseNibble(tmpNibble);
    for (int i = 0; i < 4; i++)
    {
        rawTimings[timingPosition++] = markLength;
        rawTimings[timingPosition++] = ((tmpNibble >> i) & 0b00000001) ? spaceLength0 : spaceLength1;
    }

    return rawTimings;
}

// reverse the bit order of a nibble
byte DaikinDGS01Remote::reverseNibble(byte nibble, bool leftNibble = false)
{
    if (!leftNibble)
    {
        return reverseNibbleLookup[nibble & 0xF];
    }

    return reverseNibbleLookup[(nibble >> 4)] << 4;
}
