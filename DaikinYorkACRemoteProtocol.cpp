// vim: syntax=c autoindent smartindent expandtab tabstop=4 shiftwidth=4 softtabstop=4:

#include "DaikinYorkACRemoteProtocol.h"

// Set the operation mode of bit.
void DaikinYorkACRemoteProtocol::setOperationMode(operation_mode_t operationMode) {
    settings.operationMode = operationMode;
}

void DaikinYorkACRemoteProtocol::setFanMode(fan_mode_t fanMode) {
    settings.fanMode = fanMode;
}

void DaikinYorkACRemoteProtocol::setTime(time_struct_t currentTime) {
    if (currentTime.hour <= 23 && currentTime.hour >= 0
        && currentTime.minute <= 59 && currentTime.minute >= 0)
    {
        settings.currentTime = currentTime;
    }
}

void DaikinYorkACRemoteProtocol::setOnTimer(timer_struct_t onTimer) {
    if (onTimer.hour <= 23 && onTimer.hour >= 0
        && (onTimer.halfHour || !onTimer.halfHour))
    {
        settings.onTimer = onTimer;
    }
}

void DaikinYorkACRemoteProtocol::setOffTimer(timer_struct_t offTimer) {
    if (offTimer.hour <= 23 && offTimer.hour >= 0)
    {
        settings.offTimer = offTimer;
    }
}

void DaikinYorkACRemoteProtocol::setTemperature(int temperature) {
    if (temperature >= 16 && temperature <= 30)
    {
        settings.temperature = temperature;
    }
}

void DaikinYorkACRemoteProtocol::setSleep(bool active) {
    settings.sleep = active;
}

void DaikinYorkACRemoteProtocol::setSwing(bool active) {
    settings.sleep = active;
}

/*
 * This method generates raw pulse/pause timings to send to the AC by using the
 * *DaikinYorkACRemoteProtocol::getDataBytes() method to generate the byte stream and
 * then converting that to raw pulses. The raw pulse data can then be fed to
 * another function or class that handles the actual transmission of those
 * pulses to the AC.
 */
unsigned int *DaikinYorkACRemoteProtocol::getRawTimings(bool powerToggle = false) {
    // Storage for the raw timings (in microseconds) of the pulses and pauses
    // we are sending to the AC.
    static unsigned int rawTimings[137];

    // Data byte stream
    byte *dataByteStream;

    // A temporary variables to store the current byte being processed
    byte tmpByte;

    // This will keep track of the current index of the rawTimings array.
    unsigned int timingPosition = 0;

    // Start the rawTimings array with the "beginning of transmission" signal
    // pulses/pauses
    for (int i = 0; i < 6; i++)
    {
        rawTimings[timingPosition++] = beginTransmission[i];
    }

    // Copy the powerToggle boolean flag passed to us by the caller into the AC
    // settings structure.
    dataByteStream = getDataBytes(powerToggle);
    for (int i = 0; i < 8; i++)
    {
        // Get byte i of the data stream
        tmpByte = dataByteStream[i];

        // Loop through the bits in each byte and convert those into raw
        // pulse/pause pairs
        for (int j = 0; j < 8; j++)
        {
            // Add a pulse
            rawTimings[timingPosition++] = pulseLength;

            // Add pause (long pause if bit is 1 and short pause if bit is 0)
            rawTimings[timingPosition++] = ((tmpByte >> (7-j)) & 0x00000001) ? pauseLength1 : pauseLength0;
        }
    }


    // End the rawTimings array with the "end of transmission" signal
    // pulses/pause
    for (int i = 0; i < 3; i++)
    {
        rawTimings[timingPosition++] = endTransmission[i];
    }

    return rawTimings;
}


////////////////////////////////
//                            //
// Daikin DGS01 Specific Code //
//                            //
////////////////////////////////

/*
 * This method generates a 64-bit (8 bytes or 16 nibbles) long stream for the
 * Daikin DGS01 Remote Controller containing an 8-bit header, 1-nibble for AC
 * operation mode, 1-nibble fan mode, 2-bytes for current time, 1-byte for off
 * timer, 1-byte for on-timer, 1-byte for temperature setting, 1-nibble for
 * swing mode/sleep mode/power button, 1-nibble checksum
 */
