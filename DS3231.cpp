/*
    DS3231.cpp: DS3231 Real-Time Clock library
*/

#include "DS3231.h"

// These included for the DateTime class inclusion; will try to find a way to
// not need them in the future...
#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

/**
 * @brief Construct a new DS3231::DS3231 object
 * 
 */
DS3231::DS3231() : _Wire(Wire) {
	// nothing to do for this constructor.
}

/**
 * @brief Construct a new DS3231::DS3231 object
 * 
 * @param w reference of twoWire
 */
DS3231::DS3231(TwoWire &twowire) : _Wire(twowire) {
}

/*****************************************
	Public Functions
 *****************************************/

/*******************************************************************************
 * TO GET ALL DATE/TIME INFORMATION AT ONCE AND AVOID THE CHANCE OF ROLLOVER
 * DateTime implementation spliced in here from Jean-Claude Wippler's (JeeLabs)
 * RTClib, as modified by Limor Fried (Ladyada); source code at:
 * https://github.com/adafruit/RTClib
 ******************************************************************************/

/**
 * @brief Construct a new Date Time:: Date Time object
 * 
 * @param timestamp 
 */
DateTime::DateTime (time_t unix_timestamp)
: _unix_timestamp{unix_timestamp}, _y2k_timestamp{unix_timestamp - UNIX_OFFSET}
{
	gmtime_r(&_unix_timestamp, &_tm);
}

/**
 * @brief Construct a new Date Time:: Date Time object
 * 
 * @param year year e.g. 2022
 * @param month months since January - [ 1...12 ]
 * @param day day of the month - [ 1...31 ]
 * @param hour hours since midnight - [ 0...23 ]
 * @param min inutes after the hour - [ 0...59 ]
 * @param sec seconds after the minute - [ 0...59 ]
 * @param wday wdays since Sunday - [ 1...7 ]
 * @param dst Daylight Saving Time flag
 */
DateTime::DateTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t min, int8_t sec, int8_t wday, int16_t yday, int16_t dst)
{
    _tm.tm_sec = sec;
    _tm.tm_min = min;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month-1;
    _tm.tm_year = year-1900;
    _tm.tm_yday = yday;
    _tm.tm_wday = wday-1;
    _tm.tm_isdst = dst;

    set_timstamps();
}


/**
 * @brief Construct a new Date Time:: Date Time object by givin the precompiler marcos
 * as __DATE__ and __TIME__
 * 
 * @param date as  Mmm dd yyyy (e.g. "Jan 14 2012")
 * @param time as HH:MM:SS (e.g. "23:59:01")
 */
DateTime::DateTime(const char *date, const char *time) {
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    static char month_buff[4] = {'0','0','0','0'};
    int year, day;
    sscanf(date, "%s %2d %4d", month_buff, &day, &year);
    int month = (strstr(month_names, month_buff) - month_names) / 3 + 1;
    _tm.tm_year = year-1900;
    _tm.tm_mon = month-1;
    _tm.tm_mday = day;
    byte hour, min, sec;
    sscanf(time, "%hhu:%hhu:%hhu", &hour, &min, &sec);
    _tm.tm_hour = hour;
    _tm.tm_min = min;
    _tm.tm_sec = sec;
    set_timstamps();
}

/**
 * @brief set_timstamps fom struct tm entries
 * 
 */
void DateTime::set_timstamps() {
#if defined (__AVR__)
    _y2k_timestamp = mktime(&_tm);
    _unix_timestamp = _y2k_timestamp + UNIX_OFFSET;
#else
    _unix_timestamp = mktime(&_tm);
    _y2k_timestamp = _unix_timestamp - UNIX_OFFSET;
#endif
}

/**
 * @brief smart time print function based on the standard strftime function
 * see: https://en.cppreference.com/w/cpp/chrono/c/strftime
 * 
 * @param buffer buffer for time string 
 * @param buffersize size of buffer
 * @param formatSpec define format see strftime
 * @return size_t lenth of used buffer
 */
size_t DateTime::show_DateTime(char *buffer, size_t buffersize, const char *formatSpec) {
   size_t len {strftime(buffer, buffersize, formatSpec, &_tm)};
   return len;
}

// Slightly modified from JeeLabs / Ladyada
// Get all date/time at once to avoid rollover (e.g., minute/second don't match)
// Commented to avoid compiler warnings, but keeping in case we want this
// eventually
// static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
static uint8_t bcd2bin (uint8_t val) {
	return val - 6 * (val >> 4);
}

