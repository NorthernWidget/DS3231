#include <DS3231.h>
#include <Wire.h>

// Clock interrupt pin
#define CLINT 2

DS3231 Clock;

volatile int tick = 1;

void setup() {
    // Begin serial communction
    Serial.begin(19200);
    Serial.println("DS3231 AlarmInterrupt Example");
    Serial.println("=============================");

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
}

void loop() {
    if (tick) {
        Serial.println("Tick.");
        tick = 0;
        Clock.checkIfAlarm(1);
    }
    delay(100);
}


void isr_TickTock() {
    tick = 1;
    return;
}