byte *DaikinDGS01RemoteProtocol::getDataBytes(bool powerToggle = false) {
    static byte byteStream[8];
    byte tmpByte;
    int checksum = 0;

    // BYTE 0: The binary data header is 8 bits long. It seems to be a binary
    // representation of the ASCII character 'h' (probably just short for
    // "header")
    byteStream[0] = (byte) settings.header;

    // BYTE 1: Left nibble is for operation mode and right nibble is for fan
    // mode
    tmpByte = ((byte) settings.operationMode) << 4;
    tmpByte |= ((byte) settings.fanMode);

    // Append BYTE 1 to byteStream
    byteStream[1] = tmpByte;

    // BYTE 2: Left nibble is the right digit of current time in minutes (0M)
    // and right nibble is the left digit of the current time in minutes (M0)
    tmpByte = reverseNibble((byte) (settings.currentTime.minute % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.currentTime.minute / 10));

    // Append BYTE 2 to byteStream
    byteStream[2] = tmpByte;

    // BYTE 3: Left nibble is the right digit of the current time in hours (0H)
    // and the left nibble is the left digit of the current time in hours (H0)
    tmpByte = reverseNibble((byte) (settings.currentTime.hour % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.currentTime.hour / 10));

    // Append BYTE 3 to byteStream
    byteStream[3] = tmpByte;

    // BYTE 4: Left nibble is the right digit of the off timer time in hours
    // and the first two bits of the right nibble is the left digit of the off
    // timer time in hours. The third bit of the nibble is 1 when the off
    // timer time is at half past the hour, else 0. The last bit is 1 only when
    // the off timer is active
    tmpByte = reverseNibble((byte) (settings.offTimer.hour % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.offTimer.hour / 10)) << 2;
    tmpByte != settings.offTimer.halfHour ? 0b00000010 : 0b00000000;
    tmpByte != settings.offTimer.active ? 0b00000001 : 0b00000000;

    // Append BYTE 4 to byteStream
    byteStream[4] = tmpByte;

    // BYTE 5: Left nibble is the right digit of the on timer time in hours
    // and the first two bits of the right nibble is the left digit of the on
    // timer time in hours. The third bit of the nibble is 1 when the on
    // timer time is at half past the hour, else 0. The last bit is 1 only when
    // the on timer is active
    tmpByte = reverseNibble((byte) (settings.onTimer.hour % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.onTimer.hour / 10)) << 2;
    tmpByte != settings.onTimer.halfHour ? 0b00000010 : 0b00000000;
    tmpByte != settings.onTimer.active ? 0b00000001 : 0b00000000;

    // Append BYTE 5 to byteStream
    byteStream[5] = tmpByte;

    // BYTE 6: Left nibble is the right digit (1s) of the temperature in
    // Celcius and the right nibble is the left digit (10s) of the temperature
    // in Celcius
    tmpByte = reverseNibble((byte) (settings.temperature % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.temperature / 10));

    // Append BYTE 6 to byteStream
    byteStream[6] = tmpByte;

    // BYTE 7: Left nibble is a concatenation of 4-bits: Louvre Swing On/Off +
    // Sleep Mode + 1 + Power Toggle. Right nibble is the reverse bit order
    // checksum of all the reverse bit order nibbles before it.
    tmpByte = (settings.swing ? 0b1000 : 0b0000);  // Louvre Swing On/Off
    tmpByte |= (settings.sleep ? 0b0100 : 0b0000); // Sleep Mode On/Off
    tmpByte |= 0b0010;                             // This bit is always 1
    tmpByte |= (powerToggle ? 0b0001 : 0b0000);    // Power toggle bit

    // Append left half of BYTE 7 to byteStream
    byteStream[7] = tmpByte << 4;

    for (int i = 0; i < 8; i++)
    {
        // Add reverse left nibble value
        checksum += reverseNibble(byteStream[i], true);

        // Add reverse right nibble value
        if (i < 7)
        {
            checksum += reverseNibble(byteStream[i]);
        }
    }

    // OR checksum with BYTE 7 of byteStream
    byteStream[7] |= reverseNibble(checksum);

    return byteStream;
}


/////////////////////////////////
//                             //
// York ECGS01-i Specific Code //
//                             //
/////////////////////////////////

/*
 * This method generates a 64-bit (8 bytes or 16 nibbles) long stream for the
 * York ECGS01-i Remote Controller containing an 8-bit header, 1-nibble for AC
 * operation mode, 1-nibble fan mode, 2-bytes for current time, 1-byte for
 * on-timer, 1-byte for off timer, 1-byte for temperature setting, 1-nibble for
 * swing mode/sleep mode/power button, 1-nibble checksum. You might notice that
 * the auto on and auto off bytes are swapped in position compared to the
 * Daikin DGS01 Remote Controller.
 */
