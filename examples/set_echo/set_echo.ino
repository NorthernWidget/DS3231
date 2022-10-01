/*

Sets the time from input and prints back time stamps for 5 seconds

Based on DS3231_set.pde
by Eric Ayars
4/11

Added printing back of time stamps and increased baud rate
(to better synchronize computer and RTC)
Andy Wickert
5/15/2011

Clean for SAMD arch, add explanation, respect code-style and
fix interpretation of Serial input if used more than once
Olivier Staquet
4/26/2020

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;

byte year;
byte month;
byte date;
byte dow;
byte hour;
byte minute;
byte second;

bool century = false;
bool h12Flag;
bool pmFlag;

/*****************************************************************************************************
 * Setup
 *  - Open Serial and Wire connection
 *  - Explain to the user how to use the program
 *****************************************************************************************************/
void setup() {
  // Start the serial port
  Serial.begin(57600);

  // Start the I2C interface
  Wire.begin();

  // Request the time correction on the Serial
  delay(4000);
  Serial.println("Format YYMMDDwhhmmssx");
  Serial.println("Where YY = Year (ex. 20 for 2020)");
  Serial.println("      MM = Month (ex. 04 for April)");
  Serial.println("      DD = Day of month (ex. 09 for 9th)");
  Serial.println("      w  = Day of week from 1 to 7, 1 = Sunday (ex. 5 for Thursday)");
  Serial.println("      hh = hours in 24h format (ex. 09 for 9AM or 21 for 9PM)");
  Serial.println("      mm = minutes (ex. 02)");
  Serial.println("      ss = seconds (ex. 42)");
  Serial.println("Example for input : 2004095090242x");
  Serial.println("-----------------------------------------------------------------------------");
  Serial.println("Please enter the current time to set on DS3231 ended by 'x':");
}

/*****************************************************************************************************
 * Loop
 *  - Each time we receive the time correction on the Serial
 *  - Set the clock of the DS3231
 *  - Echo the value from the DS3231 during 5 seconds
 *****************************************************************************************************/
void loop() {
  // If something is coming in on the serial line, it's
  // a time correction so set the clock accordingly.
  if (Serial.available()) {
    inputDateFromSerial();

    myRTC.setClockMode(false);  // set to 24h

    myRTC.setYear(year);
    myRTC.setMonth(month);
    myRTC.setDate(date);
    myRTC.setDoW(dow);
    myRTC.setHour(hour);
    myRTC.setMinute(minute);
    myRTC.setSecond(second);

    // Give time at next five seconds
    for (uint8_t i = 0; i < 5; i++){
        delay(1000);
        Serial.print(myRTC.getYear(), DEC);
        Serial.print("-");
        Serial.print(myRTC.getMonth(century), DEC);
        Serial.print("-");
        Serial.print(myRTC.getDate(), DEC);
        Serial.print(" ");
        Serial.print(myRTC.getHour(h12Flag, pmFlag), DEC); //24-hr
        Serial.print(":");
        Serial.print(myRTC.getMinute(), DEC);
        Serial.print(":");
        Serial.println(myRTC.getSecond(), DEC);
    }

    // Notify that we are ready for the next input
    Serial.println("Please enter the current time to set on DS3231 ended by 'x':");
  }
  delay(1000);
}

/*****************************************************************************************************
 * inputDateFromSerial
 *  - Read and interpret the data from the Serial input
 *  - Store the data in global variables
 *****************************************************************************************************/
void inputDateFromSerial() {
	// Call this if you notice something coming in on
	// the serial port. The stuff coming in should be in
	// the order YYMMDDwHHMMSS, with an 'x' at the end.
	boolean isStrComplete = false;
	char inputChar;
	byte temp1, temp2;
	char inputStr[20];

	uint8_t currentPos = 0;
	while (!isStrComplete) {
		if (Serial.available()) {
			inputChar = Serial.read();
			inputStr[currentPos] = inputChar;
			currentPos += 1;

      // Check if string complete (end with "x")
			if (inputChar == 'x') {
				isStrComplete = true;
			}
		}
	}
	Serial.println(inputStr);

  // Find the end of char "x"
  int posX = -1;
  for(uint8_t i = 0; i < 20; i++) {
    if(inputStr[i] == 'x') {
      posX = i;
      break;
    }
  }

  // Consider 0 character in ASCII
  uint8_t zeroAscii = '0';

	// Read Year first
	temp1 = (byte)inputStr[posX - 13] - zeroAscii;
	temp2 = (byte)inputStr[posX - 12] - zeroAscii;
	year = temp1 * 10 + temp2;

	// now month
	temp1 = (byte)inputStr[posX - 11] - zeroAscii;
	temp2 = (byte)inputStr[posX - 10] - zeroAscii;
	month = temp1 * 10 + temp2;

	// now date
	temp1 = (byte)inputStr[posX - 9] - zeroAscii;
	temp2 = (byte)inputStr[posX - 8] - zeroAscii;
	date = temp1 * 10 + temp2;

	// now Day of Week
	dow = (byte)inputStr[posX - 7] - zeroAscii;

	// now Hour
	temp1 = (byte)inputStr[posX - 6] - zeroAscii;
	temp2 = (byte)inputStr[posX - 5] - zeroAscii;
	hour = temp1 * 10 + temp2;

	// now Minute
	temp1 = (byte)inputStr[posX - 4] - zeroAscii;
	temp2 = (byte)inputStr[posX - 3] - zeroAscii;
	minute = temp1 * 10 + temp2;

	// now Second
	temp1 = (byte)inputStr[posX - 2] - zeroAscii;
	temp2 = (byte)inputStr[posX - 1] - zeroAscii;
	second = temp1 * 10 + temp2;
}