DateTime RTClib::now(TwoWire & _Wire) {
    _Wire.beginTransmission(CLOCK_ADDRESS);
    // This is the first register address (Seconds)
    _Wire.write(0);
    // We'll read from here on for 7 bytes from registers:
    // seconds, minutes, hours, day(1...7), date(1...31), month, year.
    _Wire.endTransmission();

    _Wire.requestFrom(CLOCK_ADDRESS, 7);
    int8_t sec = bcd2bin(_Wire.read() & 0x7F);
    int8_t min = bcd2bin(_Wire.read());
    int8_t hour = bcd2bin(_Wire.read());
    int8_t wday = bcd2bin(_Wire.read())-1;
    int8_t day = bcd2bin(_Wire.read());
    int8_t month = bcd2bin(_Wire.read());
    int16_t year = bcd2bin(_Wire.read()) + 2000;
    int16_t yday = 0;
    int16_t dst = 0;

    // add yday and DST calculation if needed
    // use the complete set also yday and dst to keep in mind the parameters
    return DateTime{year, month, day, hour, min, sec, wday, yday, dst};
}

byte DS3231::getSecond() {
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x00);
    _Wire.endTransmission();
    return getRegisterValue();
}

byte DS3231::getMinute() {
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x01);
    _Wire.endTransmission();
    return getRegisterValue();
}

byte DS3231::getHour(bool& h12, bool& PM_time) {
    byte temp_buffer;
    byte hour;
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x02);
    _Wire.endTransmission();

    _Wire.requestFrom(CLOCK_ADDRESS, 1);
    temp_buffer = _Wire.read();
    h12 = temp_buffer & 0b01000000;
    if (h12) {
        PM_time = temp_buffer & 0b00100000;
        hour = bcdToDec(temp_buffer & 0b00011111);
    }
    else {
        hour = bcdToDec(temp_buffer & 0b00111111);
    }
    return hour;
}

byte DS3231::getDoW() {
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x03);
    _Wire.endTransmission();
    return getRegisterValue();
}

byte DS3231::getDate() {
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x04);
    _Wire.endTransmission();
    return getRegisterValue();
}

byte DS3231::getMonth(bool& Century) {
    byte temp_buffer;
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x05);
    _Wire.endTransmission();

    _Wire.requestFrom(CLOCK_ADDRESS, 1);
    temp_buffer = _Wire.read();
    Century = temp_buffer & 0b10000000;
    return (bcdToDec(temp_buffer & 0b01111111));
}

byte DS3231::getYear() {
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x06);
    _Wire.endTransmission();
    return getRegisterValue();
}

// setEpoch function gives the epoch as parameter and feeds the RTC
// epoch = UnixTime and starts at 01.01.1970 00:00:00
// HINT: => the AVR time.h Lib is based on the year 2000
void DS3231::setEpoch(time_t epoch, bool flag_localtime) {
#if defined (__AVR__)
    epoch -= UNIX_OFFSET;
#endif
    struct tm tmnow;
    if (flag_localtime) {
        localtime_r(&epoch, &tmnow);
    }
    else {
        gmtime_r(&epoch, &tmnow);
    }
    setSecond(tmnow.tm_sec);
    setMinute(tmnow.tm_min);
    setHour(tmnow.tm_hour);
    setDoW(tmnow.tm_wday + 1U);
    setDate(tmnow.tm_mday);
    setMonth(tmnow.tm_mon + 1U);
    setYear(tmnow.tm_year - 100U);
}

void DS3231::setSecond(byte Second) {
    // Sets the seconds
    // This function also resets the Oscillator Stop Flag, which is set
    // whenever power is interrupted.
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x00);
    _Wire.write(decToBcd(Second));
    _Wire.endTransmission();
    // Clear OSF flag
    byte temp_buffer = readControlByte(1);
    writeControlByte((temp_buffer & 0b01111111), 1);
}

void DS3231::setMinute(byte Minute) {
    // Sets the minutes
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x01);
    _Wire.write(decToBcd(Minute));
    _Wire.endTransmission();
}

// Following setHour revision by David Merrifield 4/14/2020 correcting handling of 12-hour clock

