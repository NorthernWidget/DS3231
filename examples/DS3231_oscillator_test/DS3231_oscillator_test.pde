/*
oscillator_test.pde
Eric Ayars
4/11

Test/demo of oscillator routines for a DS3231 RTC.

Use a scope after loading this to check if things are
working as they should.

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;
byte j;
bool on = false;

void setup() {
	// Start the I2C interface
	Wire.begin();
	// Start the serial interface
	Serial.begin(57600);
}

void loop() {
	for (j=0;j<4;j++) {
		// invert state of 32kHz oscillator.
		on = !on;
		myRTC.enable32kHz(on);
		// Turn on oscillator pin, frequency j
		myRTC.enableOscillator(true, false, j);
		delay(4000);
	}
	// So... The 32kHz oscillator (pin 1) will turn on or off once each 2s,
	// and the oscillator out pin (pin 3) will cycle through frequencies.
}
