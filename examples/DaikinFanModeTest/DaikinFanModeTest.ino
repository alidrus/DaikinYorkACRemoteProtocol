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

// The carrier frequency to use for the IR remote.
#define IR_CARRIER_FREQUENCY 38

// Define this if you want to see debug messages in the Serial monitor.
#undef DEBUG

IRsend irsend;
DaikinDGS01RemoteProtocol irObj;

// Define variables to use
byte *dataBytes;
unsigned int *rawTimings;
bool fanModeToggle = false;

// 
void setup() {

#ifdef DEBUG
    Serial.begin(9600);
#endif

}

//
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

#ifdef DEBUG
    Serial.println("These are the raw timings we generated:");

    for (int i = 0; i < 137; i++)
    {
        Serial.print(rawTimings[i], DEC);
        if (i < 136)
        {
            Serial.print(", ");

            if (i > 0 && ((i + 1) % 10) == 0)
            {
                Serial.println();
            }
        }
    }
    Serial.println();
#endif

    // Send the raw signals that we generated to the AC remote
#ifdef DEBUG
    Serial.println("Sending to AC.");
#endif
    irsend.sendRaw(rawTimings, 137, IR_CARRIER_FREQUENCY);
}
