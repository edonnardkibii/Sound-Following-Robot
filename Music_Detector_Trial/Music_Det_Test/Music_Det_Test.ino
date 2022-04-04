#include <AFMotor.h>

#define leftMic A10
#define rightMic A12
#define frontMic A15


const int maxArray = 100;
short leftAve[maxArray]; short rightAve[maxArray]; short frontAve[maxArray];

int newAve=0; int oldAve=1;
float leftAcc=0; float rightAcc=0; float frontAcc=0;
float level=0; float levelAcc=0;
float leftLevel=0; float rightLevel=0; float frontLevel=0; 
float left=0; float right=0; float front=0;
float centerLR=0; float centerFB=0;  
int SpeedL=30;
int SpeedR=30;
/*---------- Gear-Motor Controls ----------*/
int gearsOff=0;
int forwardDrive=1; int reverseDrive=2;
int rightTurn=3; int leftTurn=4;

AF_DCMotor M1(1, MOTOR12_1KHZ);
AF_DCMotor M2(2, MOTOR12_1KHZ);
AF_DCMotor M3(3, MOTOR34_1KHZ);
AF_DCMotor M4(4, MOTOR34_1KHZ);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    /*---------- Intitialize Array ----------*/
  for(int i=0; i<maxArray; i++)
  {
    leftAve[i]=0; rightAve[i]=0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 // put your main code here, to run repeatedly:
  leftAve[newAve] = analogRead(leftMic);
  leftAcc += leftAve[newAve];
  leftAcc -= leftAve[oldAve];

  rightAve[newAve] = analogRead(rightMic);
  rightAcc += rightAve[newAve];
  rightAcc -= rightAve[oldAve];

  frontAve[newAve] = analogRead(frontMic);
  frontAcc += frontAve[newAve];
  frontAcc -= frontAve[oldAve];

   right = abs((rightAcc/maxArray)  -  (rightAve[newAve]));
  left = abs((leftAcc/maxArray)    -  (leftAve[newAve]));
  front = abs((frontAcc/maxArray)  -  (frontAve[newAve]));
  

  centerLR += right; centerLR -= left; centerLR /= 1.2;
  centerFB += front; centerFB /= 1.25;

  newAve++;
  if(newAve == maxArray)
  {
    newAve=0;
  }
  oldAve++;
  if(oldAve==maxArray)
  {
    oldAve=0;
  }


//if (centerFB > 0){
  //Serial.println("Forward");
  //motorControl(1);
  //delay(500);
  motorControl(1);
  if (centerLR > 7)
  {
    Serial.println("Links");
    motorControl(4);
    delay(500);
  }
  else if (centerLR < 7)
  {
    motorControl(3);
    Serial.println("Rechts");
    delay(500);
  }
  else
  {
    Serial.println("Vorne");
    motorControl(1);
    delay(500);
  }
//}


}

/*------------------------------ Control Gear Motors ------------------------------*/
#define maximum_speed 180
boolean drive = false;      //Default Setting: Motor Gears are off
//M1HL,M2HR,M3VR,M4VL
void motorControl(int control)
{
  switch(control)
  {
    case 0: //Gear-Motors are off
           M1.run(RELEASE); M2.run(RELEASE); M3.run(RELEASE); M4.run(RELEASE);
    break;
    
    case 1: //Drive the car forward
          if(!drive)
          {
            drive=true;
            M1.run(FORWARD); M2.run(FORWARD); M3.run(FORWARD); M4.run(FORWARD);
            
            for (int i=30; i < maximum_speed; i+=20) //slowly increase the speed of the car to the maximum speed
            {
              M1.setSpeed(i); M2.setSpeed(i); M3.setSpeed(i); M4.setSpeed(i);
              delay(5);
              SpeedL=i;
              SpeedR=i;
            }
            
     
           }
    break;
    
    case 2: //Reverse the car
            drive=true;
            M1.run(BACKWARD); M2.run(BACKWARD); M3.run(BACKWARD); M4.run(BACKWARD);
            
            for (int i=30; i < maximum_speed; i+=20) //slowly increase the speed of the car to the maximum speed
            {
              M1.setSpeed(i); M2.setSpeed(i); M3.setSpeed(i); M4.setSpeed(i);
              delay(5);
              SpeedL=i;
              SpeedR=i;
            
              
            }
            
    break;
    case 3: //Turn the car to the right
            drive=true;
            M1.run(BACKWARD); M2.run(BACKWARD); M3.run(BACKWARD); M4.run(BACKWARD);
                        if(SpeedL<SpeedR)
            SpeedL=SpeedR;
            else
            SpeedR=SpeedL;
            for (int i=SpeedL; i>SpeedL/2; i-=20) //slowly increase the speed of the car to the maximum speed
            {
                        
              M1.setSpeed(SpeedL); M2.setSpeed(i); M3.setSpeed(i); M4.setSpeed(SpeedL);
              delay(5);
              SpeedR=i;
              }
         
    break;
    
    case 4: //Turn the car to the left
              drive=true;
            M1.run(BACKWARD); M2.run(BACKWARD); M3.run(BACKWARD); M4.run(BACKWARD);
            if(SpeedL<SpeedR)
            SpeedL=SpeedR;
            else
            SpeedR=SpeedL;
            for (int i=SpeedR; i>SpeedR/2; i-=20) //slowly increase the speed of the car to the maximum speed
            {
                        
              M1.setSpeed(i); M2.setSpeed(SpeedR); M3.setSpeed(SpeedR); M4.setSpeed(i);
              delay(5);
              SpeedL=i;
              }
    break;
    
    default: //Gear-Motors are off by default
          M1.run(RELEASE); M2.run(RELEASE); M3.run(RELEASE); M4.run(RELEASE);
    break;
  }
}