void DS3231::setHour(byte Hour) {
    // Sets the hour, without changing 12/24h mode.
    // The hour must be in 24h format.

    bool h12;
    byte temp_hour;

    // Start by figuring out what the 12/24 mode is
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x02);
    _Wire.endTransmission();
    _Wire.requestFrom(CLOCK_ADDRESS, 1);
    h12 = (_Wire.read() & 0b01000000);
    // if h12 is true, it's 12h mode; false is 24h.

    if (h12) {
        // 12 hour
        bool am_pm = (Hour > 11);
        temp_hour = Hour;
        if (temp_hour > 11) {
            temp_hour = temp_hour - 12;
        }
        if (temp_hour == 0) {
            temp_hour = 12;
        }
        temp_hour = decToBcd(temp_hour) | (am_pm << 5) | 0b01000000;
    } else {
        // 24 hour
        temp_hour = decToBcd(Hour) & 0b10111111;
    }

    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x02);
    _Wire.write(temp_hour);
    _Wire.endTransmission();
}

void DS3231::setDoW(byte DoW) {
    // Sets the Day of Week
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x03);
    _Wire.write(decToBcd(DoW));
    _Wire.endTransmission();
}

void DS3231::setDate(byte Date) {
    // Sets the Date
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x04);
    _Wire.write(decToBcd(Date));
    _Wire.endTransmission();
}

void DS3231::setMonth(byte Month) {
    // Sets the month
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x05);
    _Wire.write(decToBcd(Month));
    _Wire.endTransmission();
}

void DS3231::setYear(byte Year) {
    // Sets the year
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x06);
    _Wire.write(decToBcd(Year));
    _Wire.endTransmission();
}

void DS3231::setClockMode(bool h12) {
    // sets the mode to 12-hour (true) or 24-hour (false).
    // One thing that bothers me about how I've written this is that
    // if the read and right happen at the right hourly millisecnd,
    // the clock will be set back an hour. Not sure how to do it better,
    // though, and as long as one doesn't set the mode frequently it's
    // a very minimal risk.
    // It's zero risk if you call this BEFORE setting the hour, since
    // the setHour() function doesn't change this mode.

    byte temp_buffer;

    // Start by reading byte 0x02.
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x02);
    _Wire.endTransmission();
    _Wire.requestFrom(CLOCK_ADDRESS, 1);
    temp_buffer = _Wire.read();

    // Set the flag to the requested value:
    if (h12) {
        temp_buffer = temp_buffer | 0b01000000;
    } else {
        temp_buffer = temp_buffer & 0b10111111;
    }

    // Write the byte
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x02);
    _Wire.write(temp_buffer);
    _Wire.endTransmission();
}

float DS3231::getTemperature() {
    // Checks the internal thermometer on the DS3231 and returns the
    // temperature as a floating-point value.

    // Updated / modified a tiny bit from "Coding Badly" and "Tri-Again"
    // http://forum.arduino.cc/index.php/topic,22301.0.html

    byte tMSB, tLSB;
    float temp3231;

    // temp registers (11h-12h) get updated automatically every 64s
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x11);
    _Wire.endTransmission();
    _Wire.requestFrom(CLOCK_ADDRESS, 2);

    // Should I do more "if available" checks here?
    if(_Wire.available()) {
    tMSB = _Wire.read(); //2's complement int portion
    tLSB = _Wire.read(); //fraction portion

    int16_t  itemp  = ( tMSB << 8 | (tLSB & 0xC0) );  // Shift upper byte, add lower
    temp3231 = ( (float)itemp / 256.0 );              // Scale and return
    }
    else {
    temp3231 = -9999; // Impossible temperature; error value
    }

    return temp3231;
}

void DS3231::getA1Time(byte& A1Day, byte& A1Hour, byte& A1Minute, byte& A1Second, byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM) {
    byte temp_buffer;
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x07);
    _Wire.endTransmission();

    _Wire.requestFrom(CLOCK_ADDRESS, 4);

    temp_buffer	= _Wire.read();	// Get A1M1 and A1 Seconds
    A1Second	= bcdToDec(temp_buffer & 0b01111111);
    // put A1M1 bit in position 0 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>7;

    temp_buffer		= _Wire.read();	// Get A1M2 and A1 minutes
    A1Minute	= bcdToDec(temp_buffer & 0b01111111);
    // put A1M2 bit in position 1 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>6;

    temp_buffer	= _Wire.read();	// Get A1M3 and A1 Hour
    // put A1M3 bit in position 2 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>5;
    // determine A1 12/24 mode
    A1h12       = temp_buffer & 0b01000000;
    if (A1h12) {
        A1PM    = temp_buffer & 0b00100000;			// determine am/pm
        A1Hour  = bcdToDec(temp_buffer & 0b00011111);	// 12-hour
    } else {
        A1Hour  = bcdToDec(temp_buffer & 0b00111111);	// 24-hour
    }

    temp_buffer	= _Wire.read();	// Get A1M4 and A1 Day/Date
    // put A1M3 bit in position 3 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>4;
    // determine A1 day or date flag
    A1Dy		= (temp_buffer & 0b01000000)>>6;
    if (A1Dy) {
        // alarm is by day of week, not date.
        A1Day	= bcdToDec(temp_buffer & 0b00001111);
    } else {
        // alarm is by date, not day of week.
        A1Day	= bcdToDec(temp_buffer & 0b00111111);
    }
}

