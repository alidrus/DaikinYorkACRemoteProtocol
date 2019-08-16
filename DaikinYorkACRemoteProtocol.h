// vim: syntax=c autoindent smartindent expandtab tabstop=4 shiftwidth=4 softtabstop=4:

/*
 * The ifndef tells the compiler to only compile the following lines if it has
 * not been done before
 */
#ifndef DAIKIN_YORK_AC_REMOTE_PROTOCOL_H
#define DAIKIN_YORK_AC_REMOTE_PROTOCOL_H

/*
 * This include is needed so that Arduino data types are recognized by the
 * compiler.
 */
#include "DaikinYorkACTypedefs.h"


/*
 * ABSTRACT CLASS containing nibble manipulation methods to be used for various
 * operations such as checksumming.
 */
class NibbleManipulation
{
    protected:
        // This is simply a lookup table containing the reversed bit order
        // nibbles of numbers 0 through 15
        byte reverseNibbleLookup[16] = {
            0b0000, 0b1000, 0b0100, 0b1100, 0b0010, 0b1010, 0b0110, 0b1110,
            0b0001, 0b1001, 0b0101, 0b1101, 0b0011, 0b1011, 0b0111, 0b1111
        };

        // This method takes a nibble and uses the reverseNibbleLookup array to map the
        // reverse byte order nibble and return it to the caller. The leftNibble
        // argument is true if we want the reverse byte order nibble of the left nibble
        // of a byte.
        byte reverseNibble(byte nibble, bool leftNibble = false)
        {
            if (!leftNibble)
            {
                return reverseNibbleLookup[nibble & 0xF];
            }

            return reverseNibbleLookup[nibble >> 4];
        }
};


/*
 * ABSTRACT CLASS for generating code to send to the AC using the protocol used
 * by some Daikin and York AC remote controllers. This was written based on
 * results of reverse engineering the Daikin DGS01 and York ECGS01-i remotes.
 */
class DaikinYorkACRemoteProtocol
{
    public:
        // Constructor
        DaikinYorkACRemoteProtocol()
        {
            // Initialize the settings with default values
            settings.operationMode      = OPERATION_MODE_COOL;
            settings.fanMode            = FAN_MODE_AUTO;
            settings.currentTime.hour   = 0;
            settings.currentTime.minute = 0;
            settings.onTimer.hour       = 0;
            settings.onTimer.halfHour   = true;
            settings.onTimer.active     = false;
            settings.offTimer.hour      = 0;
            settings.offTimer.halfHour  = true;
            settings.offTimer.active    = false;
            settings.temperature        = 28;
            settings.swing              = true;
            settings.sleep              = false;
        }

        void setOperationMode(operation_mode_t operationMode);

        void setFanMode(fan_mode_t fanMode);

        void setTime(time_struct_t currentTime);

        void setOnTimer(timer_struct_t onTimer);

        void setOffTimer(timer_struct_t offTimer);

        void setTemperature(int temperature);

        void setSleep(bool active);

        void setSwing(bool active);

        // Get data bytes of the signal we will be sending to the AC.
        virtual byte *getDataBytes(bool powerToggle = false);

        // Get the raw timings based on the data bytes
        unsigned int *getRawTimings(bool powerToggle = false);

    private:

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

/*
 * Daikin DGS01 remote protocol class is derived from
 * DaikinYorkACRemoteProtocol but has its own set of pulse widths and method
 * for constructing raw timings.
 */
class DaikinDGS01RemoteProtocol : public DaikinYorkACRemoteProtocol,
                                  private NibbleManipulation
{
    public:

        // Get data bytes of the signal we will be sending to the AC.
        byte *getDataBytes(bool powerToggle = false);

    private:

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

/*
 * York ECGS01-i remote protocol class is derived from
 * DaikinYorkACRemoteProtocol but has its own set of pulse widths and method
 * for constructing raw timings.
 */
class YorkECGS01iRemoteProtocol : public DaikinYorkACRemoteProtocol,
                                  private NibbleManipulation
{
    public:

        // Get data bytes of the signal we will be sending to the AC.
        byte *getDataBytes(bool powerToggle = false);

    private:

        // Pulse and pause lengths
        const unsigned int pulseLength  = 436; // 436 us pulse
        const unsigned int pauseLength0 = 296; // 296 us space
        const unsigned int pauseLength1 = 868; // 868 us space

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
            436,   // 436us pulse
            20052, // 20.05ms pause
            4652   // 4.652 ms pulse
        };

        // Air conditioner settings
        ac_settings_t settings;

};

#endif
