// now.pde
// Prints a snapshot of the current date and time along with the UNIX time
// Modified by Andy Wickert from the JeeLabs / Ladyada RTC library examples
// 5/15/11

#include <Wire.h>
#include <DS3231.h>

RTClib myRTC;

void setup () {
    Serial.begin(57600);
    Wire.begin();
    delay(500);
    Serial.println("Nano Ready!");
}

void loop () {
    
    delay(1000);
    
    DateTime now = myRTC.now();
    
    Serial.print(now.getYear(), DEC);
    Serial.print('/');
    Serial.print(now.getMonth(), DEC);
    Serial.print('/');
    Serial.print(now.getDay(), DEC);
    Serial.print(' ');
    Serial.print(now.getHour(), DEC);
    Serial.print(':');
    Serial.print(now.getMinute(), DEC);
    Serial.print(':');
    Serial.print(now.getSecond(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.getUnixTime());
    Serial.print("s = ");
    Serial.print(now.getUnixTime() / 86400L);
    Serial.println("d");
}
