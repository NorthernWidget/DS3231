# DS3231 Library
An Arduino library for the DS3231 real-time clock (RTC).  

## Description
The library provides easy-to-use methods to:

* set and read the current date and time;
* set, clear, and detect two, independent, Time-of-Day alarms;
* perform certain conversions and calculations with time data;
* manage certain hardware inside the DS3231 RTC module.

This document explains the installation and usage of the Library with the Arduino IDE.

## Links to Useful Information

* [About the DS3231](#about-the-ds3231)
* [How to Install the Library](#installation)
* [Functions Provided in the Library](#functions)
* [Examples of Using the Library](#examples-of-use)
* [Helpful Resources](#additional-resources-and-references)
* [To-Do List](#to-do)

<hr>

## Functions

### Functions to Set the Time
- setEpoch()
- setSecond()
- setMinute()
- setHour()
- setDoW()
- setDate()
- setMonth()
- setYear()
- setClockMode()

### Functions to Read the Time
- now()
- getSecond()
- getMinute()
- getHour()
- getDoW()
- getDate()
- getMonth()
- getYear()

### Functions to Set, Clear and Check Alarms
- getA1Time()
- getA2Time()
- setA1Time()
- setA2Time()
- turnOnAlarm()
- turnOffAlarm()
- checkAlarmEnabled()
- checkIfAlarm()

### Functions to Perform Calculations on Time Data
- date2days()
- time2long()
- unixtime()
- bcd2bin()
- bin2bcd()
- decToBcd()
- bcdToDec()

### Functions to Manage DS3231 Hardware
- getTemperature()
- enableOscillator()
- enable32kHz()
- oscillatorCheck()
- readControlByte()
- writeControlByte()

[back to top](#d3231-library)
<hr>

## Examples of Use

There are many examples provided in the [examples](https://github.com/NorthernWidget/DS3231/tree/master/examples) folder of this repository. 

See also [Working with the DS3231 libraries and interrupts](https://github.com/IowaDave/RTC-DS3231-Arduino-Interrupt), a tutorial provided by [IowaDave](https://github.com/IowaDave).

[back to top](#d3231-library)
<hr>

## About the DS3231
DS3231 is a low-cost, highly accurate real time clock module for attachment to Arduino, Raspberry Pi, BBC micro:bit and other popular small computing devices. 

It runs independently and can be kept running for a considerable length of time by a small, backup battery, even if power to the Arduino is turned off.

According to the [datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231-DS3231S.pdf), the DS3231 hardware "completely manages all timekeeping functions (including):

* Seconds, 
* Minutes, 
* Hours
    * 12-hour format with AM/PM indication, or
    * 24-hour format,
* Day of the Week,
* Date of the Month, 
* Month,  and
* Year, with Leap-Year Compensation Valid Up to 2100"

[back to top](#d3231-library)
<hr>

## Installation

### First Method

![image](https://user-images.githubusercontent.com/36513474/68038497-eae68200-fceb-11e9-9812-b47ff6e06e2f.png)

1. In the Arduino IDE, navigate to Sketch > Include Library > Manage Libraries
1. Then the Library Manager will open and you will find a list of libraries that are already installed or ready for installation.
1. Then search for DS3231 using the search bar.
1. Click on the text area and then select the specific version and install it.

### Second Method

1. Navigate to the [Releases page](https://github.com/NorthernWidget/DS3231/releases).
1. Download the latest release.
1. Extract the zip file
1. In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library

### Dependencies

The user must also ensure that two, other, required libraries are available to the Arduino IDE. This DS3231 library depends upon the following, but does not supply them:

* Wire.h : a widely-used Arduino library for I2C communications
* time.h : a standard C language library

Note: At the time of writing, both of these libraries were included with a standard installation of the 1.8.x version of Arduino IDE.

A simple way to check for the availability of the two libraries is to compile the following, blank Arduino sketch. If the IDE does not report any error, then the required libraries are available for use with this DS3231 library.

```
#include <Wire.h>
#include <time.h>
void setup() {}
void loop() {}
```

[back to top](#d3231-library)
<hr>

## Additional Resources and References

* [Maxim DS3231 Data Sheet](https://datasheets.maximintegrated.com/en/ds/DS3231-DS3231S.pdf)

[back to top](#d3231-library)
<hr>

## To Do
A project is underway to update the library's documentation.

### Existing Repo Issues to be addressed:
* \#42 Alarm Documentation
* \#24 DateTime Comparison Operators
* \#20 AlarmBits Documentation

[back to top](#d3231-library)
<hr>

## Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell others about this library
- Contribute new protocols

Please read [CONTRIBUTING.md](https://github.com/NorthernWidget/DS3231/blob/master/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

[back to top](#d3231-library)
<hr>

## Credits
This is a splice of [Ayars'](http://hacks.ayars.org/2011/04/ds3231-real-time-clock.html) and [Jeelabs/Ladyada's](https://github.com/adafruit/RTClib) libraries.

The authors of this library are A. Wickert <awickert@umn.edu>, E. Ayars, J. C. Wippler, N. W. LLC <info@northernwidget.com> and it is maintained by A. Wickert. This library is released into the public domain by Jeelabs, Ladyada, and E. Ayars.

Based on previous work by:

- S. T. Andersen
- SimGas
- Per1234
- Glownt

[back to top](#d3231-library)
<hr>

## License

DS3231 is licensed under [The Unlicense](https://github.com/NorthernWidget/DS3231/blob/master/LICENSE).
