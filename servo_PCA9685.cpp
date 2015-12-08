
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
	setServoMin(PCA9685_DEF_MIN_SERVO_VAL);
	setServoMax(PCA9685_DEF_MAX_SERVO_VAL);
	reset();
	setPWMFreq(60);// Analog servos run at ~60 Hz updates
}

void servo_PCA9685::reset(void)
{
	_sendWire(PCA9685_MODE1, 0x0);
}

void servo_PCA9685::setServoMin(uint16_t minVal)
{
	if (minVal > 4095) minVal = 4095;
	_servoMin = minVal;
}

void servo_PCA9685::setServoMax(uint16_t maxVal)
{
	if (maxVal > 4095) maxVal = 4095;
	_servoMax = maxVal;
}

uint16_t servo_PCA9685::getServoMin(void)
{
	return _servoMin;
}

uint16_t servo_PCA9685::getServoMax(void)
{
	return _servoMax;
}


void servo_PCA9685::setPWMFreq(float freq) 
{
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  /*
  float prescaleval = 25000000UL;
  prescaleval /= 4096;
  Serial.print("pulse(us/bit):");Serial.print(prescaleval); Serial.println(" us per bit");
  */
  float prescaleval = 6103.52;
  prescaleval /= freq;
  prescaleval -= 1;
  #if defined(ENABLE_DEBUG_OUTPUT)
    Serial.print("Estimated pre-scale: "); 
	Serial.println(prescaleval);
  #endif
  uint8_t prescale = floor(prescaleval + 0.5);
  #if defined(ENABLE_DEBUG_OUTPUT)
    Serial.print("Final pre-scale: "); 
	Serial.println(prescale);
  #endif
  
  uint8_t oldmode = _receiveWire(PCA9685_MODE1);
  uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
  _sendWire(PCA9685_MODE1, newmode); // go to sleep
  _sendWire(PCA9685_PRE_SCALE, prescale); // set the prescaler
  _sendWire(PCA9685_MODE1, oldmode);
  delay(5);
  _sendWire(PCA9685_MODE1, oldmode | 0xA1);
}


void servo_PCA9685::setPWM(uint8_t num, uint16_t on, uint16_t off) 
{
		//Serial.print("Setting PWM "); 
		//Serial.print(num); 
		//Serial.print(": "); 
		//Serial.print(on); 
		//Serial.print("->"); 
		//Serial.println(off);
	_setPWM(num,on,off);
}

void servo_PCA9685::setPin(uint8_t num, uint16_t val, bool invert)
{
	val = min(val, 4095);
	if (invert) {
		if (val < 1) {
			_setPWM(num, 4096, 0);// Special value for signal fully on.
		} else if (val > 4094) {
			_setPWM(num, 0, 4096);// Special value for signal fully off.
		} else {
			_setPWM(num, 0, 4095 - val);
		}
	} else {
		if (val > 4094) {
			_setPWM(num, 4096, 0);// Special value for signal fully on.
		} else if (val < 1) {
			_setPWM(num, 0, 4096); // Special value for signal fully off.
		} else {
			_setPWM(num, 0, val);
		}
	}
}