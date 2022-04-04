#include <AFMotor.h>

AF_DCMotor M1(1, MOTOR12_8KHZ); AF_DCMotor M2(2, MOTOR12_8KHZ); 
AF_DCMotor M3(3, MOTOR34_8KHZ); AF_DCMotor M4(4, MOTOR34_8KHZ);

//  Analog Mic Variables
const int arrayMax  = 100.0;
short     averageLeft[arrayMax];
short     averageRight[arrayMax];
int       averageIndexNew = 0;
int       averageIndexOld = 1;
float     accLeft = 0;
float     accRight = 0;
float     level = 0;
float     levelAcc = 0;
float     levelLeft = 0;
float     levelRight = 0;
float     left = 0;
float     right = 0;
float     center = 0;

const int rightMic  = 51;
const int leftMic   = 25;

boolean rightVal    = 0;
boolean leftVal     = 0;

boolean goesForward = false;

#define maximum_speed 220

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(38400);
  pinMode(rightMic, INPUT);
  pinMode(leftMic, INPUT);
  
  for( int i = 0 ; i < arrayMax ; i++ )
 {
         averageLeft[i] = 0;
         averageRight[i] = 0;    
 }
}

void loop() {
  // put your main code here, to run repeatedly:
  rightVal  = digitalRead(rightMic);
  leftVal   = digitalRead(leftMic);

 // Serial.print(rightVal);
  //Serial.print("\t\t");
  //Serial.println(leftVal);
  //delay(300);

  averageLeft[averageIndexNew] = analogRead(A14);       //Left Mic
  accLeft += averageLeft[averageIndexNew];
  accLeft -= averageLeft[averageIndexOld];       
        
  averageRight[averageIndexNew] = analogRead(A15);       //Right Mic
  accRight += averageRight[averageIndexNew];
  accRight -= averageRight[averageIndexOld];        
  right = (accRight/arrayMax) -  averageRight[averageIndexNew];
  right = abs(right);
  //Serial.print(right);
  //Serial.print("\t\t");
  left = (accLeft/arrayMax) -  averageLeft[averageIndexNew];
  left = abs(left);
  //Serial.print(left);
  //Serial.print("\t\t");
      
  center += (right * 1.05); // the left sensor is more sensitive than the right sensor (Left: 145mV Right: 138mV 145/138 = 1,05) 
  center -= (left) ;  
  center /= 1.25;           // divided by 1.25 so Center returns to 0 when it is quite : there is no sound input 
  //Serial.println(center);

  // increament Array Index
  averageIndexNew++; 
  if (averageIndexNew == arrayMax){  averageIndexNew = 0;} 
      averageIndexOld++;        
  if (averageIndexOld == arrayMax){  averageIndexOld = 0; }

  
  if(leftVal == LOW && rightVal == HIGH && center > 7)
  {
    turnRight();
  }
  else if(leftVal == HIGH && rightVal == LOW && center < -7)
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
