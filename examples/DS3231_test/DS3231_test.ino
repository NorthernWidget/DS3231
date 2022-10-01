/*
DS3231_test.pde
Eric Ayars
4/11

Test/demo of read routines for a DS3231 RTC.

Turn on the serial monitor after loading this to check if things are
working as they should.

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;
bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

void setup() {
	// Start the I2C interface
	Wire.begin();
 
	// Start the serial interface
	Serial.begin(57600);
}

void loop() {
	// send what's going on to the serial monitor.
	
	// Start with the year
	Serial.print("2");
	if (century) {			// Won't need this for 89 years.
		Serial.print("1");
	} else {
		Serial.print("0");
	}
	Serial.print(myRTC.getYear(), DEC);
	Serial.print(' ');
	
	// then the month
	Serial.print(myRTC.getMonth(century), DEC);
	Serial.print(" ");
  
	// then the date
	Serial.print(myRTC.getDate(), DEC);
	Serial.print(" ");
  
	// and the day of the week
	Serial.print(myRTC.getDoW(), DEC);
	Serial.print(" ");
  
	// Finally the hour, minute, and second
	Serial.print(myRTC.getHour(h12Flag, pmFlag), DEC);
	Serial.print(" ");
	Serial.print(myRTC.getMinute(), DEC);
	Serial.print(" ");
	Serial.print(myRTC.getSecond(), DEC);
 
	// Add AM/PM indicator
	if (h12Flag) {
		if (pmFlag) {
			Serial.print(" PM ");
		} else {
			Serial.print(" AM ");
		}
	} else {
		Serial.print(" 24h ");
	}
 
	// Display the temperature
	Serial.print("T=");
	Serial.print(myRTC.getTemperature(), 2);
  
	// Tell whether the time is (likely to be) valid
	if (myRTC.oscillatorCheck()) {
		Serial.print(" O+");
	} else {
		Serial.print(" O-");
	}
 
	// Indicate whether an alarm went off
	if (myRTC.checkIfAlarm(1)) {
		Serial.print(" A1!");
	}
 
	if (myRTC.checkIfAlarm(2)) {
		Serial.print(" A2!");
	}
 
	// New line on display
	Serial.println();
	
	// Display Alarm 1 information
	Serial.print("Alarm 1: ");
	myRTC.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
	Serial.print(alarmDay, DEC);
	if (alarmDy) {
		Serial.print(" DoW");
	} else {
		Serial.print(" Date");
	}
	Serial.print(' ');
	Serial.print(alarmHour, DEC);
	Serial.print(' ');
	Serial.print(alarmMinute, DEC);
	Serial.print(' ');
	Serial.print(alarmSecond, DEC);
	Serial.print(' ');
	if (alarmH12Flag) {
		if (alarmPmFlag) {
			Serial.print("pm ");
		} else {
			Serial.print("am ");
		}
	}
	if (myRTC.checkAlarmEnabled(1)) {
		Serial.print("enabled");
	}
	Serial.println();
 
	// Display Alarm 2 information
	Serial.print("Alarm 2: ");
	myRTC.getA2Time(alarmDay, alarmHour, alarmMinute, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
	Serial.print(alarmDay, DEC);
	if (alarmDy) {
		Serial.print(" DoW");
	} else {
		Serial.print(" Date");
	}
	Serial.print(" ");
	Serial.print(alarmHour, DEC);
	Serial.print(" ");
	Serial.print(alarmMinute, DEC);
	Serial.print(" ");
	if (alarmH12Flag) {
		if (alarmPmFlag) {
			Serial.print("pm");
		} else {
			Serial.print("am");
		}
	}
	if (myRTC.checkAlarmEnabled(2)) {
		Serial.print("enabled");
	}
 
	// display alarm bits
  Serial.println();
	Serial.print("Alarm bits: ");
	Serial.println(alarmBits, BIN);

	Serial.println();
	delay(1000);
}
