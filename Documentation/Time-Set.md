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
  <li><a href="#setClockMode">setClockMode&#40;&#41;</a></li>
  <li><a href="#setSecond">setSecond&#40;&#41;</a></li>
  <li><a href="#setMinute">setMinute&#40;&#41;</a></li>
  <li><a href="#setHour">setHour&#40;&#41;</a></li>
  <li><a href="#setDoW">setDoW&#40;&#41;</a></li>
  <li><a href="#setDate">setDate&#40;&#41;</a></li>
  <li><a href="#setMonth">setMonth&#40;&#41;</a></li>
  <li><a href="#setYear">setYear&#40;&#41;</a></li>
  <li><a href="#setEpoch">setEpoch&#40;&#41;</a></li>
</ul>


<h3 id="setClockMode">void setClockMode(bool h12)</h3>

```
/*
 * returns: nothing (void)
 * parameters: bool = true or false
 * effect: sets the clock mode, 12-hour (`true`) or 24-hour (`false`), in the DS3231
 * DS3231 register addressed: 0x02
 */

// Illustrate passing the clock mode as a boolean variable
bool mode12 = true; // use 12-hour clock mode
myRTC.setClockMode(mode12); // uploads 'true' (1) to bit 6 of register 0x02
```

The clock mode should certainly be set prior to setting the hour. In other words, invoke this function before invoking ```setHour()``` or ```setEpoch()```. Best practice would be simply to set the mode before setting the date and time.


<h3 id="setSecond">void setSecond(byte Second)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 0 to 59
 * effects: 
 *   1. writes the seconds to the DS3231
 *   2. clears the Oscillator Stop Flag in the DS3231 hardware
 * DS3231 registers addressed: 0x00, 0x0F
 */

// Illustrate passing the seconds value in a variable
byte theSecond = 42; // 42 seconds
myRTC.setSecond(theSecond); // uploads 42 to register 0x00
// side-effect: also clears bit 7 of register 0x0F
```

Note: the oscillator stop flag is an informational data item indicating that the DS3231's oscillator stopped at some point in the past. The flag does not affect the operation of the DS3231 hardware. Clearing the flag makes good housekeeping sense when setting the time.

<h3 id="setMinute">void setMinute(byte Minute)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 0 to 59
 * effects: writes the minutes to the DS3231
 * DS3231 register addressed: 0x01
 */

// Illustrate passing the minutes as a literal value
myRTC.setMinute(17); // uploads 17 to register 0x01
```

<h3 id="setHour">void setHour(byte Hour)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 0 to 23 (use 24-hour mode here)
 * effect: writes the hour to the DS3231
 * DS3231 register addressed: 0x02
 */

// Illustrate passing the hour in a variable
byte theHour = 19; // equal to 7:00 p.m.
myRTC.setHour(theHour); // uploads 19 to register 0x02
```

The setHour() function will convert the hour to 12-hour mode if the DS3231 is set to operate in 12-hour mode. The function does not change how the mode is set on the DS3231 hardware. 

Rather, the mode should be set by another function, setClockMode(), prior to invoking the setHour() function.

<h3 id="setDoW">void setDoW(byte DoW)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 1 to 7
 * effect: writes the Day of Week to the DS3231
 * DS3231 register addressed: 0x03
 */

// Illustrate passing the Day of the Week as a literal value
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

<h3 id="setDate">void setDate(byte Date)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 1 to 28, 29, 30, or 31, depending on the month and year
 * effect: writes the day of the month to the DS3231
 * DS3231 register addressed: 0x04
 */

// Illustrate passing the date in a variable
byte theDate = 5; // the 5th day of the month
myRTC.setDate(theDate); // uploads 5 to register 0x04
```

Reminder: it is the code developer's responsibility to ensure that a sensible value be supplied for the date.

<h3 id="setMonth">void setMonth(byte Month)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 1 to 12
 * effect: writes the month to the DS3231
 * DS3231 register addressed: 0x05
 */

// Illustrate passing the month in a variable
byte theMonth = 6; // the 6th month of the year; in English, June
myRTC.setMonth(theMonth); // uploads 6 to register 0x05
```

<h3 id="setYear">void setYear(byte Year)</h3>

```
/*
 * returns: nothing (void)
 * parameters: byte = 00 to 99
 * effect: writes the year to the DS3231
 * DS3231 register addressed: 0x06
 */

