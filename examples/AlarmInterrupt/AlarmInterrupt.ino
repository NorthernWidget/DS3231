/*
AlarmInterrupt.ino
Jacob Nuernberg
08/22

Example on using interrupts with DS3231 alarms.

Hardware setup:
  Connect DS3231 SQW pin to Arduino interrupt pin 2

Tested on:
- Arduino UNO
- Arduino nano

Added to this example:

1. Descriptively named variables to pass parameter values.

2. Modify AlarmBits for Alarm 1 to 0b00001111,
   for clarity, because Alarm 1 uses only bits 3:0.
   
3. Add code to prevent Alarm 2 from interfering with the interrupt,
   by setting A2Minute to a value that can never match the time
   and setting AlarmBits to 0b01100000: alarm "when minutes match".
   Also clear the A2 alarm flag.

David Sparks, September 2022
*/

#include <DS3231.h>
#include <Wire.h>

// myRTC interrupt pin
#define CLINT 2

// Setup clock
DS3231 myRTC;

// Variables for use in method parameter lists
byte alarmDay;
byte alarmHour;
byte alarmMinute;
byte alarmSecond;
byte alarmBits;
bool alarmDayIsDay;
bool alarmH12;
bool alarmPM;

// Interrupt signaling byte
volatile byte tick = 1;

void setup() {
    // Begin I2C communication
    Wire.begin();

    // Begin Serial communication
    Serial.begin(9600);
    while (!Serial);
    Serial.println();
    Serial.println("Starting Serial");

    // Assign parameter values for Alarm 1
    alarmDay = 0;
    alarmHour = 0;
    alarmMinute = 0;
    alarmSecond = 0;
    alarmBits = 0b00001111; // Alarm 1 every second
    alarmDayIsDay = false;
    alarmH12 = false;
    alarmPM = false;    

    // Set alarm 1 to fire at one-second intervals
    myRTC.turnOffAlarm(1);
    myRTC.setA1Time(
       alarmDay, alarmHour, alarmMinute, alarmSecond,
       alarmBits, alarmDayIsDay, alarmH12, alarmPM);
    // enable Alarm 1 interrupts
    myRTC.turnOnAlarm(1);
    // clear Alarm 1 flag
    myRTC.checkIfAlarm(1);

    // When using interrupt with only one of the DS3231 alarms, as in this example,
    // it may be advisable to prevent the other alarm entirely,
    // so it will not covertly block the outgoing interrupt signal.

    // Prevent Alarm 2 altogether by assigning a 
    // nonsensical alarm minute value that cannot match the clock time,
    // and an alarmBits value to activate "when minutes match".
    alarmMinute = 0xFF; // a value that will never match the time
    alarmBits = 0b01100000; // Alarm 2 when minutes match, i.e., never
    
    // Upload the parameters to prevent Alarm 2 entirely
    myRTC.setA2Time(
        alarmDay, alarmHour, alarmMinute,
        alarmBits, alarmDayIsDay, alarmH12, alarmPM);
    // disable Alarm 2 interrupt
    myRTC.turnOffAlarm(2);
    // clear Alarm 2 flag
    myRTC.checkIfAlarm(2);

    // NOTE: both of the alarm flags must be clear
    // to enable output of a FALLING interrupt

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

        // optional serial output
        Serial.print("Turning LED ");
        Serial.println((state ? "ON" : "OFF"));

        // Clear Alarm 1 flag
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
