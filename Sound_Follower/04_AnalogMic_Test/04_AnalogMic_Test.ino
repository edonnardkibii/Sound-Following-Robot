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
  Serial.begin(38400);
  
  for( int i = 0 ; i < arrayMax ; i++ )
  {
         averageLeft[i] = 0;
         averageRight[i] = 0;    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  averageLeft[averageIndexNew] = analogRead(A4);       //Left Mic
  accLeft += averageLeft[averageIndexNew];
  accLeft -= averageLeft[averageIndexOld];       
        
  averageRight[averageIndexNew] = analogRead(A5);       //Right Mic
  accRight += averageRight[averageIndexNew];
  accRight -= averageRight[averageIndexOld];     
     
  right = (accRight/arrayMax) -  averageRight[averageIndexNew];
  right = abs(right);
  
  left = (accLeft/arrayMax) -  averageLeft[averageIndexNew];
  left = abs(left);

  
  center += right;
  center -= (left/* * 1.1*/) ;  // Left * 1.1 as my left hand microphone seems at little deaf // left is not as responsive as right microphone
  center /= 1.25;     // dived by 1.25 so C returns to 0 when it is quite : there is no sound input 
  //Serial.println(center);
    
  // increament Array Index
  averageIndexNew++; 
  if (averageIndexNew == arrayMax){  averageIndexNew = 0;} 
      averageIndexOld++;        
  if (averageIndexOld == arrayMax){  averageIndexOld = 0; }

  Serial.print(right);
  Serial.print("\t\t");
  Serial.print(left);
  Serial.print("\t\t");
  Serial.println(center);

  delay(300);
}
