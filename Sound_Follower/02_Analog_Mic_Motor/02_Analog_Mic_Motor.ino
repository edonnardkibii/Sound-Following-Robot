#include <AFMotor.h>

#define MAX_DISTANCE 200
#define MAX_SPEED 200

int speedSet = 30;

AF_DCMotor M1(1, MOTOR12_64KHZ); AF_DCMotor M2(2, MOTOR12_64KHZ); 
AF_DCMotor M3(3, MOTOR34_64KHZ); AF_DCMotor M4(4, MOTOR34_64KHZ);

//  variables for Microphones
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

void setup() {
  // put your setup code here, to run once:
//  pinMode(rightMic, INPUT);
 // pinMode(leftMic, INPUT);
 
 Serial.begin(38400);
  
 for( int i = 0 ; i < arrayMax ; i++ )
 {
         averageLeft[i] = 0;
         averageRight[i] = 0;    
 }
  
}

void loop() {
  //moveForward();
  // put your main code here, to run repeatedly:
  averageLeft[averageIndexNew] = analogRead(A14);       //Left Mic
  accLeft += averageLeft[averageIndexNew];
  accLeft -= averageLeft[averageIndexOld];       
        
  averageRight[averageIndexNew] = analogRead(A15);       //Right Mic
  accRight += averageRight[averageIndexNew];
  accRight -= averageRight[averageIndexOld];        
  right = (accRight/arrayMax) -  averageRight[averageIndexNew];
  right = abs(right);
  Serial.print(right);
  Serial.print("\t\t");
  left = (accLeft/arrayMax) -  averageLeft[averageIndexNew];
  left = abs(left);
  Serial.print(left);
  Serial.print("\t\t");
      
  center += (right * 1.05);
  center -= (left/* * 1.1*/) ;  // Left * 1.1 as my left hand microphone seems at little deaf // left is not as responsive as right microphone
  center /= 1.25;     // dived by 1.25 so C returns to 0 when it is quite : there is no sound input 
  Serial.println(center);
    
  // increament Array Index
  averageIndexNew++; 
  if (averageIndexNew == arrayMax){  averageIndexNew = 0;} 
      averageIndexOld++;        
  if (averageIndexOld == arrayMax){  averageIndexOld = 0; }
  ///////////////

  if(center > 7)
  {
    turnRight();
  }
  else if (center < -7)
  {
    turnLeft();
  }
  else
  {
    moveForward();
  }
  

}

void moveForward()
{

    M1.run(FORWARD); M1.setSpeed(85); 
    M2.run(FORWARD); M2.setSpeed(85);
    M3.run(FORWARD); M3.setSpeed(85);
    M4.run(FORWARD); M4.setSpeed(85);
    
}

void turnRight()
{
  M1.run(FORWARD);   M1.setSpeed(85);
  M2.run(BACKWARD);  M2.setSpeed(85);
  M3.run(BACKWARD);  M3.setSpeed(85);
  M4.run(FORWARD);   M4.setSpeed(85);

  delay(100);
}
void turnLeft()
{
  M1.run(BACKWARD); M1.setSpeed(85);
  M2.run(FORWARD);  M2.setSpeed(85);
  M3.run(FORWARD);  M3.setSpeed(85);
  M4.run(BACKWARD); M4.setSpeed(85);

  delay(100);
  
}
