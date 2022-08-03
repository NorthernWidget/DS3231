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

// Clock interrupt pin
#define CLINT 2

// Setup clock
DS3231 Clock;

// Interrupt signaling byte
volatile byte tick = 1;

void setup() {
    // Begin I2C communication
    Wire.begin();

    // Setup alarm two to fire every second
    Clock.turnOffAlarm(1);
    Clock.setA1Time(0, 0, 0, 0, 0b01111111, false, false, false);
    Clock.turnOnAlarm(1);
    Clock.checkIfAlarm(1);

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
        Clock.checkIfAlarm(1);
    }
    delay(10);
}


void isr_TickTock() {
    // interrupt signals to loop
    tick = 1;
    return;
}
