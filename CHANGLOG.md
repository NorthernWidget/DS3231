# Changelog

When adding new features to the repository (i.e. by merging your PR), please add your changes to the section with the upcoming version. With every build, this section becomes the next version, and a new upcoming section is created.

## -- Upcoming --
- Overloaded `getAXTime()` methods with `bool clearAlarmBits` flag.
- Added `tests` directory for rudimentary unit tests
- Added Arduino test script for `getAXTime()` methods
- Introduced Changelog
- PR #66 fixes a bug in setEpoch() and adds an example


## v1.1.1
- Overloaded `checkIfAlarm()` function with optinal parameter to suppress clearing of the alarm flags
- Introduced new alarm examples working with polling and interrupting code
- Addressed an issue with naming in the declaration: Standard naming now uses `myRTC` instead of `Clock`.


## v1.1.0
This is the last code version, whos changes are not covered in the changelog.
