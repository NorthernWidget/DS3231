#DS3231
## Utility Functions
The versatile DS3231 is more than just an alarm clock. It supplies a timer and a temperature sensor, also. This Library includes utility functions giving access to these other capabilities.

### Contents

* [enable32kHz()](#32k)
* [enableOscillator()](#enable-oscillator)
* [oscillatorCheck()](#oscillator-check)
* [getTemperature()](#temperature)
* [Pin Change Interrupt](#pin-change-interrupt)

### <a id="32k">enable32kHz()</a>

The 32K output pin of a DS3231 can supply a highly accurate timer for interrupt-driven Arduino programming. Connect the pin to an interrupt-enabled Arduino input pin, enable the 32K output, and explore the benefits of programming with timer-driven interrupts, without the complexities of configuring timers built into the Arduino's microcontroller.

```

/*
 * Turn on or off the 32.768 kHz output of a DS3231 RTC
 *
 * returns: nothing (void)
 * one parameter: TF, boolean
 * effect:
 *    TF == true, turns the output on
 *    TF = false, turns the output off
 */

void enable32kHz(bool TF);

 /* example of use */
 myRTC.enable32kHz( true );
```

The heart of a DS3231 Real Time Clock device is an oscillator switching a voltage from HIGH to LOW and back again at an exquisitely maintained frequency of 32,768 cycles per second, or 32.768 kHz.

An output pin on the DS3231 makes these voltage levels available to other devices. The Arduino can use the signal as a timer for interrupt-drive applications. It takes just a few steps:

* Connect an interrrupt-capable input pin to the 32K output.
* Set a few bits in certain Arduino registers to enable the interrupt.
* Write program code to "service" the interrupt, that is, to be executed after each interrupt occurs.

The following example toggles an LED on or off 16 times per second, using the output of a DS3231 oscillator. Notice that there is *no code* in the idle process, also known as *loop()*. 

Significantly, the alarms of the DS3231 do not come into play here. Instead, the Arduino program determines the interval between actions.

The ease of using such a simple timer could support a very nice pathway into learning timer/counter interrupt programming techniques.

```
/*
 * Control an LED from an interrupt service routine
 * activated by a 32.768 kHz oscillator in a DS3231 RTC
 *
 * Hardware setup for Uno/Nano (ATmega328P-based):
 *   Connect digital pin 3 to the 32K pin on the DS3231
 *
 */

#import <DS3231.h>
DS3231 myRTC; // Set up access to the DS3231

#define PIN32K 3 // the pin to receive signal from DS3231

void blinky(); // prototype for the interrupt service routine

void setup() {
  Wire.begin(); // establish I2C communications

  // configure I/O pins on the Arduino
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN32K, INPUT);

  /*
   * Configure the interrupt to detect the 32K oscillator.
   * Note that interrupting on CHANGE will generate 2 interrupts per cycle,
   * once when the voltage changes to LOW from HIGH, 
   * then again when it changes to HIGH from LOW,
   * for a total of 32768 x 2 = 65536 interrupts per second
   */

  attachInterrupt(digitalPinToInterrupt(PIN32K), blinky, CHANGE);

  // enable output on the 32K pin of the DS3231
  myRTC.enable32kHz(true);
}

void loop() {
  // no code here
}

// the interrupt service routine 

void blinky() {
  static byte state = 0; // for turning LED on and off
  static uint16_t counter = 0; // accumulates count of interrupts received
  
  // Increment the counter and test it. Perform an action if the test result is true.
  // Examples of several tests are defined as macros here.

#define BLINK_ONCE (++counter == 0) // once per second, when counter rolls over
#define BLINK_TWICE (++counter >> 15) // twice per second, when counter reaches 2^15
#define BLINK_FOUR (++counter >> 14) // four times per second, when counter reaches 2^14
#define BLINK_EIGHT (++counter >> 13) // eight per second, when counter reaches 2^13
#define BLINK_16 (++counter >> 12) // 16 times per second, when counter reaches 2^12

  if (BLINK_16)   // Write-in the macro you wish to evaluate.
  {
    state = !state; // flipflop the state
    digitalWrite(LED_BUILTIN, state); // flipflop the LED
    tCount = 0; // start the count over
  }
}

```

Let's follow the math in that example.

The oscillator frequency, 32.768 kHz lends itself easily to binary arithmetic because the number of cycles is a power of two: 2<sup>15</sup> = 32768, to be precise. 

Each cycle is made up of two voltage changes, e.g., HIGH to LOW, then LOW back to HIGH. Thus, the oscillator produces 2<sup>15</sup> × 2 = 2<sup>16</sup> voltage changes per second. 

Each voltage change triggers an interrupt because the code specifies CHANGE to be the trigger. Thus, the interrupt service routine (ISR) will be invoked 2<sup>16</sup> times each second.

A counter variable is incremented upon each interrupt. An unsigned 16-bit counter being incremented 2<sup>16</sup> times per second will "roll over" to a value of zero exactly once every second. 

The ISR can test for and act upon lesser values. It means programmers can design ISRs to perform actions at intervals shorter than one second. For an example of an interval that is not a power of two, see the section on [Pin Change Interrupts](#pin-change-interrupt), below.

Note that the output on the 32K pin is independent of that on the INT/SQW pin. The two pins can separately drive interrupts to two, different input pins on an Arduino.

### <a id="enable-oscillator">enableOscillator()</a>

```
/*
 * Regulates the output on the INT/SQW pin
 *
 * returns: nothing (void)
 * three parameters:
 *    TF, boolean
 *        (true) output a square wave on the INT/SQW pin
 *        (false) output alarm interrupts on the INT/SQW pin
 *    battery, boolean
 *        (true) allow square wave output when running on battery
 *        (false) do not output square wave if running on battery
 *    frequency, 8-bit unsigned integer, select the frequency of the square wave output
 *        0: 1 Hz = one cycle per second
 *        1:       1.024 kHz
 *        2:       4.096 kHz
 *        3 - 255: 8.192 kHz 
 */

void enableOscillator(bool TF, bool battery, byte frequency)

/*
 * example of use
 * output square wave at frequency 1 Hz even when running on battery
 */

bool outputSQW = true;
bool batteryUseAllowed = true;
byte outputFrequency = 0; // select 1 Hz

myRTC.enableOscillator(outputSQW, batteryUseAllowed, outputFrequency);

```

The INT/SQW output pin on a DS3231 can operate in either one of two, different modes. 

* Interrupt mode is described in the documentation for Alarms. It:
    * uses the pin to generate an interrupt signal when an alarm flag goes HIGH.
    * is ideal for occasional signals under program control.
    * can automate a repeating series of pulses at long intervals ranging from one second up to one month.
* Oscillator mode is described below. It:
    * generates a series of pulses that repeat automatically.
    * offers programs a choice of four, rapid frequencies ranging from one second (1 Hz) to 8.192 KHz.

By default when power is first applied to the DS3231, the INT/SQW pin is configured to operate in Interrupt mode.

Note that the output on the INT/SQW pin is independent of that on the 32K pin. The two pins can separately drive interrupts to two, different input pins on an Arduino.


### <a id="oscillator-check">oscillatorCheck()</a>

```
/*
 * Detects trouble that might have made the time inaccurate in the DS3231.
 *
 * returns: boolean,
 *   (false) means the DS3231 oscillator has stopped running for some reason
 *   in which case the time in the DS3231 could be inaccurate.
 *
 *   (true) means that the DS3231 oscillator has not stopped since the flag was last cleared
 *
 * parameters: none
 */

bool oscillatorCheck()

/* example of usage */

bool rtcRemainsHealthy = myRTC.oscillatorCheck();
```

A flag named Oscillator Stop Flag (OSF)in the DS3231 gets written to logic 1 by the hardware whenever one of the following four conditions occurs:

1. Power is first applied to the DS3231.
2. DS3231 switches to battery backup power, unless another flag ("EOSC", see the data sheet) is set to disable the oscillator. In that situation, the oscillator stops and data in all of the DS3231 registers is held static.
3. The power has decreased, on both the V<sub>CC</sub> and V<sub>BAT</sub> pins, to a level less than what is needed to sustain reliable operation.  
4. Certain other "external influences", such as electrical noise, cause the flag to be set.

The timekeeping data in the DS3231 should be viewed with doubt in the event the OSF flag is found to be at logic level 1.

The flag will remain at logic level 1 until it is written to zero by program code. In this Library, the function that writes OSF to zero is the setSeconds() method of the DS3231 class.

The setEpoch() method invokes setSeconds(), which means that setEpoch() will clear the OSF flag also.

Reminder: the OSF flag will be written to 1 and the oscillator will not be running when power is first applied to the DS3231. Setting the time, specifically setting the seconds value of the time, writes OSF to zero and starts the oscillator which drives the timekeeping process.

### <a id="temperature">getTemperature()</a>

```
/*
 * Retrieve the internal temperature of the DS3231
 *
 * returns: the floating-point value of the temperature
 *
 * error return: −9999, if a valid temperature could not be retrieved
 * 
 * parameters: none
 * 
 */

float getTemperature()

/* example of usage */

float rtcTemp = myRTC.getTemperature();
if (float > -9999)
{
  // it may be OK to use the returned value
} 
else 
{
  // the value returned is not valid
}

```

Why would a clock chip contain a temperature sensor? The answer helps to understand how the DS3231 can maintain a very high level of accuracy. 

Your friendly neighborhood Documentarian will make a non-engineer's attempt to explain. 

An oscillator's frequency can vary with temperature. It can also vary with small changes in electrical capacitance.

The DS3231 hardware includes an array of tiny capacitors that become engaged or disengaged in regulating the oscillator, based on the measured voltage level from a temperature sensor. 

Long story short, the temperature sensor is there to help maintain the accuracy of the clock. The sensor's measurement is updated at 64-second intervals in two memory registers. 

This function retrieves the values in those two registers and combines them into a floating-point value.

According to the data sheet, the temperature values stored in the DS3231 registers claim to be accurate within a range of three degrees Celsius above or below the actual temperature.

### Pin Change Interrupt
The oscillating output from the 32K pin of a DS3231 makes an excellent source of timer input for the  Pin Change Interrupt capability of AVR-based Arduino boards.

All of the I/O pins on popular models such as the Uno and Nano can be used to generate interrupts in response to a fluctuating voltage. Each change, whether to HIGH from LOW or vice versa, will trigger an interrupt.

65,536 interrupts will result from connecting the 32K output of a DS3231 to a Pin Change Interrupt-enabled input pin of an Arduino.

It falls outside the scope of this article to explain interrupt-based programming in general. Many books and articles cover the topic very well. 

Pin Change Interrupts may be a lesser-known feature of AVR microcontrollers. Their usage is documented in the different data sheets for each model of controller. The example below was written for the ATmega328P chip found on Arduino Uno and Nano models. Refer to that data sheet for more information.

The example also illustrates a solution to a general problem of working with powers of 2: how to handle remainders following division by numbers other than 2. 

An algorithm is developed to divide the 65,536 interrupts into ten, nearly-equal intervals of time, while completing all ten intervals in exactly one second.

Dividing by other values can be approached similarly. The program writer would need to work out the applicable numbers of steps and interval lengths.

Comments in the example provide additional documentaiton.

```
#include <DS3231.h>
DS3231 myRTC;


void setup() {

  // set up the LED blink pin
  pinMode(LED_BUILTIN, OUTPUT);

  // start I2C and enable 32.768 KHz output from the DS3231 
  Wire.begin();
  myRTC.enable32kHz(true);

  // enable pin A2 to receive the 32.768 KHz output from DS3231
  pinMode(A2, INPUT); // not INPUT_PULLUP

  // Configure pin A2 to trigger interrupts 
  // each time the voltage level changes between LOW and HIGH.
  //
  // The following explicitly uses hardware address names
  // defined in the ATmega328P datasheet
  // to modify relevant bits in relevant registers.
  // It avoids the use of special libraries, by programmer's preference.
  
  PCMSK1 = (1<<PCINT10); // select pin A2 for pin change interrupt
  cli(); // temporarily disable interrupts globally
  PCICR |= (1<<PCIE1); // enable pin change interrupt for A2
  PCIFR |= (1<<PCIF1); // clear the interrupt flag bit for A2
  sei(); // re-enable interrupts globally 
}

void loop() {
  
  // no code in the loop!

}

/* 
 * The Interrupt Service Routine initiates an action 
 * at intervals very close to 1/10 second in length.
 * The DS3231 outputs precisely 65,536 voltage level changes per second.
 * Arduino hardware generates 65,536 interrupts per second in response.
 * Alas, 65536 does not divide evenly by 10. 
 * However, a well-designed algorithm can accurately
 * separate the 65,536 interrupts into ten segments
 * of very nearly the same length, as follows: 
 *    four segments of 6553 
 *    plus six segments of 6554.
 * This is accurate because ( 4 × 6553) + (6 × 6554) = 65536.
 * Humans cannot discern a difference of less than 1/65000th of a second.
 * 
 */

ISR(PCINT1_vect) {

  // housekeeping variables
  static uint16_t interval = 0; // step length counter
  static uint8_t led_state = 0;
  static uint8_t next_step = 0; // step number
  
  if (interval > 0) { 
    
    // The interval of time as not yet expired.
    // Decrement the counter and exit the ISR.
    interval -= 1; 
    
  } else {
    
    // The step length counter has timed out ( == 0 )
    // which means the interval of time has expired.

    // First, take care of the housekeeping.
    
    // Renew the step length counter
    //   assign value of 6553 on steps 0 through 3
    //   but value of 6554 on steps 4 through 9
    interval = (next_step < 4) ? 6553 : 6554;

    // Increment the step number 
    next_step += 1;
    // but roll it over to 0 after step 9.
    if (next_step > 9) next_step = 0;

    // The housekeeping is complete.
    // Now execute code for the action to be taken 
    // after an interval of time expires.

    // In this example, we merely toggle an LED.
    // "Real" code could initiate almost any action.
    
    led_state = ! led_state;
    digitalWrite(LED_BUILTIN, led_state);
  }
}
```

#### References for Pin Change Interrupts

* Almy, Tom. *Far Inside the Arduino*. 2020. pp 54-83

* Williams, Elliot. *Make: AVR Programming". 2014. Maker Media Inc. pp 155-165.

* "Engineer, Wandering". *Arduino Pin Change Interrupts*. 2014. Web page: https://thewanderingengineer.com/2014/08/11/arduino-pin-change-interrupts/. Accessed Sept 17, 2022. 




 
