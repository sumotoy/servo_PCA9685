# servo_PCA9685
A custom version from the Adafruit library that works with many cpu's included ESP8266.
The adafruit library doesn't work out of the box, it contains also some unused code and it's pretty old, unfortunatly the rarely update libraries (some pull request rely for years) and I need to use this chip so I wroted a compatible library just for servo's that works with any CPU I have (included ESP8266) and...voilà.<br>
This version of the library it's servo dedicated so commands changed and it's not back compatible with adafruit one, hower it's much simple to use!<br>
Have fun!<br>

Simple to use<br>
This library it's really simple to use, once instanced it needs only begin and it's ready. To move a servo the only command you need it's clearly moveServo(servo.num, servo.val) where servo.num it's one of the possible servos (0..15) and servo.val it's a number between 0...255..
