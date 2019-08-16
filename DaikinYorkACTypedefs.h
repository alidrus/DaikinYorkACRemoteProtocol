// vim: syntax=c autoindent smartindent expandtab tabstop=4 shiftwidth=4 softtabstop=4:

/*
 * The ifndef tells the compiler to only compile the following lines if it has
 * not been done before
 */
#ifndef DAIKIN_YORK_AC_TYPEDEFS_H
#define DAIKIN_YORK_AC_TYPEDEFS_H

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
} time_struct_t;

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

    // Off Timer
    timer_struct_t offTimer;

    // On Timer
    timer_struct_t onTimer;

    // Temperature Setting
    int temperature;

    // Louvre Swing
    bool swing;

    // Sleep Mode
    bool sleep;

    // NOTE: We are not going to store the power button flag in this struct
    // since that is a toggle type of switch which is only sent to the AC when
    // the remote controller power button is pressed. Only the AC knows its own
    // current powered state since the remote controller does not receive
    // feedback from the AC.
} ac_settings_t;

#endif
