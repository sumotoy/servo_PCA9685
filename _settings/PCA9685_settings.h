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
#define PCA9685_DEF_MIN_ST_SERVO_VAL  150//122
#define PCA9685_DEF_MAX_ST_SERVO_VAL  600//615
#define PCA9685_DEF_MIN_CNT_SERVO_VAL  340
#define PCA9685_DEF_MAX_CNT_SERVO_VAL  410
/*
---- TowerPro MicroServo SG90 -----
	Stall torque: 1.8 kgf.cm 
	Operating speed: 0.1 s/60 degree
	Operating voltage: 4.8 V (~5V) 
	Range: 180°
	MIN:
	MAX:
*/
#endif