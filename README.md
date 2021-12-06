[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4540372.svg)](https://doi.org/10.5281/zenodo.4540372)

# DS3231

## Description

Arduino library for the DS3231 real-time clock (RTC). Abstracts functionality for clock reading, clock setting, and alarms for the DS3231 high-precision real-time clock. This is a splice of [Ayars'](http://hacks.ayars.org/2011/04/ds3231-real-time-clock.html) and [Jeelabs/Ladyada's](https://github.com/adafruit/RTClib) libraries.

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

## Requirements

This library depends on the wire header file. To use this library functions, the user has to include **wire.h** (header) file.

## Features

- ### Easy Interface

    DS3231 library has a very easy to use interface

- ### Low-cost

    DS3231 is a low-cost module

- ### Accuracy

    DS3231 is an extremely accurate I2C realtime clock (RTC) with an integrated temperature-compensated crystal oscillator (TCXO) and crystal.

- ### Time Fomat

    The clock operates in either the 24-hour or 12-hour format with an AM/PM indicator.

- ### Information

    The RTC maintains seconds, minutes, hours, day, date, month, and year information. The date at the end of the month is automatically adjusted for months with fewer than 31 days, including corrections for leap year.

## Functions

- date2days()
- time2long()
- unixtime()
- bcd2bin()
- bin2bcd()
- now()
- getSecond()
- getMinute()
- getHour()
- getDoW()
- getDate()
- getMonth()
- getYear()
- setEpoch()
- setSecond()
- setMinute()
- setHour()
- setDoW()
- setDate()
- setMonth()
- setYear()
- setClockMode()
- getTemperature()
- getA1Time()
- getA2Time()
- setA1Time()
- setA2Time()
- turnOnAlarm()
- turnOffAlarm()
- checkAlarmEnabled()
- checkIfAlarm()
- enableOscillator()
- enable32kHz()
- oscillatorCheck()
- decToBcd()
- bcdToDec()
- readControlByte()
- writeControlByte()

## Example

There are many examples implemented where this library is used. You can find other examples from [Github-DS3231](https://github.com/NorthernWidget/DS3231/tree/master/examples)

## Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell others about this library
- Contribute new protocols

Please read [CONTRIBUTING.md](https://github.com/NorthernWidget/DS3231/blob/master/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Credits

The authors of this library are A. Wickert <awickert@umn.edu>, E. Ayars, J. C. Wippler, N. W. LLC <info@northernwidget.com> and it is maintained by A. Wickert. This library is released into the public domain by Jeelabs, Ladyada, and E. Ayar.

Based on previous work by:

- S. T. Andersen
- SimGas
- Per1234
- Glownt

## License

DS3231 is licensed under [The Unlicense](https://github.com/NorthernWidget/DS3231/blob/master/LICENSE).
