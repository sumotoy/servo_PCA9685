/*
 * This library it's dedicated to servo, here's a simple example for a single servo
 */

#include <Wire.h>
#include <servo_PCA9685.h>


servo_PCA9685 servo = servo_PCA9685();

#define TEST_SERVO_NUM  0//0..15

void setup() {
#if defined(ESP8266)
  Serial.begin(115200);
#else
  Serial.begin(38400);
#endif
  Serial.println("\nservo start");
  servo.begin();
  //All servos (16) now are STANDARD with limits declared in settings.
  //If default values are OK for you you don't need the following commands.
  //Eventually you can change a signle servo as this:
  servo.setServo(TEST_SERVO_NUM, STANDARD); //this is a standard (180°) servo
  servo.setServoMin(TEST_SERVO_NUM, 150);//A new MIN value
  servo.setServoMax(TEST_SERVO_NUM, 600);//A new MAX value
}

void loop() {
  for (uint8_t i = 0; i < 255; i++) {
    uint16_t val = map(i, 0, 254, servo.getServoMin(TEST_SERVO_NUM), servo.getServoMax(TEST_SERVO_NUM));//this map 0..255 to min..max servo range
    servo.setPWM(TEST_SERVO_NUM, 0, val);//update server
    delay(50);
  }
}