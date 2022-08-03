
struct bits3231 {
  // 00h time seconds 00 - 59
  unsigned timeSec:4; // seconds digit 0 - 9
  unsigned timeSec10:3; // tens of seconds digit 0 - 5 
  unsigned :1; // skip high bit 
  // 01h time minutes 00 - 59
  unsigned timeMin:4; // minutes digit 0 - 9
  unsigned timeMin10:3; // tens of minuts digit 0 - 5
  unsigned: 1; // skip high bit
  // 02h time hour 1 - 12 or 0 - 23
  unsigned timeHour:4; // hours digit 0 - 9
  unsigned timeHour10:1; // hour ten digit 0 - 1
  unsigned timeAP20:1;  // 0=am, 1=pm if timeMode12 = 1, else 0 = 0, 1 = 20-hour
  unsigned timeMode12:1; // 1=12-hour mode, 0=24-hour mode
  unisgned :1; // skip high bit
  // 03h time day of week
  unsigned timeDay:3; // 1 - 7, user-defined
  unisgned :5; // skip bits 7:3
  // 04h time date of month 01 - 31
  unsigned timeDate:4; // date 0 - 9
  unsigned timeDate10:2; // tens 0 - 3
  unsigned :2; // skip bits 7:6
  // 05h time month 01 - 12
  unsigned timeMonth:4; // month digit 0 - 9
  unsigned timeMonth10:1; // month tens digit 0 - 1
  unsigned :2; // skip bits 6:5
  unsigned timeCentury:1; // century bit
  // 06h time year 00-99
  unsigned timeYear:4; // year digit 0 - 9
  unsigned timeYear10:4; // year tens digit 0 - 9
  // 07h Alarm 1 seconds 00 - 59
  unsigned A1Sec:4; // seconds digit 0 - 9
  unsigned A1Sec10:3; // tens of seconds digit 0 - 5 
  unsigned A1M1:1; // Alarm 1 Bit 1
  // 08h Alarm 1 minutes 00 - 59
  unsigned A1Min:4; // minutes digit 0 - 9
  unsigned A1Min10:3; // tens of minuts digit 0 - 5
  unsigned: A1M2; // Alarm 1 Bit 2
  // 09h Alarm 1 hour 1 - 12 or 0 - 23
  unsigned A1Hour:4; // hours digit 0 - 9
  unsigned A1Hour10:1; // hour ten digit 0 - 1
  unsigned A1AP20:1;  // 0=am, 1=pm if time12 = 1, else 0 = 0, 1 = 20-hour
  unsigned A1Mode12:1; // 1=12-hour mode, 0=24-hour mode
  unsigned A1M3:1; // Alarm 1 Bit 3
  // 0Ah Alarm 1 date
  unsigned A1DayDate:4; // month digit 0 - 9 if A1DYDT = 1, else day 1 - 7 
  unsigned A1Date10:2; // month tens 0-3 if A1DyDT = 1
  unsigned A1DYDT:1; // 1 = match month, 0 = match day of week
  unsigned A1M4:1; // Alarm 1 Bit 4
  //
  // note: there is no alarm bit named A2M1
  //
  // 0Bh Alarm 2 minutes 00 - 59
  unsigned A2Min:4; // minutes digit 0 - 9
  unsigned A2Min10:3; // tens of minuts digit 0 - 5
  unsigned: A2M2; // Alarm 2 Bit 2
  // 0Ch Alarm 2 hour 1 - 12 or 0 - 23
  unsigned A2Hour:4; // hours digit 0 - 9
  unsigned A2Hour10:1; // hour ten digit 0 - 1
  unsigned A2AP20:1;  // 0=am, 1=pm if time12 = 1, else 0 = 0, 1 = 20-hour
  unsigned A2Mode12:1; // 1=12-hour mode, 0=24-hour mode
  unsigned A2M3:1; // Alarm 2 Bit 3
  // 0Dh Alarm 2 date
  unsigned A2DayDate:4; // month digit 0 - 9 if A1DYDT = 1, else day 1 - 7 
  unsigned A2Date10:2; // month tens 0-3 if A1DyDT = 1
  unsigned A2DYDT:1; // 1 = match month, 0 = match day of week
  unsigned A2M4:1; // Alarm 2 Bit 4
  // 0Eh Device control register; see data sheet
  unsigned EOSC:1; // enable oscillator, active low; all data static when high
  unsigned BBSQW:1; // battery-backed square wave enable
  unsigned CONV:1; // convert temperature, active high
  unsigned RS2:1; // set square wave frequency
  unsigned RS1:1; // set square wave frequency
  unsigned INTCN:1; // interrupt control, high enables interrupt output
  unsigned A2IE:1; // enbable interrupt on A2 match
  unsigned A1IE:1; // enable interrupt on A1 match
  // 0Fh Device control / status register
  unsigned OSF:1; // oscillator stop flag
  unsigned :3; // skip bits 6:4
  unsigned EN23KHZ:1; // enable 32.768kHz square wave output, active high
  unsigned BSY:1; // device busy flag
  unsigned A2F:1; // alarm 2 flag, high = time match with Alarm 2
  unsigned A1F:1; // alarm 1 flag, high = time match with Alarm 1 
  // 10h Aging offset
  signed AgingOffset:8; // aging offset in two's-complement form
  // 11h MSB of temperature
  signed tempMSB:8; // MSB of temperature in two's complement form
  // 12h LSB of temperature
  unsigned :6; // skip bits 5:0
  unsigned tempLSB:2; // LSB in bits 7:6
}