void DS3231::getA1Time(byte& A1Day, byte& A1Hour, byte& A1Minute, byte& A1Second, byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM, bool clearAlarmBits) {
    if (clearAlarmBits) {
        AlarmBits = 0x0;
    }
    getA1Time(A1Day, A1Hour, A1Minute, A1Second, AlarmBits, A1Dy, A1h12, A1PM);
}

void DS3231::getA2Time(byte& A2Day, byte& A2Hour, byte& A2Minute, byte& AlarmBits, bool& A2Dy, bool& A2h12, bool& A2PM) {
    byte temp_buffer;
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x0b);
    _Wire.endTransmission();

    _Wire.requestFrom(CLOCK_ADDRESS, 3);
    temp_buffer	= _Wire.read();	// Get A2M2 and A2 Minutes
    A2Minute	= bcdToDec(temp_buffer & 0b01111111);
    // put A2M2 bit in position 4 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>3;

    temp_buffer	= _Wire.read();	// Get A2M3 and A2 Hour
    // put A2M3 bit in position 5 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>2;
    // determine A2 12/24 mode
    A2h12		= temp_buffer & 0b01000000;
    if (A2h12) {
        A2PM	= temp_buffer & 0b00100000;			// determine am/pm
        A2Hour	= bcdToDec(temp_buffer & 0b00011111);	// 12-hour
    } else {
        A2Hour	= bcdToDec(temp_buffer & 0b00111111);	// 24-hour
    }

    temp_buffer	= _Wire.read();	// Get A2M4 and A1 Day/Date
    // put A2M4 bit in position 6 of DS3231_AlarmBits.
    AlarmBits	= AlarmBits | (temp_buffer & 0b10000000)>>1;
    // determine A2 day or date flag
    A2Dy		= (temp_buffer & 0b01000000)>>6;
    if (A2Dy) {
        // alarm is by day of week, not date.
        A2Day	= bcdToDec(temp_buffer & 0b00001111);
    } else {
        // alarm is by date, not day of week.
        A2Day	= bcdToDec(temp_buffer & 0b00111111);
    }
}

void DS3231::getA2Time(byte& A2Day, byte& A2Hour, byte& A2Minute, byte& AlarmBits, bool& A2Dy, bool& A2h12, bool& A2PM, bool clearAlarmBits) {
    if (clearAlarmBits) {
        AlarmBits = 0x0;
    }
    getA2Time(A2Day, A2Hour, A2Minute, AlarmBits, A2Dy, A2h12, A2PM);
}

void DS3231::setA1Time(byte A1Day, byte A1Hour, byte A1Minute, byte A1Second, byte AlarmBits, bool A1Dy, bool A1h12, bool A1PM) {
    //	Sets the alarm-1 date and time on the DS3231, using A1* information
    byte temp_buffer;
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x07);	// A1 starts at 07h
    // Send A1 second and A1M1
    _Wire.write(decToBcd(A1Second) | ((AlarmBits & 0b00000001) << 7));
    // Send A1 Minute and A1M2
    _Wire.write(decToBcd(A1Minute) | ((AlarmBits & 0b00000010) << 6));
    // Figure out A1 hour
    if (A1h12) {
        // Start by converting existing time to h12 if it was given in 24h.
        if (A1Hour > 12) {
            // well, then, this obviously isn't a h12 time, is it?
            A1Hour = A1Hour - 12;
            A1PM = true;
        }
        if (A1PM) {
            // Afternoon
            // Convert the hour to BCD and add appropriate flags.
            temp_buffer = decToBcd(A1Hour) | 0b01100000;
        } else {
            // Morning
            // Convert the hour to BCD and add appropriate flags.
            temp_buffer = decToBcd(A1Hour) | 0b01000000;
        }
    } else {
        // Now for 24h
        temp_buffer = decToBcd(A1Hour);
    }
    temp_buffer = temp_buffer | ((AlarmBits & 0b00000100)<<5);
    // A1 hour is figured out, send it
    _Wire.write(temp_buffer);
    // Figure out A1 day/date and A1M4
    temp_buffer = ((AlarmBits & 0b00001000)<<4) | decToBcd(A1Day);
    if (A1Dy) {
        // Set A1 Day/Date flag (Otherwise it's zero)
        temp_buffer = temp_buffer | 0b01000000;
    }
    _Wire.write(temp_buffer);
    // All done!
    _Wire.endTransmission();
}

