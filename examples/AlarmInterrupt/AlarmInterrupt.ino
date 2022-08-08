/*
AlarmInterrupt.ino
Jacob Nuernberg
08/22

Example on using interrupts with DS3231 alarms.

Connect DS3231 SQW pin to Arduino interrupt pin 2

Tested on:
- Arduino UNO
- Arduino nano

*/

#include <DS3231.h>
#include <Wire.h>

// myRTC interrupt pin
#define CLINT 2

// Setup clock
DS3231 myRTC;

// Interrupt signaling byte
volatile byte tick = 1;

void setup() {
    // Begin I2C communication
    Wire.begin();

    // Setup alarm one to fire every second
    myRTC.turnOffAlarm(1);
    myRTC.setA1Time(0, 0, 0, 0, 0b01111111, false, false, false);
    myRTC.turnOnAlarm(1);
    myRTC.checkIfAlarm(1);

    // attach clock interrupt
    pinMode(CLINT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CLINT), isr_TickTock, FALLING);

    // Use builtin LED to blink
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // static variable to keep track of LED on/off state
    static byte state = false;

    // if alarm went of, do alarm stuff
    if (tick) {
        tick = 0;
        state = ~state;
        digitalWrite(LED_BUILTIN, state);
        // Clear alarm state
        myRTC.checkIfAlarm(1);
    }

    // Loop delay to emulate other running code
    delay(10);
}


void isr_TickTock() {
    // interrupt signals to loop
    tick = 1;
    return;
}
