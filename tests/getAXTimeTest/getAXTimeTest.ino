/*
getAXTimeTest.ino
Jacob Nuernberg
08/22

Testing getAXTimeMethods.
Code should generate the following output every 5s on the Serial Monitor:
-> Initialize AlarmBits: 0
-> getA1Time(): 1110
-> getA2Time(): 1111110
-> getA1Time(): 1110
-> getA2Time(): 1110000

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

    // Begin Serial communication
    Serial.begin(57600);

    // Setup alarm one to fire every minute
    // No need to turn Alarm one on.
    myRTC.turnOffAlarm(1);
    myRTC.setA1Time(0, 0, 0, 0, 0b01111110, false, false, false);
    myRTC.checkIfAlarm(1);

    // Setup alarm two to fire every minute
    // No need to turn Alarm two on.
    myRTC.turnOffAlarm(2);
    myRTC.setA2Time(0, 0, 0, 0b01111110, false, false, false);
    myRTC.checkIfAlarm(2);
}

void loop() {
    // Initialize AlarmBits
    byte AlarmBits = 0x0;
    Serial.print("Initialize AlarmBits: ");
    Serial.println(AlarmBits, BIN);

    // Initialize Others
    byte ADay, AHour, AMinute, ASecond;
    bool ADy, Ah12, APM;

    // getA1Time (not clearing).
    // Expected AlarmBits = 0x(0000)1110
    myRTC.getA1Time(ADay, AHour, AMinute, ASecond, AlarmBits, ADy, Ah12, APM);
    Serial.print("getA1Time(): ");
    Serial.println(AlarmBits, BIN);

    // getA2Time (not clearing).
    // Expected AlarmBits = 0x01111110
    myRTC.getA2Time(ADay, AHour, AMinute, AlarmBits, ADy, Ah12, APM);
    Serial.print("getA2Time(): ");
    Serial.println(AlarmBits, BIN);

    // getA1Time (clearing).
    // Expected AlarmBits = 0x(0000)1110
    myRTC.getA1Time(ADay, AHour, AMinute, ASecond, AlarmBits, ADy, Ah12, APM, true);
    Serial.print("getA1Time(): ");
    Serial.println(AlarmBits, BIN);

    // getA2Time (clearing).
    // Expected AlarmBits = 0x01110000
    myRTC.getA2Time(ADay, AHour, AMinute, AlarmBits, ADy, Ah12, APM, true);
    Serial.print("getA2Time(): ");
    Serial.println(AlarmBits, BIN);
    delay(5000);
}
