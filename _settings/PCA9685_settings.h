#ifndef _PCA9685_SETTINGS_H_
#define _PCA9685_SETTINGS_H_



#if defined(__SAM3X8E__)
	#define WIRE Wire1
#else
	#define WIRE Wire
#endif

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define PCA9685_DEF_MIN_SERVO_VAL  150
#define PCA9685_DEF_MAX_SERVO_VAL  600

#endif