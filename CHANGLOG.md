# Changelog

When adding new features to the repository (i.e. by merging your PR), please add your changes to the section with the upcoming version. With every build, this section becomes the next version, and a new upcoming section is created.

## -- Upcoming --

- [PR00](https://github.com/NorthernWidget/DS3231/pull/00) Description of change
    * this is just a placeholder: to be replaced with first unreleased change


## v1.1.2

- [PR77](https://github.com/NorthernWidget/DS3231/pull/77) CI by compiling the examples
- [PR75](https://github.com/NorthernWidget/DS3231/pull/75) Rename "pde" extensions to "ino" according to Arduino IDE convention
- [PR69](https://github.com/NorthernWidget/DS3231/pull/69) Resolve non-fatal compiler warnings in DateTime::DateTime(uint32_t)
- [PR66](https://github.com/NorthernWidget/DS3231/pull/66) add new example and BUGFIX for `__AVR__` for function `DS3231::setEpoch()`
    * fixes a bug in setEpoch() and adds an example
- [PR62](https://github.com/NorthernWidget/DS3231/pull/62) AlarmBits parameter not initialized in getA1Time and getA2Time [#51](https://github.com/NorthernWidget/DS3231/issues/51)
    * Added `tests` directory for rudimentary unit tests
    * Overloaded `getAXTime()` methods with `bool clearAlarmBits` flag.
    * Added Arduino test script for `getAXTime()` methods
- [PR61](https://github.com/NorthernWidget/DS3231/pull/61) Checkifalarm issue [#9](https://github.com/NorthernWidget/DS3231/issues/9)
- [PR58](https://github.com/NorthernWidget/DS3231/pull/58) Optional clear flag in checkIfAlarm() [#9](https://github.com/NorthernWidget/DS3231/issues/9)
- Documentation and examples updates:
    * [PR80](https://github.com/NorthernWidget/DS3231/pull/80) corrects description of getYear() to resolve issue [#78](https://github.com/NorthernWidget/DS3231/issues/78)
    * [PR73](https://github.com/NorthernWidget/DS3231/pull/73) adding links to the documentation for each public function
    * [PR72](https://github.com/NorthernWidget/DS3231/pull/72) document utility functions
    * [PR70](https://github.com/NorthernWidget/DS3231/pull/70) Alarm documentation and examples
    * [PR67](https://github.com/NorthernWidget/DS3231/pull/67) Time-Set functions, defaults in setEpoch()
    * [PR63](https://github.com/NorthernWidget/DS3231/pull/63) Remove Perform-Calculations section
    * [85688185](https://github.com/NorthernWidget/DS3231/commit/8568818513d8d9912e5f9d6a3810b0b4793eb2e2) Introduced Changelog
    * [PR59](https://github.com/NorthernWidget/DS3231/pull/59) Finished first draft revision of README
    * [PR57](https://github.com/NorthernWidget/DS3231/pull/57) Example/interrupt

## v1.1.1
- Overloaded `checkIfAlarm()` function with optinal parameter to suppress clearing of the alarm flags
- Introduced new alarm examples working with polling and interrupting code
- Addressed an issue with naming in the declaration: Standard naming now uses `myRTC` instead of `Clock`.


## v1.1.0
This is the last code version, whos changes are not covered in the changelog.
