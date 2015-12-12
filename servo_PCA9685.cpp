
#include "servo_PCA9685.h"

#if defined(ESP8266)
	servo_PCA9685::servo_PCA9685(const uint8_t adrs,const uint8_t scl_pin,const uint8_t sda_pin)
	{
		_sclPin = scl_pin;
		_sdaPin = sda_pin;
		_adrs = adrs;
	}
#else
	servo_PCA9685::servo_PCA9685(const uint8_t adrs)
	{
		_adrs  = adrs;
	}
#endif

void servo_PCA9685::begin(bool skipWireInit)
{
	uint8_t i;
	if (!skipWireInit){
		#if defined(ESP8266)
			if (_sclPin != 5 || _sdaPin != 4) {
				WIRE.begin(_sdaPin,_sclPin);
			} else {
				WIRE.begin();
			}
			WIRE.setClock(400000UL);
		#else
			WIRE.begin();
		#endif
	}
	_sendWire(PCA9685_MODE1, 0x0);//reset chip
	delay(10);
	setPWMFreq(60);// Analog servos usually run at ~60 Hz updates 
	for (i=0;i<16;i++){
		setServoType(i,STANDARD);//set temporarely all servo as standard
	}
	delay(100);
}

void servo_PCA9685::setServoType(uint8_t servo,const enum PAC9685mechanical type)
{
	if (type > 1) return;
	if (servo > 15) servo = 15;
	_servoType[servo] = type;
	if (type == STANDARD){			//standard servo
		setServoMin(servo,PCA9685_DEF_MIN_ST_SERVO_VAL);
		setServoMax(servo,PCA9685_DEF_MAX_ST_SERVO_VAL);
	} else {
		setServoMin(servo,PCA9685_DEF_MIN_CNT_SERVO_VAL);
		setServoMax(servo,PCA9685_DEF_MAX_CNT_SERVO_VAL);
	}
}


void servo_PCA9685::setServoMin(uint8_t servo,uint16_t minVal)
{
	if (minVal > 4095) minVal = 4095;
	_servoMin[servo] = minVal;
}

void servo_PCA9685::setServoMax(uint8_t servo,uint16_t maxVal)
{
	if (maxVal > 4095) maxVal = 4095;
	_servoMax[servo] = maxVal;
}

uint16_t servo_PCA9685::getServoMin(uint8_t servo)
{
	return _servoMin[servo];
}

uint16_t servo_PCA9685::getServoMax(uint8_t servo)
{
	return _servoMax[servo];
}


void servo_PCA9685::setPWMFreq(float freq) 
{
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  /*
  float prescaleval = 25000000UL;
  prescaleval /= 4096;
  Serial.print("pulse(us/bit):");Serial.print(prescaleval); Serial.println(" us per bit");
  */
  float prescaleval = 6103.5156;
  prescaleval /= freq;
  prescaleval -= 1;
  uint8_t prescale = floor(prescaleval + 0.5);
  uint8_t oldmode = _receiveWire(PCA9685_MODE1);
  uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
  _sendWire(PCA9685_MODE1, newmode); // go to sleep
  _sendWire(PCA9685_PRE_SCALE, prescale); // set the prescaler
  _sendWire(PCA9685_MODE1, oldmode);
  delay(5);
  _sendWire(PCA9685_MODE1, oldmode | 0xA1);
}


void servo_PCA9685::setPWM(uint8_t servo, uint16_t on, uint16_t off) 
{
	_setPWM(servo,on,off);
}

void servo_PCA9685::moveServo(uint8_t servo, uint8_t pos) 
{
	if (servo > 15) servo = 15;
	uint16_t off = 0;
	if (_servoType[servo] == STANDARD){
		off = map(pos,0,255,_servoMin[servo],_servoMax[servo]);
	} else {
		//TODO
		off = map(pos,0,255,_servoMin[servo],_servoMax[servo]);
	}
	_setPWM(servo,0,off);
}

/*
void servo_PCA9685::setPin(uint8_t servo, uint16_t val, bool invert)
{
	val = min(val, 4095);
	if (invert) {
		if (val < 1) {
			_setPWM(servo, 4096, 0);// Special value for signal fully on.
		} else if (val > 4094) {
			_setPWM(servo, 0, 4096);// Special value for signal fully off.
		} else {
			_setPWM(servo, 0, 4095 - val);
		}
	} else {
		if (val > 4094) {
			_setPWM(servo, 4096, 0);// Special value for signal fully on.
		} else if (val < 1) {
			_setPWM(servo, 0, 4096); // Special value for signal fully off.
		} else {
			_setPWM(servo, 0, val);
		}
	}
}
*/