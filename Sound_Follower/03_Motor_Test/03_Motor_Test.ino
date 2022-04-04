#include<AFMotor.h>

AF_DCMotor M1(1, MOTOR12_1KHZ);
AF_DCMotor M2(2, MOTOR12_1KHZ);
AF_DCMotor M3(3, MOTOR34_1KHZ);
AF_DCMotor M4(4, MOTOR34_1KHZ);

#define maximum_speed 180


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  M1.run(FORWARD); M2.run(FORWARD); M3.run(FORWARD); M4.run(FORWARD);
  
  for (int i = 80; i < maximum_speed; i=i+5)
  {
    M1.setSpeed(i); M2.setSpeed(i); M3.setSpeed(i); M4.setSpeed(i);
    delay(200);
  }
}
