// vim: syntax=arduino autoindent smartindent expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent smartindent:

/*
 * DaikinYorkRemoteProtocol: Daikin Power Button Test
 * Version 0.1 August, 2019
 * Copyright 2019 Abu Bakar Al-Idrus
 *
 * This sketch emulates sending alternately sending Fan Mode Turbo and Fan Mode
 * Quiet to the AC every 10 seconds.
 *
 * In order to run this test, you will need Ken Shirriff's IRremote library
 * installed. Your IR LED driving circuit should be connected to PIN 3 of the
 * Arduino Uno or PIN 9 if you are using Arduino Mega 2560 (for other models,
 * refer to comments in the boarddefs.h file of the IRremote library).
 */

#include <IRremote.h>
#include <DaikinYorkACRemoteProtocol.h>

#define IR_CARRIER_FREQUENCY 38

IRsend irsend;
DaikinDGS01Remote irObj;

// Define variables to use
byte *dataBytes;
unsigned int *rawTimings;
bool fanModeToggle = false;

/*
 * All code will be in the setup as we are sending to the AC only once.
 */
void setup() {
}

/*
 * We are not running the loop as this will not be a repetitive process.
 */
void loop() {
    delay(10000);

    if (!fanModeToggle)
    {
        irObj.setFanMode(FAN_MODE_QUIET);
    }
    else
    {
        irObj.setFanMode(FAN_MODE_TURBO);
    }

    fanModeToggle = !fanModeToggle;

    // Get the raw timings for the signal
    rawTimings = irObj.getRawTimings();

    // Send raw signals to the AC remote
    irsend.sendRaw(rawTimings, 137, IR_CARRIER_FREQUENCY);
}
