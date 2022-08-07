# DS3231 Library
## Time Setting Functions
The following methods of the DS3231 object use I2C hardware connection and the Wire library to transfer data into certain hardware registers of the DS3231. The Library assumes that the DS3231 has an I2C address of 0x68.

The DS3231 register addresses mentioned below are documented on page 11 of the [manufacturer's datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231-DS3231S.pdf).

The examples provided below assume a variable has been declared as follows:
```DS3231 myRTC;```

### Note to Developers
It is the code developer's sole responsibility to upload only "sensible" values to the DS3231. For example: 

* 0 to 59 for minutes or seconds, 
* 1 to 7 for Day of the Week, 
* 30 but not 31 for the month of June. 

The Library does not check the values to be set. The compiler only checks that the value is of the right size, i.e., 8 bits. A large value such as 117 will fit in 8 bits and would be passed along by the Library. Yet it makes no sense for any part of a time or a date. Code with care!

<ul>
  <li><a href="#setSecond">setSecond&#40;&#41;</a></li>
  <li><a href="#setMinute">setMinute&#40;&#41;</a></li>
  <li><a href="#setHour">setHour&#40;&#41;</a></li>
  <li><a href="#setDow">setDoW&#40;&#41;</a></li>
  <li><a href="#setDate">setDate&#40;&#41;</a></li>
  <li><a href="#setMonth">setMonth&#40;&#41;</a></li>
  <li><a href="#setYear">setYear&#40;&#41;</a></li>
  <li><a href="#setClockMode">setClockMode&#40;&#41;</a></li>
  <li><a href="#setEpoch">setEpoch&#40;&#41;</a></li>
</ul>

<h3 id="setSecond">setSecond&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 0 to 59
 * asserts: none
 * side effects: 
 *   1. writes the seconds to the DS3231
 *   2. clears the Oscillator Stop Flag in the DS3231 hardware
 * DS3231 registers addressed: 0x00, 0x0F
 */

// Illustrate passing the seconds value in a variable
byte theSecond = 42; // 42 seconds
myRTC.setSecond(theSecond); // uploads 42 to register 0x00
// side-effect also clears bit 7 of register 0x0F
```

Note: the oscillator stop flag is an informational data item indicating that the DS3231's oscillator stopped at some point in the past. The flag does not affect the operation of the DS3231 hardware. Clearing the flag makes good housekeeping sense when setting the time.

<h3 id="setMinute">setMinute&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 0 to 59
 * asserts: none
 * side effects: writes the minutes to the DS3231
 * DS3231 register addressed: 0x01
 */

// Illustrate passing the minutes as a literal value
myRTC.setMinute(17); // uploads 17 to register 0x01
```

<h3 id="setHour">setHour&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 0 to 23 (must be in 24-hour mode)
 * asserts: none
 * side effects: writes the hour to the DS3231
 * DS3231 register addressed: 0x02
 */

// Illustrate passing the hour in a variable
byte theHour = 19; // equal to 7:00 p.m.
myRTC.setHour(theHour); // uploads 19 to register 0x02
```

The setHour() function will convert the hour to 12-hour mode if the DS3231 is set to operate in 12-hour mode. The function does not change how the mode is set on the DS3231 hardware. 

Rather, the mode should be set by another function, setClockMode(), prior to invoking the setHour() function.

<h3 id="setDoW">setDoW&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 1 to 7
 * asserts: none
 * side effects: writes the Day of Week to the DS3231
 * DS3231 register addressed: 0x03
 */

// Illustrate passing the DAy of the Week as a literal value
myRTC.setDoW(1); // uploads 1 to register 0x03
```

The Day of Week value is user-determined. For example, if one chooses Sunday to be the first day of the week, then the DoW values would be:

1. Sunday
2. Monday
3. Tuesday. and so forth.

On the other hand, if Monday were selected as the start of a week, then:

1. Monday
2. Tuesday
3. Wednesday, etcetera.

<h3 id="setDate">setDate&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 1 to 28, 29, 30, or 31, depending on the month and year
 * asserts: none
 * side effects: writes the date of the month to the DS3231
 * DS3231 register addressed: 0x04
 */

// Illustrate passing the date in a variable
byte theDate = 5; // the 5th day of the month
myRTC.setDate(theDate); // uploads 5 to register 0x04
```

It is the code developer's responsibility to ensure that a sensible value be supplied for the date.

<h3 id="setMonth">setMonth&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 1 to 12
 * asserts: none
 * side effects: writes the month to the DS3231
 * DS3231 register addressed: 0x05
 */

// Illustrate passing the month in a variable
byte theMonth = 6; // the 6th month of the year; in English, June
myRTC.setMonth(theMonth); // uploads 6 to register 0x05
```

<h3 id="setYear">setYear&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 00 to 99
 * asserts: none
 * side effects: writes the year to the DS3231
 * DS3231 register addressed: 0x06
 */

// Illustrate passing the year as a literal value
myRTC.setYear(22); // uploads 22 to register 0x06
```

<h3 id="setClockMode">setClockMode&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: bool = true or false
 * asserts: none
 * side effects: sets the clock mode, 12-hour or 24-hour, in the DS3231
 * DS3231 register addressed: 0x02
 */

// Illustrate passing the clock mode as a boolean variable
bool h12 = true; // use 12-hour clock mode
myRTC.setClockMode(h12); // uploads 'true' (1) to bit 6 of register 0x02
```

The clock mode should be set prior to setting the hour. In other words, invoke this function before invoking ```setHour()```.

<h3 id="setEpoch">setEpoch&#40;&#41;</h3>

```
/*
 * returns: nothing (void)
 * parameters: two parameters are required
 *   1. a time_t-type value (unsigned long) 
 *      containing a time value as a number of seconds
 *   2. a boolean variable indicating whether
 *      parameter #1 is measured from 1/1/1970 (false)\
 *      or from 1/1/2000 (true)
 *
 * asserts: none
 * side effects: calculates and uploads values 
 *   into the DS3231 registers for 
 *   seconds, minutes, hours, day of week,
 *   date, month and year.
 * DS3231 registers addressed: 0x00 through 0x06
 */

```

Your friendly neighborhood Documentarian does not (yet) understand this function. The implementation code in the DS3231.cpp module calls functions defined in the time.h library. If you are reading this confession, it means he has not figured it out yet.

A clever person might imagine using this function as a short-cut for setting all of the DS3231 time values without writing seven or eight individual commands. Think again. There is another way to do that, taking advantage of the DateTime object. See the documentation for DateTime Functions.


