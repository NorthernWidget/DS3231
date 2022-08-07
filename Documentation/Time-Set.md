# DS3231 Library
## Time Setting Functions
The following methods of the DS3231 object use I2C hardware connection and the Wire library to transfer data into certain hardware registers of the DS3231. The Library assumes that the DS3231 has an I2C address of 0x68.

The DS3231 register addresses mentioned below are documented on page 11 of the [manufacturer's datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231-DS3231S.pdf).

The examples provided below assume a variable has been declared as follows:
```DS3231 myRTC;```

<ul>
  <li><a href="#setSecond">setSecond&#40;&#41;</a></li>
  <li><a href="#setMinute">setMinute&#40;&#41;</a></li>
  <li><a href="#setHour">setHour&#40;&#41;</a></li>
  <li><a href="#setDow">setDoW&#40;&#41;</a></li>
  <li><a href="#setDate">setDate&#40;&#41;</a></li>
  <li><a href="#setMonth">setMonth&#40;&#41;</a></li>
  <li><a href="#setYear">setYear&#40;&#41;</a></li>
</ul>

<h3 id="setSecond">setSecond&#40;&#41;</h3>

