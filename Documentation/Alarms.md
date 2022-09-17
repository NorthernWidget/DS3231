#DS3231 Library
## Functions for Alarms
The DS3231 offers two, versatile, independent alarms. 

Every second, a DS3231 updates the values in its "time" registers: second, minute, hour, day, date, month and year.

The hardware then instantly compares the new time values to corresponding values stored in the two alarm registers: second, minute, hour, and either day or date depending on a certain alarm setting.

When the values match for one of the alarms, the hardware writes a "flag" bit for that alarm to HIGH, that is, to logic 1.

What happens after that flag bit goes HIGH is... anything the developer can imagine!

This Library provides functions for setting, managing and detecting the alarms. 

Optionally, the DS3231 can be configured to interrupt the Arduino when an alarm occurs. An Arduino program can execute specific instructions immediately upon receiving the interrupt.

Interrupts from a DS3231 enable an Arduino to perform time-sensitive tasks very accurately, without the need for any ```delay()``` statements, polling or timing loops running on the Arduino!

An [example program](https://github.com/NorthernWidget/DS3231/tree/master/examples/AlarmInterrupt) demonstrates using an alarm-triggered interrupt to blink an LED.

## Contents

* [Arduino Code Requirements](#arduino-code-requirements)
* [Alarm Bits Quick Reference](#alarm-bits-quick-reference)
* [The DS3231 Library Alarm Functions](#alarm-functions)
    * [getA1Time()](#get-a1-time)
    * [getA1Time() with Option](#get-a1-time-option)
    * [getA2Time()](#get-a2-time)
    * [getA2Time() with Option](#get-a2-time-option)
    * [setA1Time()](#set-a1-time)
    * [setA2Time()](#set-a2-time)
    * [turnOnAlarm()](#turn-on-alarm)
    * [turnOffAlarm()](#turn-off-alarm)
    * [checkAlarmEnabled()](#check-alarm-enabled)
    * [checkIfAlarm()](#check-if-alarm)
    * [checkIfAlarm() with Option](#check-if-alarm-option)
* [Alarm Bits in Detail](#alarm-bits-in-detail)
* [How to Advance an Alarm Time](#how-to-advance-an-alarm-time)
* [How (and Why) to Prevent an Alarm Entirely](#prevent-alarm)

## Arduino Code Requirements
A program needs certain software resources to work with DS3231 alarms. 

This includes a set of variables that are required in the parameter lists by some of the alarm methods. The listing below identifies the resources and suggests descriptive names a programmer could choose for the variables.

All of the code examples in this article will use the variables by these names in parameter lists, including those of functions that could otherwise use numeric constants.

```
#include <DS3231.h>  // import this Library
DS3231 myRTC;        // declare an object for access to the alarm methods

// declare variables to use with the alarm methods

// for the hour of an alarm
bool alarmH12;       // (true) if hour is 1 - 12, (false) if hour is 00 - 23
byte alarmHour;      // 1-12 if 12-hour mode, 0-23 if 24-hour mode
bool alarmPM;        // (true) if 12-hour time is PM, (false) if AM

byte alarmMinute;    // 00 - 59
byte alarmSecond;    // 00 - 59

// for the day or date of an alarm
byte alarmDay;       // 1-7 if day of week, 1-31 if date in month
bool alarmIsDay;     // (true) if alarmDay is a day of the week, (false) if date in month

// for the frequency of an alarm
byte alarmBits;      // a bitfield, to be explained below

void setup() {
  Wire.begin();      // establish I2C communications
}
```

The following code fragment illustrates setting Alarm 1 to activate at 12:00 noon once per week, on Mondays. It makes use of the software resources described above.

```
// assign values to program variables
alarmH12 = true;  // use 12-hour time mode
alarmHour = 12; // ambiguous value in 12-hour time mode
alarmPM = true; // determines that 12 means "noon"
alarmMinute = 0;
alarmSecond = 0;
// The following assignment will be correct only if
// the programmer determined 1 to mean Sunday
// when setting the time in the DS3231 clock registers.
alarmDay = 2; // 2 = Monday
// alarmDay to be interpreted as day in the week, not as date in the month
alarmIsDay = true; 
alarmBits = 0b00000000; // alarm when day, hours, minutes and seconds match the time

// write to the DS3231 time registers for Alarm 1
myRTC.setA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmIsDay, alarmH12, alarmPM);
```

[Back to Contents](#contents)

## Alarm Bits Quick Reference
Two of the alarm values shown in the foregoing example combine to determine the frequency of an alarm:

* alarmIsDay, and
* alarmBits.

In total, eleven different combinations of values exist for these two parameters. The following table lists the combinations and explains the result that each combination produces.

| |alarmIsDay | alarmBits |Explanation|
|--:|:------:|--------|-----------|
|1| (ignored) | 0b00001111 | Alarm 1 every second. All other settings for Alarm 1 are ignored.|
|2| (ignored) | 0b00001110 | Alarm 1 every minute, when the Alarm 1 seconds value matches the seconds value of the time. All other settings for Alarm 1 are ignored.|
|3| (ignored) | 0b00001100 | Alarm 1 every hour, when the Alarm 1 minutes and seconds values match those of the time. All other settings for Alarm 1 are ignored.|
|4| (ignored) | 0b00001000 | Alarm 1 every day, when the Alarm 1 hours, minutes and seconds values match those of the time. The time modes, 24- or 12-hour and AM/PM, are evaluated.|
|5| True<br>(Day of Week) | 0b00000000 | Alarm 1 every week, when the Alarm 1 day, hours, minutes and seconds values match those of the time. The time modes, 24- or 12-hour and AM/PM, are evaluated.|
|6| False<br>(Date in Month) | 0b00000000 | Alarm 1 every month, when the Alarm 1 date, hours, minutes and seconds values match those of the time. The time modes, 24- or 12-hour and AM/PM, are evaluated.|
|7| (ignored) | 0b01110000 | Alarm 2 every minute, when the seconds value of the time equals zero. All other settings for Alarm 2 are ignored.|
|8| (ignored) | 0b01100000 | Alarm 2 every hour, when the Alarm 2 minutes value matches that of the time and the time seconds value equals zero. All other settings for Alarm 2 are ignored.|
|9| (ignored) | 0b01000000 | Alarm 2 every day, when the Alarm 2 hours and minutes values match those of the time and the time seconds value equals zero. The time modes, 24- or 12-hour and AM/PM, are evaluated.|
|10| True<br>(Day in Week) | 0b00000000 | Alarm 2 every week, when the Alarm 2 day, hours and minutes values match those of the time and the time seconds value equals zero. The time modes, 24- or 12-hour and AM/PM, are evaluated.|
|11| False<br>(Date in Month) | 0b00000000 | Alarm 2 every month, when the Alarm 2 date, hours and minutes values match those of the time and the time seconds value equals zero. The time modes, 24- or 12-hour and AM/PM, are evaluated.|

Notice that combinations 5 and 6 appear identical to 11 and 12. Fortunately, the methods for setting Alarm 1 and Alarm 2 prevent confusion. Each method sets values for just one of the alarms.

[Back to Contents](#contents)

## Alarm Functions
The illustrations in this section assume that the program has already declared a DS3231 object named ```myRTC``. For example:

```DS3231 myRTC; // create a DS3231 object```

---

### <a id="get-a1-time">getA1Time()</a>

```
/*
 * Retrieves values from the Alarm 1 time registers then stores the values
 * in the external variables passed to the parameter list.
 * NOTE: previous values of the parameter variables are over-written.
 * returns: nothing (void)
 * parameters (all of these are references to external variables):
 *   byte& A1Day,      // "byte&" refers to a byte variable
 *   byte& A1Hour,     // 
 *   byte& A1Minute,   //
 *   byte& A1Second,   //
 *   byte& AlarmBits,  //
 *   bool& A1Dy,       // "bool&" refers to a boolean variable
 *   bool& A1h12,      //
 *   bool& A1PM        //
 */
void DS3231::getA1Time(
   byte& A1Day, byte& A1Hour, byte& A1Minute, 
   byte& A1Second, byte& AlarmBits, bool& A1Dy, 
   bool& A1h12, bool& A1PM);
```

The variables passed in the parameter list must match the types of the parameters but need not match the names.

[Back to Contents](#contents)

---

### <a id="get-a1-time-option">getA1Time() with Option</a>


```
/*
 * Retrieves values from the Alarm 1 time registers then stores the values
 * in the external variables passed to the parameter list.
 * NOTE: previous values of the parameter variables are over-written.
 * Overloads getA1Time() to include clearing the AlarmBits parameter variable
 * returns: nothing (void)
 * parameters (the following are references to external variables):
 *   byte& A1Day,        // "byte&" refers to a byte variable
 *   byte& A1Hour,       // 
 *   byte& A1Minute,     //
 *   byte& A1Second,     //
 *   byte& AlarmBits,    //
 *   bool& A1Dy,         // "bool&" refers to a boolean variable
 *   bool& A1h12,        //
 *   bool& A1PM,         //
 *   // the additional parameter is passed by value,
 *   // meaning that a literal true or false keyword may be used
 *   bool clearAlarmBits // (true) = clear the AlarmBits parameter 
 */
void DS3231::getA1Time(
   byte& A1Day, byte& A1Hour, byte& A1Minute, 
   byte& A1Second, byte& AlarmBits, bool& A1Dy, 
   bool& A1h12, bool& A1PM, bool clearAlarmBits);
```

The variables passed in the parameter list must match the types of the parameters but need not match the names.

#### Managing the Retrieved AlarmBits Value
Remember that the variable chosen to receiving the AlarmBits values is an 8-bit "bitfield". 

* The bits for alarm 1 occupy positions 0-3 in the bitfield. 
* Those for alarm 2 are found in positions 4-6. Bit 7 has no meaning.

Each of the alarm "get" functions addresses only the bits *for that alarm*. This means that the variable receiving the AlarmBits value will wind up containing:

* the actual bits for the targeted alarm in one-half of the field, plus
* any leftover bits that might have been lingering in the other half.

Any one of the following alternative approaches can guard against irrelevant bits cluttering the AlarmBits variable. The illustrations assume the variable holding the bitfield is named "AlarmBits".

Approach 1. *After* calling "get", use the bitwise & operator to distill only the desired bits.

* Alarm 1: ```AlarmBits &= 0b00001111;```
* Alarm 2: ```AlarmBits &= 0b01110000;```

Approach 2. *Before* calling "get", clear the bitfield by assigning 0 to the variable.

* ```AlarmBits = 0;```

Approach 3. Set the optional "clearAlarmBits" boolean parameter in the "get" function to "true".

* "true" tells the "get" function to perform Approach 2 internally.
* "false" or omitted leaves management of the bitfield in the hands of the program writer.

[Back to Contents](#contents)

---

### <a id="get-a2-time">getA2Time()</a>

```
/*
 * Retrieves values from the Alarm 2 time registers then stores the values
 * in the external variables passed to the parameter list.
 * NOTE: previous values of the parameter variables are over-written.
 * returns: nothing (void)
 * parameters (all of these are references to external variables):
 *   byte& A2Day,      // "byte&" refers to a byte variable
 *   byte& A2Hour,     // 
 *   byte& A2Minute,   //
 *   byte& AlarmBits,  //
 *   bool& A2Dy,       // "bool&" refers to a boolean variable
 *   bool& A2h12,      //
 *   bool& A2PM        //
 */
void DS3231::getA2Time(
   byte& A2Day, byte& A2Hour, byte& A2Minute, 
   byte& A2Second, byte& AlarmBits, bool& A2Dy, 
   bool& A2h12, bool& A2PM);
```

The variables passed in the parameter list must match the types of the parameters but need not match the names.

[Back to Contents](#contents)

---

### <a id="get-a2-time-option">getA2Time() with Option</a>

```
/*
 * Retrieves values from the Alarm 2 time registers then stores the values
 * in the external variables passed to the parameter list.
 * NOTE: previous values of the parameter variables are over-written.
 * Overloads getA2Time() to include clearing the AlarmBits parameter variable
 * returns: nothing (void)
 * parameters (the following are references to external variables):
 *   byte& A2Day,        // "byte&" refers to a byte variable
 *   byte& A2Hour,       // 
 *   byte& A2Minute,     //
 *   byte& A2Second,     //
 *   byte& AlarmBits,    //
 *   bool& A2Dy,         // "bool&" refers to a boolean variable
 *   bool& A2h12,        //
 *   bool& A2PM,         //
 *   // the additional parameter is passed by value,
 *   // meaning that a literal true or false keyword may be used
 *   bool clearAlarmBits // (true) = clear the AlarmBits parameter 
 */
void DS3231::getA1Time(
   byte& A2Day, byte& A2Hour, byte& A2Minute, 
   byte& A2Second, byte& AlarmBits, bool& A2Dy, 
   bool& A2h12, bool& A2PM, bool clearAlarmBits);
```

The variables passed in the parameter list must match the types of the parameters but need not match the names.

See the discussion of [Managing the Retrieved AlarmBits Value](#managing-the-retrieved-alarmbits-value), above, for more information about the purpose and usage of this optional method.


[Back to Contents](#contents)

---

### <a id="set-a1-time">setA1Time()</a>


```
/*
 * transfers the parameter values to Alarm 1 registers inside the DS3231
 * returns: nothing (void)
 * parameters:
 *   byte A1Day, 
 *   byte A1Hour, 
 *   byte A1Minute, 
 *   byte A1Second
 *   byte AlarmBits, 
 *   bool A1Dy, 
 *   bool A1h12, 
 *   bool A1PM
 *
 * depends upon I2C connection to DS3231 but does not validate it
 */
void DS3231::setA1Time(
   byte A1Day, byte A1Hour, byte A1Minute, 
   byte A1Second, byte AlarmBits, bool A1Dy, 
   bool A1h12, bool A1PM);
```

Alarm 1 has a parameter for seconds of time. By contrast, Alarm 2 (below) does not.

[Back to Contents](#contents)

### <a id="set-a2-time">setA2Time()</a>

```
/*
 * transfers the values to Alarm 2 registers inside the DS3231
 * returns: nothing (void)
 * parameters:
 *   byte A2Day, 
 *   byte A2Hour, 
 *   byte A2Minute, 
 *   byte AlarmBits, 
 *   bool A2Dy, 
 *   bool A2h12, 
 *   bool A2PM
 */
void DS3231::setA2Time(
   byte A2Day, byte A2Hour, byte A2Minute, 
   byte AlarmBits, bool A2Dy, bool A2h12, 
   bool A2PM);
```

Alarm 2 does not have a parameter for seconds of time. By contrast, Alarm 1 (above) does.

[Back to Contents](#contents)

---

### <a id="turn-on-alarm">turnOnAlarm()</a>

```
/*
 * Modifies control register 0x0E of DS3231 to enable the chosen alarm interrupt
 * returns: nothing (void)
 * one parameter:
 *     Alarm, 1 or 2, determines which alarm interrupt to enable
*/

void turnOnAlarm(byte Alarm);
```

* Enables the interrupt for alarm 1 if and only if the Alarm parameter is equal to 1.
* For all other values of the Alarm parameter, it sets the interrupt for alarm 2.
* The Alarm parameter is required.

Consider clearing both of the alarms' flags after enabling an interrupt, even if only one alarm is in use. 

The reason is that the DS3231 cannot signal an interrupt if *either one of the flags* is set HIGH, that is, equal to logic 1.

See the discussion of [checkIfAlarm()](#check-if-alarm), below.

```
myRTC.turnOnAlarm(1); // enable alarm 1 interrupt
myRTC.checkIfAlarm(1); // clear alarm 1 flag
myRTC.checkIfAlarm(2); // better yet, clear both of the flags
```


[Back to Contents](#contents)

---

### <a id="turn-off-alarm">turnOffAlarm()</a>

```
/*
 * Modifies control register 0x0E of DS3231 to disable the chosen alarm interrupt
 * returns: nothing (void)
 * one parameter:
 *     Alarm, 1 or 2, determines which alarm interrupt to disable
 */

void turnOffAlarm(byte Alarm) ;
```

* Disables the interrupt for alarm 1 if and only if the Alarm parameter is equal to 1.
* For all other values of the Alarm parameter, it disables the interrupt for alarm 2.
* The Alarm parameter is required.

Disabling interrupts for one of the alarms has no effect on the other alarm.

[Back to Contents](#contents)

---

### <a id="check-alarm-enabled">checkAlarmEnabled()</a>

```
/*
 * Retrieves the interrupt status bit of the chosen alarm.
 * returns: boolean, 
 *     true = bit is set (enabled), 
 *     false = bit is clear (disabled)
 * one parameter:
 *     Alarm, 1 or 2, determines which alarm to check
 */

bool checkAlarmEnabled(byte Alarm);
```

[Back to Contents](#contents)

---

### <a id="check-if-alarm">checkIfAlarm()</a>

```
/*
 * Retrieves the value of the alarm flag for the chosen alarm
 * returns: boolean,
 *     (true) if the flag is set = logic 1, 
 *     (false) if the flag is clear = logic 0
 * only one parameter:
 *     Alarm, 1 or 2, determines which alarm flag to examine
 * side effect:
 *     Clears the alarm flag after examining it, also.
 */

bool checkIfAlarm(byte Alarm);
```

NOTE TO PROGRAMMERS: this version of the method always overwrites the value of the alarm flag to zero. An alternate version, below, allows programs an option to preserve the value of the flag in the DS3231.

It may be desirable to do so in programs that re-use a single alarm repeatedly.

The flag value is returned to the program, where it may be assigned to a program variable for later evaluation and use. 

Keep two things in mind:

1. The alarm flag *must* be reset to zero (cleared) by the program before a subsequent alarm event can be detected. 
2. This method is the only one in the Library that can clear an alarm flag with certainty.

[Back to Contents](#contents)

---

### <a id="check-if-alarm-option">checkIfAlarm() with Option</a>

```
/*
 * Retrieves the value of the alarm flag for the chosen alarm.
 * Adds a parameter allowing the alarm flag to be preserved in the DS3231.
 * returns: boolean,
 *     (true) if the flag is set = logic 1, 
 *     (false) if the flag is clear = logic 0
 * two parameters:
 *     Alarm, 1 or 2, determines which alarm flag to examine
 *     clearFlag, determines whether to clear the alarm flag
 *         (true) clears the flag
 *         (false) preserves the value of the flag in the DS3231
 * side effect:
 *     Clears the alarm flag only if the clearFlag parameter is equal to true
 */

bool checkIfAlarm(byte Alarm, bool clearFlag);
```

This version of the method may be preferable when both alarms are in use, with interrupts enabled,  and the program desires to preserve the flags in the hardware temporarily. 

Version 2 performs exactly like Version 1, above, clearing the flag after checking, when the "clearFlag" parameter is equal to "true".

Note that this version cannot guarantee to clear the alarm flag automatically, because it will not clear the flag if the program passes a value of "false" to the second parameter.

[Back to Contents](#contents)

---

## Alarm Bits in Detail

Here is more about the magic of DS3231 alarms: the alarmBits parameter. It needs careful understanding and rewards attentive study. 

It is a *bitfield*, that is, an array of individual bits, where each bit has a certain, definite meaning. 

Actually, the AlarmBits parameter should be understood as a *pair of bitfields*. The least significant four bits, 0 through 3, regulate Alarm 1, while bits 4-6 apply to Alarm 2. Bit 7 is disregarded.

The parameter looks like an 8-bit integer but has no practical meaning as such. Instead, *each, individual bit* enables or disables a specific feature of an alarm in the DS3231. 

The following table illustrates the arrangement of the two bitfields in the AlarmBits parameter.

|Bit7|Alarm 2|Alarm 1|
|----|-------|-------|
| X | bits 6:4 | bits 3:0 |

**Important, for Alarm 2**: locate the alarm bits in positions 6, 5 and 4 of the AlarmBits parameter for the functions that set and retrieve values for Alarm 2. This is addressed in greater detail below.

#### Locating the Alarm Bits

It will be helpful to name the seven bits. Inside the DS3231, each bit is stored separately, as Bit 7 in each of the 8-bit data registers storing values for the alarm.

##### Alarm 1
|Register|Bit7|Bits 6:0|
|--------|----|--------|
| 0x07 | A1M1 |Second|
| 0x08 | A1M2 |Minute|
| 0x09 | A1M3 |Hour|
| 0x0A | A1M4 |Day|

##### Alarm 2
|Register|Bit7|Bits 6:0|
|--------|----|--------|
| 0x0B | A2M2 |Minute|
| 0x0C | A2M3 |Hour|
| 0x0D | A2M4 |Day|

By contrast, the bits are stored adjacently in the parameter. It is very convenient for programming purposes to arrange them into a single byte this way. 

Note again that Bit 7 *in the parameter* is marked "X", which means it is ignored.

##### alarmBits Parameter Map

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
| X |A2M4|A2M3|A2M2|A1M4|A1M3|A1M2|A1M1|

Clearly, the alarm setting and retrieving functions perform an elaborate but beneficial operation with these bits. They translate the bits from adjacent positions in a bitfield (the AlarmBits parameter) to separate registers in the DS3231, and vice versa.

#### Alarm Bits Determine Alarm Rate
The bits combine with the *alarmIsDay* parameter to form a "mask" that governs the Rate of an alarm. That is, how often, on which occasions, an alarm will occur.

The following table is adapted from Table 2 on page 12 of the [data sheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf). The column labeled *DY/DT* reflects the boolean value of the *alarmIsDay* parameter listed above. An "X" means the value of DY/DT is ignored.

##### Alarm 1
<table>
  <tr>  <!-- Top header row -->
    <th rowspan="2">DY/DT</th>
    <th colspan="4">Alarm 1 Mask Bits</th>
    <th rowspan="2">Alarm Rate</th>
  </tr>
  <tr>  <!-- Second header row -->
    <th>A1M4</th>
    <th>A1M3</th>
    <th>A1M2</th>
    <th>A1M1</th>
  </tr>
  <tr>  <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>1</td>
    <td>1</td>
    <td>1</td>
    <td>Alarm once per second</td>
  </tr>
  <tr>  <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>1</td>
    <td>1</td>
    <td>0</td>
    <td>Alarm when seconds match</td>
  </tr>
  <tr>  <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>1</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when minutes and seconds match</td>
  </tr>
  <tr> <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when hours, minutes and seconds match</td>
  </tr>
  <tr>  <!-- Data row -->
    <td style="text-style:italic;">true</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when day, hours, minutes and seconds match</td>
  </tr>
  <tr>  <!-- Data row -->
    <td style="text-style:italic;">false</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when date, hours, minutes and seconds match</td>
  </tr>
</table>  

##### Alarm 2
<table>
  <tr>  <!-- Top header row -->
    <th rowspan="2">DY/DT</th>
    <th colspan="3">Alarm 2 Mask Bits</th>
    <th rowspan="2">Alarm Rate</th>
  </tr>
  <tr>  <!-- Second header row -->
    <th>A2M4</th>
    <th>A2M3</th>
    <th>A2M2</th>
  </tr>
  <tr>  <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>1</td>
    <td>1</td>
    <td>Alarm when seconds == 0 (once per minute) </td>
  </tr>
  <tr>  <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>1</td>
    <td>0</td>
    <td>Alarm when minutes match</td>
  </tr>
  <tr>  <!-- Data row -->
    <td>X</td>
    <td>1</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when hours and minutes match</td>
  </tr>
  <tr>  <!-- Data row -->
    <td style="text-style:italic;">true</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when day, hours and minutes match</td>
  </tr>
  <tr>  <!-- Data row -->
    <td style="text-style:italic;">false</td>
    <td>0</td>
    <td>0</td>
    <td>0</td>
    <td>Alarm when date, hours and minutes match</td>
  </tr>
</table>  

The binary number format might be the most clear way to spell out a constant value for the AlarmBits parameter. The following example would prepare a parameter writing bits 6 and 5 to logic 1, and bit 4 to logic 0, so that Alarm 2 would occur when the minutes value of the time first matches that of the alarm time. It will be used in the example for advancing the alarm time, in the next section.

```byte AlarmBits = 0b01100000; // alarm when minutes match```

[Back to Contents](#contents)

## How to Advance an Alarm Time
**Problem**: your program needs to execute a procedure at precise, ten-minute intervals. You would like to have the DS3231 generate an interrupt every ten minutes. The DS3231 hardware supports automatically repeating interrupts at intervals of one second, one minute, one hour, one day, one week or one month, but not ten minutes.

**Solution**: write program code to advance the alarm time by ten minutes after an alarm interrupt is received.

The code fragment listed below illustrates an approach that could be used for the purpose. It makes use of the versatile DateTime class and other resources that accompany the DS3231 class in this Library.

Please keep two thoughts in mind.

1. Interrupts are automatically disabled globally by AVR chip hardware when entering an Interrupt Service Routine (ISR).
2. DS3231 class methods use I2C for communications with the DS3231. I2C requires interrupts. For best results, code that needs to communicate with a DS3231 should not be executed while an ISR is running.

For this example to work, the ISR could be as short as one instruction: simply set a designated global boolean variable "true". The change can notify the idle process (called "loop()" in Arduino code) to execute the following code fragment in response to the interrupt.

```
// capture the time the alarm occurred
DateTime alarmTime = RTClib::now();
// disable Alarm 2 interrupt output
myRTC.turnOffAlarm(2);

// We need to clear both of the alarm flags
// before DS3231 can output another alarm interrupt.

// Capture Alarm 1 flag for later assessment
// and automatically clear Alarm 1 flag
bool alarm1Flag = myRTC.checkIfAlarm(1);

// Clear Alarm 2 flag. No need to retain its value.
myRTC.checkIfAlarm(2);

// prepare parameter values for setting a new alarm time
alarmBits = 0b01100000; // Alarm 2 when minutes match
alarmH12 = false; // interpret hour in 24-hour mode
alarmPM = false; // irrelevant in 24-hour mode, but it needs a value
alarmIsDay = false; // interpret "day" value as a date in the month

// add 600 seconds (10 minutes)
uint32_t nextAlarm = alarmTime.unixtime() + 600;
// update values in the DateTime
alarmTime = DateTime(nextAlarm);

// Set the next time for Alarm 2.
// Note that only the "minutes" value is significant,
// yet we must supply the day and hour also,
// and it does no harm to supply real ones.

myRTC.setA2Time (
  // get these values from the DateTime object
  alarmTime.day(), // from a DateTime, will be date of the month
  alarmTime.hour(), // from a DateTime, will be in 24-hour format
  alarmTime.minute(),
  // these were assigned, above, by the program
  alarmBits,
  alarmIsDay,
  alarmH12,
  alarmPM
  );

// enable Alarm 2 interrupt output
myRTC.turnOnAlarm(2);
```

An [example program](https://github.com/NorthernWidget/DS3231/tree/master/examples/AdvanceAlarm) demonstrates using an alarm-triggered interrupt to blink an LED and print a message to the screen at 3-second intervals.

[Back to Contents](#contents)

## <a id="prevent-alarm">How (and Why) to Prevent an Alarm Entirely</a>

An alarm that your program is not using can covertly block the output of an interrupt by the DS3231. 

The unused alarm can produce this unwanted effect silently. All it takes is for a "match" to occur between the time registers and the settings in effect for the unused alarm.

When a match occurs, the DS3231 will write logic 1 to the flag bit for the unused alarm. While that flag remains at logic 1, it will prevent the DS3231 from sending out a FALLING interrupt signal.

It is not enough merely to clear the unused alarm flag. A better plan would be to prevent the flag from ever being raised in the first place.

One way to do it is to break a rule: assign a nonsensical value to an alarm time register. Select a value that cannot match the time. 255 (0xFF) will fit nicely in an 8-bit unsigned integer. The following code fragment uploads 0xFF to the A2Minute register. 

Then it assigns the value 0b01100000 to the AlarmBits parameter, which means to activate Alarm 2 when the alarm minutes value matches that of the time. The match will never occur because the minutes value of time will not exceed 59.

```
    // these parameters need to be provided
    // but their values will not be evaluated
    alarmDay = 1;
    alarmHour = 1;
    alarmDayIsDay = false;
    alarmH12 = false;
    alarmPM = false

    // these parameters may prevent Alarm 2 from activating
    alarmMinute = 0xFF; // a value that will never match the time
    alarmBits = 0b01100000; // Alarm 2 when minutes match, in this case, never
    
    // Upload the parameters to prevent Alarm 2 entirely
    myRTC.setA2Time(
        alarmDay, alarmHour, alarmMinute,
        alarmBits, alarmDayIsDay, alarmH12, alarmPM);

    // disable Alarm 2 interrupt
    myRTC.turnOffAlarm(2);

    // clear Alarm 2 flag
    myRTC.checkIfAlarm(2);
```
[Back to Contents](#contents)