byte *YorkECGS01iRemoteProtocol::getDataBytes(bool powerToggle = false) {
    static byte byteStream[8];
    byte tmpByte;
    int checksum = 0;

    // BYTE 0: The binary data header is 8 bits long. It seems to be a binary
    // representation of the ASCII character 'h' (probably just short for
    // "header")
    byteStream[0] = (byte) settings.header;

    // BYTE 1: Left nibble is for operation mode and right nibble is for fan
    // mode
    tmpByte = ((byte) settings.operationMode) << 4;
    tmpByte |= ((byte) settings.fanMode);

    // Append BYTE 1 to byteStream
    byteStream[1] = tmpByte;

    // BYTE 2: Left nibble is the right digit of current time in minutes (0M)
    // and right nibble is the left digit of the current time in minutes (M0)
    tmpByte = reverseNibble((byte) (settings.currentTime.minute % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.currentTime.minute / 10));

    // Append BYTE 2 to byteStream
    byteStream[2] = tmpByte;

    // BYTE 3: Left nibble is the right digit of the current time in hours (0H)
    // and the left nibble is the left digit of the current time in hours (H0)
    tmpByte = reverseNibble((byte) (settings.currentTime.hour % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.currentTime.hour / 10));

    // Append BYTE 3 to byteStream
    byteStream[3] = tmpByte;

    // BYTE 4: Left nibble is the right digit of the off timer time in hours
    // and the first two bits of the right nibble is the left digit of the off
    // timer time in hours. The third bit of the nibble is 1 when the off
    // timer time is at half past the hour, else 0. The last bit is 1 only when
    // the off timer is active
    tmpByte = reverseNibble((byte) (settings.offTimer.hour % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.offTimer.hour / 10)) << 2;
    tmpByte != settings.offTimer.halfHour ? 0b00000010 : 0b00000000;
    tmpByte != settings.offTimer.active ? 0b00000001 : 0b00000000;

    // Append BYTE 4 to byteStream
    byteStream[4] = tmpByte;

    // BYTE 5: Left nibble is the right digit of the on timer time in hours
    // and the first two bits of the right nibble is the left digit of the on
    // timer time in hours. The third bit of the nibble is 1 when the on
    // timer time is at half past the hour, else 0. The last bit is 1 only when
    // the on timer is active
    tmpByte = reverseNibble((byte) (settings.onTimer.hour % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.onTimer.hour / 10)) << 2;
    tmpByte != settings.onTimer.halfHour ? 0b00000010 : 0b00000000;
    tmpByte != settings.onTimer.active ? 0b00000001 : 0b00000000;

    // Append BYTE 5 to byteStream
    byteStream[5] = tmpByte;

    // BYTE 6: Left nibble is the right digit (1s) of the temperature in
    // Celcius and the right nibble is the left digit (10s) of the temperature
    // in Celcius
    tmpByte = reverseNibble((byte) (settings.temperature % 10)) << 4;
    tmpByte |= reverseNibble((byte) (settings.temperature / 10));

    // Append BYTE 6 to byteStream
    byteStream[6] = tmpByte;

    // BYTE 7: Left nibble is a concatenation of 4-bits: Louvre Swing On/Off +
    // Sleep Mode + 1 + Power Toggle. Right nibble is the reverse bit order
    // checksum of all the reverse bit order nibbles before it.
    tmpByte = (settings.swing ? 0b1000 : 0b0000);  // Louvre Swing On/Off
    tmpByte |= (settings.sleep ? 0b0100 : 0b0000); // Sleep Mode On/Off
    tmpByte |= 0b0010;                             // This bit is always 1
    tmpByte |= (powerToggle ? 0b0001 : 0b0000);    // Power toggle bit

    // Append left half of BYTE 7 to byteStream
    byteStream[7] = tmpByte << 4;

    for (int i = 0; i < 8; i++)
    {
        // Add reverse left nibble value
        checksum += reverseNibble(byteStream[i], true);

        // Add reverse right nibble value
        if (i < 7)
        {
            checksum += reverseNibble(byteStream[i]);
        }
    }

    // OR checksum with BYTE 7 of byteStream
    byteStream[7] |= reverseNibble(checksum);

    return byteStream;
}
