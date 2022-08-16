#include <Wire.h>
#include <DS3231.h>

void showTimeFormated(time_t t) {
#if defined (__AVR__)
    t -= 946684800UL;
#endif
  char buffer[50];
  struct tm *ptm;
  ptm = gmtime (&t);
  const char * timeformat {"%a %F %X - weekday %w; CW %W"};
  strftime(buffer, sizeof(buffer), timeformat, ptm);
  Serial.print(buffer);
  Serial.print("\n");
}

constexpr time_t tstmp {1660644000UL};

RTClib myRTC;
DS3231 Clock;

void setup () {
    Serial.begin(115200);
    Wire.begin();
    delay(500);
    Serial.println("\n\n\nTest of DS3231 - setEpoch()\n\n\n");

#if defined (__AVR__)
#warning using AVR platform
    Serial.println("AVR Microcontroller Ready!");
    Wire.begin();
#elif defined (ESP8266)
Serial.println("ESP8266 Microcontroller Ready!");
#warning using espressif platform
    // SDA = 0, SCL = 2
    Wire.begin(0U, 2U);
#endif

    // set the Ds3131 with a specific UnixTimestamp
    // ==>  	Tue Aug 16 2022 10:00:00 GMT+0000 - weekday 2 (0 = Sunday); CW 33
    // ==>    1660644000
    
    Serial.println("Tue Aug 16 2022 10:00:00 GMT+0000 - weekday 2 (0 = Sunday); CW 33");
    Serial.println("UnixTimestamp - 1660644000");


    // feed UnixTimeStamp and don' t use localtime
    Clock.setEpoch(tstmp, false);
    // set to 24h
    Clock.setClockMode(false);

    // Just for verification of DS3231 Data
    // check now the data from ESP8266 and DS3231
    // get year
    bool century = false;
    bool h12Flag;
    bool pmFlag;
    DateTime now = myRTC.now();
    Serial.print("\n\n");
    Serial.print(" DateTime of DS3231:     ");
    Serial.print(Clock.getYear(), DEC);
    Serial.print("-");
    Serial.print(Clock.getMonth(century), DEC);
    Serial.print("-");
    Serial.print(Clock.getDate(), DEC);
    Serial.print(" ");
    Serial.print(Clock.getHour(h12Flag, pmFlag), DEC);
    Serial.print(":");
    Serial.print(Clock.getMinute(), DEC);
    Serial.print(":");
    Serial.print(Clock.getSecond(), DEC);
    Serial.print("  -  weekday ");
    Serial.print(Clock.getDoW(), DEC);
    Serial.println();

    Serial.print("\n\n");
    Serial.print(" DateTime of RTC:        ");
    Serial.print(now.year(), DEC);
    Serial.print("-");
    Serial.print(now.month(), DEC);
    Serial.print("-");
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print("\n\n Output of Struct tm:  ");
    showTimeFormated(tstmp);
}

void loop () {
}