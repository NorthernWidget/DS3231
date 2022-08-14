# DS3231 Library
An Arduino library for the DS3231 real-time clock (RTC).  

## Description
The library provides easy-to-use methods to:

* set and read the current date and time;
* set, clear, and detect two, independent, Time-of-Day alarms;
* perform certain conversions and calculations with time data;
* manage certain hardware inside the DS3231 RTC module.

This document explains the installation and usage of the Library with the Arduino IDE. 

You do have to install the Library in your Arduino IDE environment before you can use it. Installation instructions are provided, below.

## Contents

* [Summary](#summary)
* [How to Cite](#how-to-cite)
* [About the DS3231](#about-the-ds3231)
* [How to Install the Library](#installation)
* [Functions Provided in the Library](#functions)
* [Examples of Using the Library](#examples-of-use)
* [Helpful Resources](#additional-resources-and-references)
* [Contributing, Credits and License](#contributing)
* [To-Do List](#to-do)


<hr>

## Summary

After installing the Library in your Arduino IDE, using it in a program starts with three, simple steps:

<ol start="1"> 
  <li>Import the Library into the program code:</li>
</ol>


```
#include <DS3231.h>
```

<ol start="2">
  <li>Declare a DS3231 object, for example:</li>
</ol>


```
DS3231 myRTC;
```

<ol start="3">
  <li>Start the Wire library to enable I2C communications with the DS3231 hardware, typically in the setup() code block:</li>
</ol>


```
Wire.begin();
```

Then, Library functions are typically accessed through the DS3231 object. For example, to read the current date of the month (1 through 28, 29, 30 or 31, depending on the month and the year):


```
unsigned int theDate = myRTC.getDate();
```

The Library defines two other classes to assist with managing date and time data:

* ```DateTime``` enables a versatile object for managing date and time data. A variable of the DateTime type can represent a specific date and time in two different ways:
    1. as distinct values for year, month, day, hour, minute and second, or
    2. as a single, unsigned integer. The latter is handy for doing arithmetic with dates.
* ```RTClib``` institutes a convenient ```RTClib::now()``` function for receiving a date/time snapshot, as a DateTime object, from the DS3231 device.

## How to Cite

If you use this library in a publicatoin, please cite it in one or both of the following two ways:
1. Through the `CITATION.cff` file here, which should be up to date with the permanent archive available from Zenodo
2. If you need an academic journal reference and/or you are discussing the integration of the DS3231 into a larger hardware + firmware ecosystem,<br/>
**Wickert, A. D., Sandell, C. T., Schulz, B., & Ng, G. H. C. (2019), [Open-source Arduino-compatible data loggers designed for field research](https://hess.copernicus.org/articles/23/2065/2019/), *Hydrology and Earth System Sciences*, *23*(4), 2065-2076, doi:10.5194/hess-23-2065-2019.**<br/>
This option should not be the only one used because it does not credit the original library developer, Eric Ayars.

## About the DS3231
DS3231 is a low-cost integrated circuit (IC) providing a highly accurate, real time clock for use with Arduino, Raspberry Pi, BBC micro:bit and other popular small computing devices. 

The IC is typically mounted on a circuit board or module, along with other hardware, such as header pins, supportive electrical components, and even EEPROM memory chips, for convenient attachment to a breadboard or an Arduino. 

Several different modules are available from a number of competing vendors. This Library aspires to work with any DS3231 module that supports I2C communications with the IC.

DS3231 runs independently and can be kept running for a considerable length of time by a small, backup battery, even if power to the Arduino is turned off.

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

Data for the time and date are stored in memory locations on the DS3231. Each, distinct value is stored separately. This means the seconds are in one location, the minutes in another, and so forth. The DS3231 updates the values in the memory locations every second.

The device keeps track of time by operating its own 32.768 kHz crystal oscillator, similar to the timekeeper in an electronic watch. Temperature can affect oscillator speed. Accordingly, the DS3231 takes further steps to maintain accuracy. It senses the temperature around the crystal and adjusts the speed of the oscillator.

The temperature can be read from the DS3231 using a Library function. The data sheet declares it to be accurate to within 3 degrees, Celsius. 

### Power Supply and Battery Backup
The DS3231 can run in a range between 2.3 volts and 5.5 volts. The device actually has two power supply pins: the primary source, V<sub>CC</sub>, and a secondary, backup source, V<sub>BAT</sub>.

Some popular modules mounting a DS3231 provide a receptacle for a coin battery, attaching it to the V<sub>BAT</sub> pin. If a sufficiently-charged battery is present, the DS3231 will switch automatically to the battery after detecting a drop in V<sub>CC</sub> voltage below a certain "power-fail" level.

It will switch back to V<sub>CC</sub> automatically, if and when that voltage rises back up above both the power-fail and the battery voltage level. 

One point regarding the choice of battery may deserve consideration: the question of whether to install a rechargeable coin battery, or to disable a charging circuit if such a thing is provided on the module being used. The topic is controversial and the authors of this Library do not express any opinion about it. Readers may choose to search online for more information.

[back to top](#ds3231-library)
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

The user must also ensure that two, other, required libraries are available to the Arduino IDE. This DS3231 library takes care to ```#include```  the following in a program, but it does not install them in your Arduino IDE:

* Wire.h : a widely-used Arduino library for I2C communications
* time.h : a modified C language header file packaged with avr-libc and the AVR-GCC compiler

Note: At the time of writing, both of these libraries were included by default with a standard installation of the 1.8.x version of Arduino IDE for AVR-based devices.

A simple way to check for the availability of the two libraries is to compile the following, blank Arduino sketch. If the IDE does not complain that anything is missing, then the required libraries are available for use with this DS3231 library.

```
#include <Wire.h>
#include <time.h>
void setup() {}
void loop() {}
```

[back to top](#ds3231-library)
<hr>

## Functions
Future development of this document may include specific pages for the functions listed here. 
Presently, additional information about the functions is available in the [Examples of Use](#examples-of-use) described below, and in the source files in this repository:

* [DS3231.h](https://github.com/NorthernWidget/DS3231/blob/master/DS3231.h)
* [DS3231.cpp](https://github.com/NorthernWidget/DS3231/blob/master/DS3231.cpp)

### Read the Date or Time
- *RTClib::now()* <sup>\*</sup>
- getSecond()
- getMinute()
- getHour()
- getDoW()
- getDate()
- getMonth()
- getYear()

\* The *RTClib::now()* function is not accessed through the DS3231 object. Rather, it has a very specific syntax as described below in <a href="#RTClib_now_function">The Special <code>RTClib::now&#40;&#41;</code> Function</a>.


### Set the Date or Time
- setEpoch()
- setSecond()
- setMinute()
- setHour()
- setDoW()
- setDate()
- setMonth()
- setYear()
- setClockMode()

### Set, Clear and Check Alarms
- getA1Time()
- getA2Time()
- setA1Time()
- setA2Time()
- turnOnAlarm()
- turnOffAlarm()
- checkAlarmEnabled()
- checkIfAlarm()

### Manage DS3231 Hardware
- getTemperature()
- enableOscillator()
- enable32kHz()
- oscillatorCheck()
- readControlByte()
- writeControlByte()

The above list covers for interacting with the DS3231 hardware. Those listed below provide read-only access to information contained inside a DateTime object variable declared in program code.

### DateTime Object
- year()
- month()
- day()
- hour()
- minute()
- second()
- dayOfTheWeek()
- unixtime()

<h3 id="RTClib_now_function">The Special <code>RTClib::now()</code> Function </h3>

```RTClib::now()``` is the precise, complete name for a special function that returns a ```DateTime``` object from the DS3231. Always write it just so: ```RTClib::now()```.

The function returns a DateTime object. To use it in your program, declare a DateTime type of variable to receive the value. For example, 

```DateTime currentMoment = RTClib::now();```

The value of ```currentMoment``` can then be accessed as either:
* an unsigned integer containing the number of seconds since a certain reference date, or
* distinct values for Year, Month, Day, Date, Hour, Minute, or Second.

[back to the list of functions](#functions)<br>
[back to top](#ds3231-library)
<hr>

## Examples of Use

There are many examples provided in the [examples](https://github.com/NorthernWidget/DS3231/tree/master/examples) folder of this repository.  At the time of writing the examples include:

* *set*: demonstrates selected time-setting functions
* *test*: demonstrates selected time-reading functions
* *echo*: demonstrates setting the time and date then reading it back
* *echo_time*: similar to *echo*, demonstrates setting and reading time/date data
* *oscillator_test*: demonstrates advanced techniques for managing and using the DS3231 device as a pulse generator

Future development plans include updating these examples and adding more of them.

See also [Working with the DS3231 libraries and interrupts](https://github.com/IowaDave/RTC-DS3231-Arduino-Interrupt), a tutorial provided by [IowaDave](https://github.com/IowaDave).

[back to top](#ds3231-library)
<hr>

## Additional Resources and References

* [Maxim DS3231 Data Sheet](https://datasheets.maximintegrated.com/en/ds/DS3231-DS3231S.pdf)

[back to top](#ds3231-library)
<hr>

## Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell others about this library
- Contribute new protocols

Please read [CONTRIBUTING.md](https://github.com/NorthernWidget/DS3231/blob/master/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

[back to top](#ds3231-library)
<hr>

## Credits
This is a splice of [Ayars'](http://hacks.ayars.org/2011/04/ds3231-real-time-clock.html) and [Jeelabs/Ladyada's](https://github.com/adafruit/RTClib) libraries.

The authors of this library are A. Wickert <awickert@umn.edu>, E. Ayars, J. C. Wippler, N. W. LLC <info@northernwidget.com> and it is maintained by A. Wickert. This library is released into the public domain by Jeelabs, Ladyada, and E. Ayars.

Based on previous work by:

- S. T. Andersen
- SimGas
- Per1234
- Glownt

[back to top](#ds3231-library)
<hr>

## License

DS3231 is licensed under [The Unlicense](https://github.com/NorthernWidget/DS3231/blob/master/LICENSE).

[back to top](#ds3231-library)
<hr>

## To Do
A project is underway to update the library's documentation.

### Existing Repo Issues to be addressed:
* \#42 Alarm Documentation
* \#24 DateTime Comparison Operators
* \#20 AlarmBits Documentation

