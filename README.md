bearded-bear
============

A stub OpenFrameworks + Leap Motion application on Ubuntu

Notes
--

### Hand Position

Hand position is quite crucial in getting accurate data. A hand that isn't entirely over the sensor will sometimes produce random extreme values for reading the palm position. 

Setup
--

- Register as a [Leap Motion dev](https://developer.leapmotion.com/) and download the latest SDK
- Open the header file LeapMath.h and replace the preprocessor directives PI, DEG_TO_RAD, RAD_TO_DEG  with these:

```
...
/** The constant pi as a single precision floating point number. */
#ifndef PI
static const float PI          = 3.1415926536f;
#endif
/**
 * The constant ratio to convert an angle measure from degrees to radians.
 * Multiply a value in degrees by this constant to convert to radians.
 */
#ifndef DEG_TO_RAD
static const float DEG_TO_RAD  = 0.0174532925f;
#endif
/**
 * The constant ratio to convert an angle measure from radians to degrees.
 * Multiply a value in radians by this constant to convert to degrees.
 */
#ifndef RAD_TO_DEG
static const float RAD_TO_DEG  = 57.295779513f;
#endif
...
```

- Generate a new OF project using the project generator
- Copy libLeap.so shared libary to your PROJECT_ROOT/libs folder (if the folder doesn't exist create it)
- Add the following to your config.make

		USER_LIBS = -lLeap
		USER_LDFLAGS = -L./libs

- Update PROJECT_CFLAGS to point to the Leap SDK include folder
- Make/Compile and run application