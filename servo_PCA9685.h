/*************************************************** 
  Library for Adafruit 16-channel PWM & Servo driver
  //note by sumotoy
  Originally from Adafruit, remaked for working with modern MCU
  since Adafruit update rarely. 
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. 
  For Arduino UNOs:
  SCL -> Analog 5, SDA -> Analog 4
  For ESP8266:
  SCL -> GPIO 5 (nodemcu D2), SDA -> GPIO 4 (nodemcu D1)

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#ifndef _PCA9685_H_
#define _PCA9685_H_

#include "Arduino.h"
#include <Wire.h>

#include "_includes/PCA9685_registers.h"
#include "_settings/PCA9685_settings.h"

enum PAC9685mechanical {STANDARD=0,CONTINUOUS };

class servo_PCA9685 {

 public:
	#if defined(ESP8266)
		servo_PCA9685(const uint8_t adrs=0x40,const uint8_t scl_pin=5,const uint8_t sda_pin=4);
	#else
		servo_PCA9685(const uint8_t adrs=0x40);
	#endif
	void		begin(bool skipWireInit=false);
	void 		setPWMFreq(float freq);
	void		moveServo(uint8_t servo,uint8_t pos);
	void 		setPWM(uint8_t servo, uint16_t on, uint16_t off);
	void		setServoType(uint8_t servo,const enum PAC9685mechanical type=STANDARD);
	void 		setServoMin(uint8_t servo,uint16_t minVal);
	void 		setServoMax(uint8_t servo,uint16_t maxVal);
	uint16_t 	getServoMin(uint8_t servo);
	uint16_t 	getServoMax(uint8_t servo);
	//void 		setPin(uint8_t num, uint16_t val, bool invert);//will be deprecated soon
 protected:
	uint8_t 	_adrs;
	
	void _sendWire(uint8_t reg, uint8_t val)
	__attribute__((always_inline)) {
		WIRE.beginTransmission(_adrs);
		WIRE.write(reg);
		WIRE.write(val);
		WIRE.endTransmission();
	}
	
	uint8_t _receiveWire(uint8_t reg) 
	__attribute__((always_inline)) {
		WIRE.beginTransmission(_adrs);
		WIRE.write(reg);
		WIRE.endTransmission();
		WIRE.requestFrom((uint8_t)_adrs,(uint8_t)1);
		return WIRE.read();
	}
	
	void _setPWM(uint8_t servo, uint16_t on, uint16_t off)
	__attribute__((always_inline)) {
		WIRE.beginTransmission(_adrs);
		WIRE.write((uint8_t)(PCA9685_BASEOUT_ON_L + (4 * servo)));
		WIRE.write(on & 0xFF);
		WIRE.write(on >> 8);
		WIRE.write(off & 0xFF);
		WIRE.write(off >> 8);
		WIRE.endTransmission();
	}
 
 private:
 #if defined(ESP8266)
	uint8_t 	_sclPin;
	uint8_t 	_sdaPin;
 #endif
	bool		_servoType[16];
	uint16_t	_servoMin[16];
	uint16_t	_servoMax[16];
	
};


#endif