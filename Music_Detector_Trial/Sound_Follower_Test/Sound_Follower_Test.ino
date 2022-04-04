

/*----------------------------------------  Variables  ----------------------------------------*/
//Microphone Sensor
#define leftMic A0
#define rightMic A1
#define frontMic A2
#define backMic A3

const int maxArray = 100;
short leftAve[maxArray]; short rightAve[maxArray]; short frontAve[maxArray]; short backAve[maxArray];

int newAve=0; int oldAve=1;
float leftAcc=0; float rightAcc=0; float frontAcc=0; float backAcc=0;
float level=0; float levelAcc=0;
float leftLevel=0; float rightLevel=0; float frontLevel=0; float backLevel=0; 
float left=0; float right=0; float front=0; float back=0;
float centerLR=0; float centerFB=0;  
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  /*---------- Intitialize Array ----------*/
  for(int i=0; i<maxArray; i++)
  {
    leftAve[i]=0; rightAve[i]=0;
  }

  /*-------------Rotate Servo Motor & scan the surrounding-------------*/
  //servo_.attach(10);  //Attach PWM Pin to Digital Pin 10
  //servo_.write(90);   //Intitialize default position of the Ultra-Sonic sensor

}

void loop() {
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

  backAve[newAve] = analogRead(backMic);
  backAcc += backAve[newAve];
  backAcc -= backAve[oldAve];



  right = abs((rightAcc/maxArray)  -  (rightAve[newAve]));
  left = abs((leftAcc/maxArray)    -  (leftAve[newAve]));
  front = abs((frontAcc/maxArray)  -  (frontAve[newAve]));
  back = abs((backAcc/maxArray)    -  (backAve[newAve]));

  centerLR += right; centerLR -= left; centerLR /= 1.25;
  centerFB += front; centerFB -= back; centerFB /= 1.25;

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


  //I. Music source between 0° & 90°
  if(centerLR > 7 && centerFB > 7)
  {
    Serial.println("Music source position I");
  } 
  
  //II. Music source between 90° to 180°
  else if(centerLR < 7 && centerFB > 7)
  {
    Serial.println("Music source position II");
  } 
  
  //III. Music source between 180° to 270° 
  else if(centerLR < 7 && centerFB < 7)
  {
    Serial.println("Music source position III");
  } 
  
  //IV. Music source between 270° to 360°
    else if(centerLR > 7 && centerFB < 7)
  {
    Serial.println("Music source position IV");
  } 
  
/*
Serial.println(front);
Serial.println(back);
Serial.println(right);
Serial.println(left);
Serial.println();
delay(500);
*/
}
