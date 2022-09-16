#include <DS3231.h>

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while(!Serial){};
  
  Serial.print("Testing Subtraction: 1 Jan 2000 00:00:01 - 1 Jan 2000 00:00:00 = ");
  Serial.print(DateTime(946684801) - DateTime(946684800));Serial.println(" second");

  Serial.print("Testing Subtraction: 1 Jan 2000 00:01:01 - 1 Jan 2000 00:00:00 = ");
  Serial.print(DateTime(946684861) - DateTime(946684800));Serial.println(" seconds");

  Serial.print("Testing Subtraction: 1 Jan 2000 00:00:00 - 1 Jan 2000 00:00:01 = ");
  Serial.print(DateTime(946684800) - DateTime(946684801));Serial.println(" seconds");

  DateTime result = DateTime(946684801) + 60;
  Serial.print("Testing Addition: 1 Jan 2000 00:00:01 + 60 seconds = ");
  Serial.print(result.day());Serial.print(' ');
  Serial.print(result.month());Serial.print(' ');
  Serial.print(result.year());Serial.print(' ');
  Serial.print(result.hour());Serial.print(':');
  Serial.print(result.minute());Serial.print(':');
  Serial.println(result.second());

  Serial.println("TESTING >");

  Serial.print("1 Jan 2000 00:00:01 > 1 Jan 2000 00:00:00 = ");
  (DateTime(946684801) > DateTime(946684800)) ? Serial.println("TRUE") : Serial.println("FALSE");
  
  Serial.print("1 Jan 2000 00:01:00 > 1 Jan 2000 00:00:01 = ");
  (DateTime(946684860) > DateTime(946684801)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 01:00:00 > 1 Jan 2000 00:01:01 = ");
  (DateTime(946688400) > DateTime(946684861)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("2 Jan 2000 00:00:00 > 1 Jan 2000 00:01:01 = ");
  (DateTime(946789200) > DateTime(946684861)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Feb 2000 00:00:00 > 1 Jan 2000 00:01:01 = ");
  (DateTime(949381200) > DateTime(946684861)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2001 00:00:00 > 1 Jan 2000 00:01:01 = ");
  (DateTime(978325200) > DateTime(946684861)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 00:00:00 > 1 Jan 2000 00:00:00 = ");
  (DateTime(946684800) > DateTime(946684800)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 00:00:00 > 1 Jan 2000 00:00:01 = ");
  (DateTime(946684800) > DateTime(946684801)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.println("TESTING <");

  Serial.print("1 Jan 2000 00:00:00 < 1 Jan 2000 00:00:01 = ");
  (DateTime(946684800) < DateTime(946684801)) ? Serial.println("TRUE") : Serial.println("FALSE");
  
  Serial.print("1 Jan 2000 00:00:01 < 1 Jan 2000 00:01:00 = ");
  (DateTime(946684801) < DateTime(946684860)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 00:01:00 < 1 Jan 2000 01:00:00 = ");
  (DateTime(946684860) < DateTime(946702860)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 01:00:00 < 2 Jan 2000 00:00:00 = ");
  (DateTime(946702860) < DateTime(946789200)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("2 Jan 2000 00:00:00 < 1 Feb 2000 00:00:00 = ");
  (DateTime(946789200) < DateTime(949381200)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Feb 2000 00:00:00 < 1 Feb 2001 00:00:00 = ");
  (DateTime(949381200) < DateTime(981003600)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 00:00:00 < 1 Jan 2000 00:00:00 = ");
  (DateTime(946684800) < DateTime(946684800)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 00:00:01 < 1 Jan 2001 00:00:00 = ");
  (DateTime(946684801) < DateTime(946684800)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.println("TESTING ==");
  
  Serial.print("1 Jan 2000 00:00:00 == 1 Jan 2000 00:00:00 ? ");
  (DateTime(946684800) == DateTime(946684800)) ? Serial.println("TRUE") : Serial.println("FALSE");

  Serial.print("1 Jan 2000 00:00:00 == 1 Jan 2001 00:00:01 ? ");
  (DateTime(946684800) == DateTime(946684801)) ? Serial.println("TRUE") : Serial.println("FALSE");
}

void loop() {
  // put your main code here, to run repeatedly:

}
