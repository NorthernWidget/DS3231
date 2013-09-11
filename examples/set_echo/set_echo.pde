/*

Sets the time and prints back time stamps for 5 seconds

Based on DS3231_set.pde
by Eric Ayars
4/11

Added printing back of time stamps and increased baud rate
(to better synchronize computer and RTC)
Andy Wickert
5/15/2011

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;

byte Year;
byte Month;
byte Date;
byte DoW;
byte Hour;
byte Minute;
byte Second;

bool Century=false;
bool h12;
bool PM;

void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, 
		byte& Hour, byte& Minute, byte& Second) {
	// Call this if you notice something coming in on 
	// the serial port. The stuff coming in should be in 
	// the order YYMMDDwHHMMSS, with an 'x' at the end.
	boolean GotString = false;
	char InChar;
	byte Temp1, Temp2;
	char InString[20];

	byte j=0;
	while (!GotString) {
		if (Serial.available()) {
			InChar = Serial.read();
			InString[j] = InChar;
			j += 1;
			if (InChar == 'x') {
				GotString = true;
			}
		}
	}
	Serial.println(InString);
	// Read Year first
	Temp1 = (byte)InString[0] -48;
	Temp2 = (byte)InString[1] -48;
	Year = Temp1*10 + Temp2;
	// now month
	Temp1 = (byte)InString[2] -48;
	Temp2 = (byte)InString[3] -48;
	Month = Temp1*10 + Temp2;
	// now date
	Temp1 = (byte)InString[4] -48;
	Temp2 = (byte)InString[5] -48;
	Day = Temp1*10 + Temp2;
	// now Day of Week
	DoW = (byte)InString[6] - 48;		
	// now Hour
	Temp1 = (byte)InString[7] -48;
	Temp2 = (byte)InString[8] -48;
	Hour = Temp1*10 + Temp2;
	// now Minute
	Temp1 = (byte)InString[9] -48;
	Temp2 = (byte)InString[10] -48;
	Minute = Temp1*10 + Temp2;
	// now Second
	Temp1 = (byte)InString[11] -48;
	Temp2 = (byte)InString[12] -48;
	Second = Temp1*10 + Temp2;
}

void setup() {
	// Start the serial port
	Serial.begin(57600);

	// Start the I2C interface
	Wire.begin();
}

void loop() {

	// If something is coming in on the serial line, it's
	// a time correction so set the clock accordingly.
	if (Serial.available()) {
		GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);

		Clock.setClockMode(false);	// set to 24h
		//setClockMode(true);	// set to 12h

		Clock.setYear(Year);
		Clock.setMonth(Month);
		Clock.setDate(Date);
		Clock.setDoW(DoW);
		Clock.setHour(Hour);
		Clock.setMinute(Minute);
		Clock.setSecond(Second);
		
		// Give time at next five seconds
		for (int i=0; i<5; i++){
		    delay(1000);
		    Serial.print(Clock.getYear(), DEC);
		    Serial.print("-");
		    Serial.print(Clock.getMonth(Century), DEC);
		    Serial.print("-");
		    Serial.print(Clock.getDate(), DEC);
		    Serial.print(" ");
		    Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
		    Serial.print(":");
		    Serial.print(Clock.getMinute(), DEC);
		    Serial.print(":");
		    Serial.println(Clock.getSecond(), DEC);
		}

	}
	delay(1000);
}
