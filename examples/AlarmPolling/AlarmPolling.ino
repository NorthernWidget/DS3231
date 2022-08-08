/*
AlarmPolilng.ino
Jacob Nuernberg
08/22

Example on using DS3231 alarms with polling and test checkIfAlarm()

Tested on:
- Arduino nano

*/

#include <DS3231.h>
#include <Wire.h>

// Setup clock
DS3231 myRTC;


void setup() {
    // Begin I2C communication
    Wire.begin();

    // Setup alarm one to fire every second
    myRTC.turnOffAlarm(1);
    myRTC.setA1Time(0, 0, 0, 0, 0b01111111, false, false, false);
    myRTC.turnOnAlarm(1);
    myRTC.checkIfAlarm(1);

    // Use builtin LED to blink
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    // static variable to keep track of LED on/off state
    static byte state = false;

    // if alarm went of, do alarm stuff
    // first call to checkIFAlarm does not clear alarm flag
    if (myRTC.checkIfAlarm(1, false)) {
        state = ~state;
        digitalWrite(LED_BUILTIN, state);
        // Clear alarm state
        myRTC.checkIfAlarm(1, true);
    }

    // Loop delay to emulate other running code
    delay(10);
}
