# DS3231 Library - Examples

This directory contains examples demonstating code usage. This is a good place to start, if you wish to get to know this library.

You can find examples on the following topics:
- [Examples on Times](#examples-on-times)
- [Examples on Alarms](#examples-on-alarms)
- [Other Examples](#other-examples)

## Examples on Times
- **[now](/examples/now/now.ino)**: Basic example of reading back the current time with `RTClib::now()`.
- **[echo_time](/examples/echo_time/echo_time.ino)**: Demonstration of read back functions for the DS3231 RTC with output to serial monitor.
- **[DS3231_set](/examples/DS3231_set/DS3231_set.ino)**: Demonstration of set-time routines for a DS3231 RTC.
- **[set_echo](/examples/set_echo/set_echo.ino)**: Sets the time from input and prints back time stamps for 5s.
- **[DS3231_test](/examples/DS3231_test/DS3231_test.ino)**: Full demonstration of DS3231 RTC functions with print back to serial monitor.

## Examples on Alarms
- **[AlarmPolling](/examples/AlarmPolling/AlarmPolling.ino)**: Basic alarm example demonstrating setting and reading an alarm.
- **[AlarmInterrupt](/examples/AlarmInterrupt/AlarmInterrupt.ino)**: Using DS3231 alarms with interrupts via the SQW output.
- **[AdvanceAlarm](/examples/AdvanceAlarm/AdvanceAlarm.ino)**: Periodic alarm at an arbitrary interval in time based on interrupts.

## Other Examples
- **[DS3231_oscillator_test](/examples/DS3231_oscillator_test/DS3231_oscillator_test.ino)**: Output of 32 kHz signal via SQW pin.
