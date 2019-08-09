// vim: syntax=c expandtab tabstop=4 shiftwidth=4 softtabstop=4:

#ifndef DAIKIN_TH209_REMOTE_H
#define DAIKIN_TH209_REMOTE_H

typedef struct {
    int hour;
    int minute;
}  time_struct_t;

typedef struct {
    int hour;
    bool halfHour;
    bool active;
} timer_struct_t;

typedef enum {
    OPERATION_MODE_COOL,
    OPERATION_MODE_DRY,
    OPERATION_MODE_FAN
} operation_mode_t;

typedef enum {
    FAN_MODE_MANUAL_SPEED_1,
    FAN_MODE_MANUAL_SPEED_2,
    FAN_MODE_MANUAL_SPEED_3,
    FAN_MODE_AUTO,
    FAN_MODE_TURBO,
    FAN_MODE_QUIET
} fan_mode_t;

typedef struct {
    // Operation mode
    operation_mode_t mode = OPERATION_MODE_COOL;

    // Fan Mode
    fan_mode_t fanMode = FAN_MODE_AUTO;

    // Current Time { hour, minute }
    time_struct_t currentTime = {
        0,
        0
    };

    // On Timer { hour, halfHour, active }
    timer_struct_t onTimer = {
        0,
        false,
        false
    };

    // Off Timer { hour, halfHour, active }
    timer_struct_t offTimer = {
        0, false, false
    };

    // Temperature Setting
    int temperature = 28;

    // Louvre Swing
    bool swing = true;

    // Sleep Mode
    bool sleep = false;

    // Power Button
    bool powerButton = false;
} ac_settings_t;

typedef struct {
    const unsigned int header[] = {
        9788, 9676, 9812, 9680, 4652, 2408
    };

    unsigned int data[128] = {
    };

    const unsigned int endTransmission[] = {
        368, 20340, 4624
    };
} raw_pulses_t;

class DaikinTH209Remote
{
    public:

        // Constructor
        DaikinTH209Remote();

        void setOperationMode(operation_mode_t mode);

        void setFanMode(fan_mode_t fanMode);

        void setTime(time_struct_t time);

        void setOnTimer(timer_struct_t onTimer);

        void setOffTimer(timer_struct_t offTimer);

        void setTemperature(int temperature);

        void setSleep(bool active);

        void setSwing(bool active);

        unsigned int *getRawTimings(bool powerButton);


    private:

        byte reverseNibbleLookup[] = {
            0b0000, 0b1000, 0b0100, 0b1100, 0b0010, 0b1010, 0b0110, 0b1110,
            0b0001, 0b1001, 0b0101, 0b1101, 0b0011, 0b1011, 0b0111, 0b1111
        };

        byte reverseNibble(byte nibble, bool leftNibble);

        // Mark and space lengths
        const unsigned int markLength   = 368;
        const unsigned int spaceLength0 = 368;
        const unsigned int spaceLength1 = 944;

        // Air conditioner settings
        AirCondSettings settings;
}

#endif
