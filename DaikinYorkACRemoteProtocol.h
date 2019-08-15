// vim: syntax=c autoindent smartindent expandtab tabstop=4 shiftwidth=4 softtabstop=4:

/*
 * The ifndef tells the compiler to only compile the following lines if it has
 * not been done before
 */
#ifndef DAIKIN_DGS01_REMOTE_H
#define DAIKIN_DGS01_REMOTE_H

/*
 * This include is needed so that Arduino data types are recognized by the
 * compiler.
 */
#include "DaikinYorkACTypedefs.h"

/*
 * This is the main class for generating code to send to the AC using the
 * protocol used by the DGS01 remote controller.
 */
class DaikinDGS01Remote
{
    public:

        // Constructor
        DaikinDGS01Remote();

        void setOperationMode(operation_mode_t operationMode);

        void setFanMode(fan_mode_t fanMode);

        void setTime(time_struct_t currentTime);

        void setOnTimer(timer_struct_t onTimer);

        void setOffTimer(timer_struct_t offTimer);

        void setTemperature(int temperature);

        void setSleep(bool active);

        void setSwing(bool active);

        byte *getDataBytes(bool powerToggle = false);

        unsigned int *getRawTimings(bool powerToggle = false);

    private:
        // This is simply a lookup table containing the reversed bit order
        // nibbles of numbers 0 through 15
        byte reverseNibbleLookup[16] = {
            0b0000, 0b1000, 0b0100, 0b1100, 0b0010, 0b1010, 0b0110, 0b1110,
            0b0001, 0b1001, 0b0101, 0b1101, 0b0011, 0b1011, 0b0111, 0b1111
        };

        byte reverseNibble(byte nibble, bool leftNibble = false);

        // Pulse and pause lengths
        const unsigned int pulseLength  = 368; // 368 us pulse
        const unsigned int pauseLength0 = 368; // 368 us space
        const unsigned int pauseLength1 = 944; // 944 us space

        // The "beginning of transmission" signal consists of the following
        // pulse/pause pairs
        const unsigned int beginTransmission[6] = {
            9788, 9676, // 9.788ms pulse, 9.676ms pause
            9812, 9680, // 9.812ms pulse, 9.680ms pause
            4652, 2408  // 4.652ms pulse, 2.408ms pause
        };

        // The "end of transmission" signal consists of the following pulses
        // and pause
        const unsigned int endTransmission[3] = {
            368,   // 368us pulse
            20340, // 20.34ms pause
            4624   // 4.624 ms pulse
        };

        // Air conditioner settings
        ac_settings_t settings;
};

#endif
