# DS3231 Library
## DateTime
DateTime is a C++ class included within this DS3231 Library. Program code can store and manipulate date and time information in DateTime variables.

Date and time information can be entered in a variety of formats, not only as numbers but also as specially formatted strings.

The library even provides a function to transfer date and time data from the DS3231 hardware into a DateTime object, where it can be accessed for subsequent analysis and calculations.

Methods (another name for Functions) of a DateTime variable enable programs to access date and time data in different number formats. The values for years, months, days, hours, minutes and seconds can be retrieved either individually or as a "timestamp", a large integer number of seconds, and vice versa.

Timestamps make it easy to do arithmetic with dates and times.

DateTime variables afford convenient data management and are easy to use. Even so, please keep one limitation in mind when writing them into your code:

---

*DateTime objects are defined in this Library to work with dates of 01 January 2000 through 31 December 2099, only*.

---

## Contents

* [DateTime As a Data Type](#datetime-as-a-data-type)
* [DateTime() As a Function ](#datetime-as-a-function)
* [Uses and Limitations of the Timestamp](#uses-and-limitations-of-the-timestamp)

## DateTime As a Data Type
Simply use the class name as the type to declare a DateTime variable, for example:

```DateTime myDT;```

Date and time data can be stored in a DateTime variable by adding a parameter list to the declaration. The parameter list will invoke one of the "constructor functions" to initialize the variable's date and time values.

The following examples initialize three ways to initialize a DateTime named ```myDT``` so that it contains 4:50:59 P.M. on Sep 08 2022 16:50:59. 

### Example using strings

```DateTime myDT("Sep 08 2022", "16:50:59");```

The constructor function prototype is:

```DateTime(const char* date, const char* time);```

* Strings must be formatted as "Mmm dd yyyy" and "hh:mm:ss". 
* Valid month abbreviations are: "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov" and "Dec". 
* The year must be four characters long. 
* Space delimiters are required in the date, as are the colon delimiters in the time.  
* Hours in the time are given in 24-hour format, i.e., "00" through "23".

### Example using six integer values

```DateTime myDT(2022, 09, 08, 16, 50, 59);```

The constructor function prototype is:

```
DateTime (uint16_t year, uint8_t month, uint8_t day,
          uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
```

* The year, month and day parameters are required.
* The time parameters are optional and are given default values of zero (0).
* The value for the year can be entered in the range 2000 through 2099, or in the range 0 through 99. 
* The year will be stored internally as an offset from 2000. In the example above, 22 would be stored.
* It is the programmer's responsibility to provide sensible values for all of the parameters.

### Example using a Unix-style timestamp

```DateTime myDT(1662655859);```

The constructor function prototype is:

```DateTime myDT(uint32_t t);```

Note that this constructor function will treat the timestamp fed into it as a Unix-style timestamp, that is, the number of seconds elapsed since midnight (time 00:00:00) the morning of January 1, 1970.

As the DateTime class is limited by design to the years 2000 through 2099, the range of unambiguous values for timestamps to be used with this constructor includes:

* minimum: 946684801  = 00:00:01 (a.m.) January 1, 2000
* maximum: 4102444799 = 23:59:59 (p.m.) December 31, 2099

However, a DateTime variable does not keep track of which time zone its data actually represent. It makes no allowance for Daylight Savings Time, either.

The program writer is responsible for adjusting the date and time information, if necessary to respect a certain time zone.

---

<p style="font-style: italic;">Regardless of the method used to create it, DateTime variables store the components of date and time internally as six, distinct, 8-bit, unsigned integers:</p>

<ul style="font-style: italic;">
  <li>yOff : offset from year 2000, i.e., 00 through 99</li>
  <li>m : month, 1 through 12</li>
  <li>d : date of the month, 1 through 28, 29, 30 or 31, depending on the month and year</li>
  <li>hh : hour, 00 through 23, in 24-hour format</li>
  <li>mm : minute, 00 through 59</li>
  <li>ss : second, 00 through 59</li>
</ul>

---

## DateTime() As a Function
Constructor functions are called automatically when a new variable is declared, as shown above. 

Your code can call a constructor directly, also, when it wants to update the values stored in a DateTime variable. Simply set the old variable equal to the return value of the chosen constructor function.

For example, consider the variable named ```myDT``` that was declared in the preceding examples. It was initialized to contain a date of September 8. Here is how to change the date to September 10, while holding all the other values constant (by repeating their previous values):

```myDT = DateTime(2022, 09, 10, 16, 50, 59);```

Compare the parameter list above to the one used to declare the variable: (2022, 09, 08, 16, 50, 59).

Notice that changing even one date or time value in a DateTime requires providing relevant information about all of the values.

This Library provides a fourth function designed to transfer date and time information directly from a DS3231 device into a DateTime variable. Its prototype is:

```DateTime RTClib::now()```

The following example transfers data from a DS3231 device into the ```myDT``` variable, assuming the DS3231 is in communication with the program via I2C:

```myDT = RTClib::now();```

Look closely at the name of that function. Write it just that way in your code.

### Summary

---

*Use a constructor function to modify the values stored inside a DateTime variable.*

---

## Retrieving Date and Time Data
DateTime objects provide seven different methods for retrieving their information. None of the methods takes any parameters. 

The function names and the types of values they return are listed below:

* ```uint16_t year(); // returns 2000 + yOff```
* ```uint8_t month();```
* ```uint8_t day();```
* ```uint8_t hour();```
* ```uint8_t minute();```
* ```uint8_t second();```
* ```uint32_t unixtime(); // returns the date and time in the format of a Unix Timestamp```

The data access methods listed above are invoked from a DateTime variable with the dot (".") operator. For example, the following code segment would print the time information contained within a DateTime object named ```myDT```.

```
  Serial.print(myDT.hour());
  Serial.print(":");
  Serial.print(myDT.minute());
  Serial.print(":");
  Serial.println(myDT.second());
```

## Uses and Limitations of the Timestamp

### Uses
The timestamp returned by the *unixtime()* method of a DateTime is an unsigned, 32-bit integer. It can be very useful for adding or subtracting an interval of time measured in seconds.

Suppose you want to add one day to to an existing DateTime variable. It could be cumbersome to do this by adjusting first the date, then perhaps the month, then perhaps the year.

A better procedure might be:

1. retrieve the DateTime data as a timestamp, 
2. add 86,400 seconds, then 
3. use the timestamp constructor to update the variable. 

The following code segment illustrates the procedure:

```
#define SECONDS_IN_ONE_DAY 86400
uint32_t timeStamp = myDT.unixtime();
timeStamp += SECONDS_IN_ONE_DAY;
myDT = DateTime(timeStamp);
```

### Limitations
DateTime variables do not maintain information about time zones. 

The idea of a timestamp originated in the notion of a "system time" accumulated inside a computer running the Unix operating system. It was just an integer that began at zero when the computer started up. It would keep count of how many seconds the computer had been running. A timestamp is just the value of the system time at a given instant.

A "Unix Epoch" timestamp is like a system time that began at zero, exactly at 00:00:00 o'clock midnight on the morning of January 1, 1970 on the Prime Meridian of longitude, which runs through Greenwich, a suburb of London, England. 

Hence, Unix Timestamps represent Greenwich Meridian Time, or GMT. Adjusting a Unix timestamp to other time zones involves adding or subtracting a suitable number of seconds.

DateTime relaxes that definition. It deals with *Unix-style* timestamps. That is, it follows the Unix algorithm for converting between dates/times and timestamps for GMT, but makes no adjustment for a time zone. 

A timestamp retrieved by the *unixtime()* method of a DateTime variable is best understood as just a (rather large) number of seconds.

### Fun Facts about Unix Timestamps
Did you know that some systems using *signed 32-bit values* for Unix Timestamps may require re-configuration before the 21st Century grows much older? For entertaining reading, look up "the 2038 Problem." 

Fortunately, the DateTime class defined in this Library uses *unsigned 32-bit values* for its timestamps. In this way it postpones the 2038 Problem beyond December 2099, perhaps even through the year 2137.

Keep in mind, however: the DateTime class defined in this Library is designed to work correctly only with dates between January 1, 2000 and December 31, 2099.