// Illustrate passing the year as a literal value
myRTC.setYear(22); // uploads 22 to register 0x06
```

<h3 id="setEpoch">void setEpoch(time_t epoch = 0, bool flag_localtime = false)</h3>

Comes now the peculiar concept of the "Epoch" as the word is used by people who write code. It is peculiar because they do not always use it to mean the same thing.

Some people insist it means "The UNIX Epoch", specified as a certain point in the four-dimensional continuum of space/time, namely the time 00:00:00 on the date January 1, 1970, of the calendar instituted by the Roman Pope Gregory XIII, at the Greenwich (England) Meridian of longitude on the planet Earth.

But not everyone. There is a an old saying, "Wherever you find two programmers, you'll find three opinions."

So, here we have a function in the DS3231 Library that can set the time and date on a DS3231 by deriving the relevant values from something called an Epoch. In this context, Epoch refers to an unsigned long (positive, 32-bit) integer containing a value equal to the number of seconds that have elapsed since time 00:00:00 on January 1, 2000, *in the particular time zone chosen by the programmer*.

Trust me: plenty of sincere, intelligent programmers will earnestly disagree with that interpretation of Epoch. Which is fine. I don't argue with them. I wish them all the joy in the world. However, my tests indicate that seconds-since-2000 is the way to go when setting the time you expect to see on your DS3231 using this function.

Keep in mind that DS3231 has no concept of time zones or daylight savings time. Your code will have to supply a value for Epoch that makes sense in your situation, for the purposes you intend. A worked example is provided in the following code listing.

```
/*
 * returns: nothing (void)
 *
 * parameters: (optional; they both default to 0)
 *   1. a time_t-type value (unsigned long) 
 *      containing a "epoch" time value 
 *      defined as the number of seconds 
 *      that have elapsed since time 00:00:00, 
 *      on the date January 1, 2000,
 *      in the time zone relevant to your application.
 *   2. a boolean variable indicating whether
 *      to interpret the value as "local time" (true)
 *      or as GMT time (false).
 *
 * side effects: 
 *   calculates and uploads values 
 *   into the DS3231 registers for 
 *   seconds, minutes, hours, 
 *   day of week (where Sunday = 1),
 *   date, month and year.
 *
 * DS3231 registers addressed: 0x00 through 0x06
 *
 * Worked Example
 * Suppose a web site tells you that the "epoch" time 
 * at 12:25:00 p.m. GMT on August 7, 2022 was 1659875100.
 * Subtract the number of seconds from 1/1/1970 to 1/1/2000.
 * 1659875100 - 946684800 = 713190300
 * Oh, you also want that to be in your own local time zone?
 * Example, U.S. Central Daylight Time = GMT - 5 hours.
 * 713190300 - ( 5 hours x 3600 seconds/hour ) = 713172300
 */
// Initialize a (unsigned long) variable to contain that value.
time_t epochNow = 713172300UL;
// Set the time on the DS3231 using that variable.
myRTC.setEpoch(epochNow); 

```

By the way, your friendly neighborhood Documentarian could not detect any effect when testing the second parameter with an Arduino Nano. The function produced the same date and time settings on the DS3231 for both "true" and "false". One might consider omitting the second parameter, as shown above.

The reader is encouraged to experiment with this function. Approach it playfully and check the results until you feel satisfied with your own understanding of what to expect from it.

The DS3231 data sheet mentions that the device can track leap years accurately "up to (the year) 2100." A 32-bit unsigned integer can number enough seconds to get through the year 2135. Perhaps that capacity will suffice for most present-day needs.

If you want to read something funny, look up the so-called "2036 Problem" affecting UNIX Time. How on  Earth did the smart people who fixed the infamous Y2K problem turn right around and engineer themselves into the 2036 Problem?

There are actually four of these Problems: 2036, 2038, 2108 and 2108. 

Happily, the DS3231 and the 1/1/2000 approach taken by this Library appear well positioned to go on keeping accurate time during and beyond the years 2036 and 2038. 

After 2099? Not our problem. The kids will have changed everything by then anyway.