void DS3231::setA2Time(byte A2Day, byte A2Hour, byte A2Minute, byte AlarmBits, bool A2Dy, bool A2h12, bool A2PM) {
    //	Sets the alarm-2 date and time on the DS3231, using A2* information
    byte temp_buffer;
    _Wire.beginTransmission(CLOCK_ADDRESS);
    _Wire.write(0x0b);	// A1 starts at 0bh
    // Send A2 Minute and A2M2
    _Wire.write(decToBcd(A2Minute) | ((AlarmBits & 0b00010000) << 3));
    // Figure out A2 hour
    if (A2h12) {
        // Start by converting existing time to h12 if it was given in 24h.
        if (A2Hour > 12) {
            // well, then, this obviously isn't a h12 time, is it?
            A2Hour = A2Hour - 12;
            A2PM = true;
        }
        if (A2PM) {
            // Afternoon
            // Convert the hour to BCD and add appropriate flags.
            temp_buffer = decToBcd(A2Hour) | 0b01100000;
        } else {
            // Morning
            // Convert the hour to BCD and add appropriate flags.
            temp_buffer = decToBcd(A2Hour) | 0b01000000;
        }
    } else {
        // Now for 24h
        temp_buffer = decToBcd(A2Hour);
    }
    // add in A2M3 bit
    temp_buffer = temp_buffer | ((AlarmBits & 0b00100000)<<2);
    // A2 hour is figured out, send it
    _Wire.write(temp_buffer);
    // Figure out A2 day/date and A2M4
    temp_buffer = ((AlarmBits & 0b01000000)<<1) | decToBcd(A2Day);
    if (A2Dy) {
        // Set A2 Day/Date flag (Otherwise it's zero)
        temp_buffer = temp_buffer | 0b01000000;
    }
    _Wire.write(temp_buffer);
    // All done!
    _Wire.endTransmission();
}

void DS3231::turnOnAlarm(byte Alarm) {
    // turns on alarm number "Alarm". Defaults to 2 if Alarm is not 1.
    byte temp_buffer = readControlByte(0);
    // modify control byte
    if (Alarm == 1) {
        temp_buffer = temp_buffer | 0b00000101;
    } else {
        temp_buffer = temp_buffer | 0b00000110;
    }
    writeControlByte(temp_buffer, 0);
}

void DS3231::turnOffAlarm(byte Alarm) {
    // turns off alarm number "Alarm". Defaults to 2 if Alarm is not 1.
    // Leaves interrupt pin alone.
    byte temp_buffer = readControlByte(0);
    // modify control byte
    if (Alarm == 1) {
        temp_buffer = temp_buffer & 0b11111110;
    } else {
        temp_buffer = temp_buffer & 0b11111101;
    }
    writeControlByte(temp_buffer, 0);
}

bool DS3231::checkAlarmEnabled(byte Alarm) {
    // Checks whether the given alarm is enabled.
    byte result = 0x0;
    byte temp_buffer = readControlByte(0);
    if (Alarm == 1) {
        result = temp_buffer & 0b00000001;
    } else {
        result = temp_buffer & 0b00000010;
    }
    return result;
}

bool DS3231::checkIfAlarm(byte Alarm) {
    // Checks whether alarm 1 or alarm 2 flag is on, returns T/F accordingly.
    // Turns flag off, also.
    // defaults to checking alarm 2, unless Alarm == 1.
    byte result;
    byte temp_buffer = readControlByte(1);
    if (Alarm == 1) {
        // Did alarm 1 go off?
        result = temp_buffer & 0b00000001;
        // clear flag
        temp_buffer = temp_buffer & 0b11111110;
    } else {
        // Did alarm 2 go off?
        result = temp_buffer & 0b00000010;
        // clear flag
        temp_buffer = temp_buffer & 0b11111101;
    }
    writeControlByte(temp_buffer, 1);
    return result;
}

