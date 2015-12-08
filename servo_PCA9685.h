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

class servo_PCA9685 {

 public:
	#if defined(ESP8266)
		servo_PCA9685(const uint8_t adrs=0x40,const uint8_t scl_pin=5,const uint8_t sda_pin=4);
	#else
		servo_PCA9685(const uint8_t adrs=0x40);
	#endif
	void		begin(bool skipWireInit=false);
	void		reset(void);
	void 		setPWMFreq(float freq);
	void 		setPWM(uint8_t num, uint16_t on, uint16_t off);
	void 		setPin(uint8_t num, uint16_t val, bool invert);
	void 		setServoMin(uint16_t minVal);
	void 		setServoMax(uint16_t maxVal);
	uint16_t 	getServoMin(void);
	uint16_t 	getServoMax(void);
 protected:
 
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
		WIRE.requestFrom((uint8_t)_adrs, (uint8_t)1);
		return WIRE.read();
	}
	
	void _setPWM(uint8_t num, uint16_t on, uint16_t off)
	__attribute__((always_inline)) {
		WIRE.beginTransmission(_adrs);
		WIRE.write(PCA9685_BASEOUT_ON_L + (4 * num));
		WIRE.write(on);
		WIRE.write(on >> 8);
		WIRE.write(off);
		WIRE.write(off >> 8);
		WIRE.endTransmission();
	}
 
 private:
 #if defined(ESP8266)
	uint8_t 	_sclPin;
	uint8_t 	_sdaPin;
	uint16_t	_servoMin;
	uint16_t	_servoMax;
 #endif
	uint8_t 	_adrs;
	
};


#endif