/*
AdvanceAlarm.ino
David Sparks Sept 2022

Example of advancing an alarm by an arbitrary interval of time.

Hardware setup:
  Connect DS3231 SQW pin to Arduino interrupt pin 2

Builds upon ArduinoInterrupt.ino example by
Jacob Nuernberg August 2022

Example on using recurring interrupts 
of arbitrary frequency with DS3231 alarms.

Hardware setup:
  Connect DS3231 SQW pin to Arduino interrupt pin 2

Tested on:
- Arduino Nano (ATmega328P)

*/

#include <DS3231.h>
#include <Wire.h>

// Interrupt frequency, in seconds
#define INT_FREQ 3UL // 3 seconds, characterized as unsigned long

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

    // Set the DS3231 clock mode to 24-hour
    myRTC.setClockMode(false); // false = not using the alternate, 12-hour mode

    // Set the clock to an arbitrarily chosen time of
    // 00:00:00 midnight the morning of January 1, 2020
    // using a suitable Unix-style timestamp
    myRTC.setEpoch(1640995200);

    // Assign parameter values for Alarm 1
    alarmDay = myRTC.getDate();
    alarmHour = myRTC.getHour(alarmH12, alarmPM);
    alarmMinute = myRTC.getMinute();
    alarmSecond = INT_FREQ; // initialize to the interval length
    alarmBits = 0b00001110; // Alarm 1 when seconds match
    alarmDayIsDay = false; // using date of month

    // Upload initial parameters of Alarm 1
    myRTC.turnOffAlarm(1);
    myRTC.setA1Time(
       alarmDay, alarmHour, alarmMinute, alarmSecond,
       alarmBits, alarmDayIsDay, alarmH12, alarmPM);
    // clear Alarm 1 flag after setting the alarm time
    myRTC.checkIfAlarm(1);
    // now it is safe to enable interrupt output
    myRTC.turnOnAlarm(1);

    // When using interrupt with only one of the DS3231 alarms, as in this example,
    // it may be possible to prevent the other alarm entirely,
    // so it will not covertly block the outgoing interrupt signal.

    // Try to prevent Alarm 2 altogether by assigning a 
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

    // Configure the LED for blinking
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // static variable to keep track of LED on/off state
    static byte state = false;

    // Do when alarm interrupt received:
    if (tick) {
        // right away, capture the current time in a DateTime variable
        // for later processing
        DateTime alarmDT = RTClib::now();

        // disable Alarm 1 interrupt
        myRTC.turnOffAlarm(1);
        
        // Clear Alarm 1 flag
        myRTC.checkIfAlarm(1);
        
        tick = 0; // reset the local interrupt-received flag
        state = ~state; // reverse the state of the LED
        digitalWrite(LED_BUILTIN, state);

        // optional serial output
        Serial.print("Turning LED ");
        Serial.print((state ? "ON" : "OFF"));
        Serial.print(" at ");
        Serial.print(alarmDT.hour());
        Serial.print(":");
        Serial.print(alarmDT.minute());
        Serial.print(":");
        Serial.println(alarmDT.second());

        // extract the DateTime values as a timestamp 
        uint32_t nextAlarm = alarmDT.unixtime();
        // add the INT_FREQ number of seconds
        nextAlarm += INT_FREQ;
        // update the DateTime with the new timestamp
        alarmDT = DateTime(nextAlarm);

        // upload the new time to Alarm 1
       myRTC.setA1Time(
         alarmDT.day(), alarmDT.hour(), alarmDT.minute(), alarmDT.second(),
       alarmBits, alarmDayIsDay, alarmH12, alarmPM);
       
       // enable Alarm 1 interrupts
       myRTC.turnOnAlarm(1);
      // clear Alarm 1 flag again after enabling interrupts
        myRTC.checkIfAlarm(1);
    }
}

void isr_TickTock() {
    // interrupt signals to loop
    tick = 1;
    return;
}
