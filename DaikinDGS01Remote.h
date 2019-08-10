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
#include <Arduino.h>

/*
 * The following typedef blocks define custom variable types which hold data
 * structures specific to this library.
 */

// The time_struct_t type is used to define a variable for storing the hour and
// minute of the current time. This will be used to send the date/time to the
// AC indoor unit.
typedef struct {
    unsigned int hour;
    unsigned int minute;
}  time_struct_t;

// The timer_struct_t type is used to define a variable to hold the auto on/off
// data to send to the AC indoor unit. As with the remote controller, you can
// only define a time with 30 minute increments.
typedef struct {
    unsigned int hour;
    bool halfHour;
    bool active;
} timer_struct_t;

// The operation_mode_t type is used to define a variable to hold the desired
// operation mode of the AC.
typedef enum {
    OPERATION_MODE_DRY  = 0b1000,
    OPERATION_MODE_COOL = 0b0100,
    OPERATION_MODE_FAN  = 0b0010
} operation_mode_t;

// The fan_mode_t type is used to define a variable to hold the fan mode of the
// AC.
typedef enum {
    FAN_MODE_MANUAL_SPEED_1 = 0b0001,
    FAN_MODE_MANUAL_SPEED_2 = 0b0010,
    FAN_MODE_MANUAL_SPEED_3 = 0b0100,
    FAN_MODE_AUTO           = 0b1000,
    FAN_MODE_QUIET          = 0b1001,
    FAN_MODE_TURBO          = 0b1100
} fan_mode_t;

// The ac_settings_t type is used to define a variable to hold all the current
// setting of the air conditioner.
typedef struct {
    // Header
    const char header = 'h';

    // Operation mode
    operation_mode_t operationMode;

    // Fan Mode
    fan_mode_t fanMode;

    // Current Time
    time_struct_t currentTime;

    // On Timer
    timer_struct_t onTimer;

    // Off Timer { hour, halfHour, active }
    timer_struct_t offTimer;

    // Temperature Setting
    int temperature;

    // Louvre Swing
    bool swing;

    // Sleep Mode
    bool sleep;

    // Power Button
    bool powerButton;
} ac_settings_t;

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
