/*
	wave all servos in sequence
 This time it uses the new simple command moveServo
 that uses a range from 0..255
*/

#include <Wire.h>
#include <servo_PCA9685.h>

#define MAX_SERVOS	16

/*
 * ESP8266 I2C - pins
 * SDA: 4 - D2
 * SCL: 5 - D1
 */



servo_PCA9685 servo = servo_PCA9685();

uint8_t servonum = 0;

void setup() {
#if defined(ESP8266)
  Serial.begin(115200);
#else
  Serial.begin(38400);
#endif
  Serial.println("\nservo start");
  servo.begin();
}

void loop() {
  // Drive each servo one at a time
  Serial.println(servonum);
  for (uint8_t i = 0; i < 255; i++) {
    servo.moveServo(servonum, i);
  }
  delay(200);
  for (uint8_t i = 254; i > 0; i--) {
    servo.moveServo(servonum, i);
  }
  delay(200);

  servonum ++;
  if (servonum > MAX_SERVOS) servonum = 0;

}
