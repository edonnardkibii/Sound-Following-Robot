#include <AFMotor.h>

AF_DCMotor M1(1, MOTOR12_64KHZ); AF_DCMotor M2(2, MOTOR12_64KHZ); 
AF_DCMotor M3(3, MOTOR34_64KHZ); AF_DCMotor M4(4, MOTOR34_64KHZ);

const int rightMic  = 51;
const int leftMic   = 25;

boolean rightVal    = 0;
boolean leftVal     = 0;

boolean goesForward = false;

#define maximum_speed 220

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(rightMic, INPUT);
  pinMode(leftMic, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  rightVal  = digitalRead(rightMic);
  leftVal   = digitalRead(leftMic);

  Serial.print(rightVal);
  Serial.print("\t\t");
  Serial.println(leftVal);
  //delay(300);
  
  if(leftVal == LOW && rightVal == HIGH)
  {
    turnRight();
  }
  else if(leftVal == HIGH && rightVal == LOW)
  {
    turnLeft();
  }
  else if(leftVal == HIGH && rightVal == HIGH)
  {
    moveForward();
  }
  else
  {
    moveStop();
  }
}

void turnLeft()
{
  M1.run(BACKWARD); M1.setSpeed(90);
  M2.run(FORWARD);  M2.setSpeed(90);
  M3.run(FORWARD);  M3.setSpeed(90);
  M4.run(BACKWARD); M4.setSpeed(90);

  delay(300);

}

void turnRight()
{
  M1.run(FORWARD);   M1.setSpeed(90);
  M2.run(BACKWARD);  M2.setSpeed(90);
  M3.run(BACKWARD);  M3.setSpeed(90);
  M4.run(FORWARD);   M4.setSpeed(90);

  delay(300);
}

void moveForward()
{
    M1.run(FORWARD); M1.setSpeed(90); 
    M2.run(FORWARD); M2.setSpeed(90);
    M3.run(FORWARD); M3.setSpeed(90);
    M4.run(FORWARD); M4.setSpeed(90);

  delay(300);
}

void moveStop()
{
  M1.run(RELEASE); M2.run(RELEASE);
  M3.run(RELEASE); M4.run(RELEASE);
}
