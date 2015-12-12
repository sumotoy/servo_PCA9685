#include <Wire.h>
#include <servo_PCA9685.h>

/*
 * ESP8266 I2C - pins
 * SDA: 4 - D2
 * SCL: 5 - D1
 */
servo_PCA9685 servo = servo_PCA9685();

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


}