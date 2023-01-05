# DS3231 Library
## Time Retrieval Functions
The following methods of the DS3231 object use I2C hardware connection and the Wire library to read data from certain registers in the DS3231. The Library assumes that the DS3231 has an I2C address of 0x68.

The DS3231 register addresses mentioned below are documented on page 11 of the [manufacturer's datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231-DS3231S.pdf).

The examples provided below assume a variable has been declared as follows:
```DS3231 myRTC;```

<ul>
  <li><a href="#getSecond">getSecond&#40;&#41;</a></li>
  <li><a href="#getMinute">getMinute&#40;&#41;</a></li>
  <li><a href="#getHour">getHour&#40;&#41;</a></li>
  <li><a href="#getDow">getDoW&#40;&#41;</a></li>
  <li><a href="#getDate">getDate&#40;&#41;</a></li>
  <li><a href="#getMonth">getMonth&#40;&#41;</a></li>
  <li><a href="#getYear">getYear&#40;&#41;</a></li>

<h3 id="getSecond">getSecond&#40;&#41;</h3>

```
/*
 * returns: byte = 0 to 59
 * parameters: none
 * asserts: none
 * side effects: none
 * DS3231 register addressed: 0x00
 */

byte theSecond = myRTC.getSecond();
```

<h3 id="getMinute">getMinute&#40;&#41;</h3>

```
/*
 * returns: byte = 0 to 59
 * parameters: none
 * asserts: none
 * side effects: none
 * DS3231 register addressed: 0x01
 */

byte theMinute = myRTC.getMinute();
```

<h3 id="getHour">getHour&#40;&#41;</h3>

```
/*
 * returns: byte, value depending on mode settings in the DS3231
 *   either 1 to 12 (12-hour mode)
 *   or 0 to 23 (24-hour mode) 
 * parameters: two boolean variables, passed by reference
 *   parameter #1: 12/24-hour flag
 *   parameter #2: AM/PM flag
 *   Note: must provide the variable names, not constants
 * asserts: none
 *
 * side effects: 
 *   the two boolean parameters are set or cleared 
 *   according to the state of the corresponding flags 
 *   in the DS3231 hardware register
 *     12/24 hour flag = true if DS3231 is in 12-hour mode
 *     AM/PM flag = false if AM, true if PM, in 12-hour mode
 *
 * DS3231 register addressed: 0x02
 */

// declare global variables to be passed into the function
bool h12;
bool hPM;

byte theHour = myRTC.getHour(h12, hPM);

// example of printing the hour to the Serial monitor
Serial.print("The hour is ");
Serial.print( theHour ); // the value returned by the function

// test the values altered by side-effects of the function
if (h12 == true) { // 12-hour mode
  if (hPM == true) {
    Serial.println(" PM.");
  } else {
    Serial.println(" AM.");
  }
} else { // 24-hour mode
  Serial.println(" in 24-hour mode.");
}
```

Note that supplying boolean constants as parameters will halt program compilation with an error. The parameters must be the names of boolean variables defined in the program code.

<h3 id="getDoW">getDoW&#40;&#41;</h3>

```
/*
 * returns: byte = 1 to 7
 * parameters: none
 * asserts: none
 * side effects: none
 * DS3231 register addressed: 0x03
 */

byte theWeekday = myRTC.getDoW();
```

Note that the meaning of the day-of-week value is determined by the user when the time is *set* on the DS3231. See the documentation for setDoW(). In other words, "1" can signify any day of the week that the code writer chooses it to mean when setting the time. The values "2" through "7" then refer to the succeeding days, in their usual order.

<h3 id="getDate">getDate&#40;&#41;</h3>

```
/*
 * returns: byte = 1 to 28, 29, 30 or 31, depending on the month and year
 * parameters: none
 * asserts: none
 * side effects: none
 * DS3231 register addressed: 0x04
 */

byte theDate = myRTC.getDate();
```

<h3 id="getMonth">getMonth&#40;&#41;</h3>

```
/*
 * returns: byte = 1 to 12
 * parameters: one boolean variable, passed by reference
 * asserts: none
 * side effects: 
 *   the boolean parameter is set or cleared
 *   according to the value of the "Century" flag
 *   in the hardware register of the DS3231
 * DS3231 register addressed: 0x05
 */

// declare a variable to receive the Century bit
bool CenturyBit;

byte theDate = myRTC.getMonth(CenturyBit);
```

Note: according to the datasheet, "The century bit (bit 7 of the month register) is toggled when the years register overflows from 99 to 00."

Note also that supplying a boolean constant value of *true* or *false* as the parameter will halt program compilation with an error. The parameter must be the name of a boolean variable defined in the program code.

The "Contemplations", below, further discuss the Century Bit.

<h3 id="getYear">getYear&#40;&#41;</h3>

```
/*
 * returns: byte = 00 to 99
 * parameters: none
 * asserts: none
 * side effects: none
 * DS3231 register addressed: 0x06
 */

byte theDate = myRTC.getYear();
```

### Contemplations of An Aging Documentarian 

The Century bit may supply useful information when operating the DS3231 near the end of a century. For example, the bit would have toggled when the year changed from 1999 to 2000. It would have been important to recognize that a year "00" actually represented an *increase* of time compared to the year "99".

The bit will toggle again when the year changes from 2099 to 2100, and so forth.

For reasons best understood by its designers, the Century bit is stored in the "month" register of the DS3231, rather than in the "year" register.

It might have been nicer if the DS3231 afforded the capacity to maintain a 4-digit year value.

We users of the device might find little use for the Century bit during the years 2000 through 2098 or so. Anyone planning to use this Library with a DS3231 in the year 2099 may wish to experiment with code to evaluate and correctly use the Century bit. 

My beard will probably not grow long enough for me to reach that future era. Even so, by then I would probably look for a different RTC chip. The reason is the DS3231 makes no promise to handle Leap Years correctly in or after the year 2100.