bool DS3231::checkIfAlarm(byte Alarm, bool clearflag) {
    // Checks whether alarm 1 or alarm 2 flag is on, returns T/F accordingly.
    // Clears flag, if clearflag is set
    // defaults to checking alarm 2, unless Alarm == 1.
    byte result;
    byte temp_buffer = readControlByte(1);
    if (Alarm == 1) {
        // Did alarm 1 go off?
        result = temp_buffer & 0b00000001;
        // clear flag
        temp_buffer = temp_buffer & 0b11111110;
    } else {
        // Did alarm 2 go off?
        result = temp_buffer & 0b00000010;
        // clear flag
        temp_buffer = temp_buffer & 0b11111101;
    }
    if (clearflag) {
        writeControlByte(temp_buffer, 1);
    }
    return result;
}

void DS3231::enableOscillator(bool TF, bool battery, byte frequency) {
    // turns oscillator on or off. True is on, false is off.
    // if battery is true, turns on even for battery-only operation,
    // otherwise turns off if Vcc is off.
    // frequency must be 0, 1, 2, or 3.
    // 0 = 1 Hz
    // 1 = 1.024 kHz
    // 2 = 4.096 kHz
    // 3 = 8.192 kHz (Default if frequency byte is out of range)
    if (frequency > 3) frequency = 3;
    // read control byte in, but zero out current state of RS2 and RS1.
    byte temp_buffer = readControlByte(0) & 0b11100111;
    if (battery) {
        // turn on BBSQW flag
        temp_buffer = temp_buffer | 0b01000000;
    } else {
        // turn off BBSQW flag
        temp_buffer = temp_buffer & 0b10111111;
    }
    if (TF) {
        // set ~EOSC to 0 and INTCN to zero.
        temp_buffer = temp_buffer & 0b01111011;
    } else {
        // set ~EOSC to 1, leave INTCN as is.
        temp_buffer = temp_buffer | 0b10000000;
    }
    // shift frequency into bits 3 and 4 and set.
    frequency = frequency << 3;
    temp_buffer = temp_buffer | frequency;
    // And write the control bits
    writeControlByte(temp_buffer, 0);
}

void DS3231::enable32kHz(bool TF) {
    // turn 32kHz pin on or off
    byte temp_buffer = readControlByte(1);
    if (TF) {
        // turn on 32kHz pin
        temp_buffer = temp_buffer | 0b00001000;
    } else {
        // turn off 32kHz pin
        temp_buffer = temp_buffer & 0b11110111;
    }
    writeControlByte(temp_buffer, 1);
}

bool DS3231::oscillatorCheck() {
    // Returns false if the oscillator has been off for some reason.
    // If this is the case, the time is probably not correct.
    byte temp_buffer = readControlByte(1);
    bool result = true;
    if (temp_buffer & 0b10000000) {
        // Oscillator Stop Flag (OSF) is set, so return false.
        result = false;
    }
    return result;
}

/*****************************************
	Private Functions
 *****************************************/

byte DS3231::decToBcd(byte val) {
// Convert normal decimal numbers to binary coded decimal
    return ( (val/10*16) + (val%10) );
}

byte DS3231::bcdToDec(byte val) {
// Convert binary coded decimal to normal decimal numbers
    return ( (val/16*10) + (val%16) );
}

byte DS3231::readControlByte(bool which) {
    // Read selected control byte
    // first byte (0) is 0x0e, second (1) is 0x0f
    _Wire.beginTransmission(CLOCK_ADDRESS);
    if (which) {
        // second control byte
        _Wire.write(0x0f);
    } else {
        // first control byte
        _Wire.write(0x0e);
    }
    _Wire.endTransmission();
    _Wire.requestFrom(CLOCK_ADDRESS, 1);
    return _Wire.read();
}

void DS3231::writeControlByte(byte control, bool which) {
    // Write the selected control byte.
    // which=false -> 0x0e, true->0x0f.
    _Wire.beginTransmission(CLOCK_ADDRESS);
    if (which) {
        _Wire.write(0x0f);
    } else {
        _Wire.write(0x0e);
    }
    _Wire.write(control);
    _Wire.endTransmission();
}
