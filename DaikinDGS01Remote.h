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
#include "DaikinDGS01Typedefs.h"

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

        unsigned int *getRawTimings(bool powerButton = false);

    private:
        byte reverseNibbleLookup[16] = {
            0b0000, 0b1000, 0b0100, 0b1100, 0b0010, 0b1010, 0b0110, 0b1110,
            0b0001, 0b1001, 0b0101, 0b1101, 0b0011, 0b1011, 0b0111, 0b1111
        };

        byte reverseNibble(byte nibble, bool leftNibble = false);

        // Mark and space lengths
        const unsigned int markLength   = 368;
        const unsigned int spaceLength0 = 368;
        const unsigned int spaceLength1 = 944;

        // Start transmission timings
        const unsigned int startTransmission[6] = {
            9788, 9676, 9812, 9680, 4652, 2408
        };

        // Stop transmission timings
        const unsigned int stopTransmission[3] = {
            368, 20340, 4624
        };

        // Air conditioner settings
        ac_settings_t settings;

        // Raw pulse timings
        //raw_timing_t rawTimings;
};

#endif
