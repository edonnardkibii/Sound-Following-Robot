
#include <FreqMeasure.h>                                //FreqCount library for you know...counting frequencies
                                                    //find it here: https://github.com/PaulStoffregen/FreqCount
 // pin d8 verwenden
void setup()   {                
 
  Serial.begin(57600);
  //delay(2000);                                        //bask in the glory of LadyAda

  FreqMeasure.begin();                              //start counting 1,2,3,4...
 
}

double sum = 0;
int count = 0;
 
void loop() {
  if (FreqMeasure.available()) {
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > 30) {
      float frequency = FreqMeasure.countToFrequency(sum / count);
      Serial.println(frequency);
      sum = 0;
      count = 0;
    }
  }
}                                                     

/* FreqMeasureMulti - Example with serial output
 * http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
 *
 * This example code is in the public domain.
 */
/*
#include <FreqMeasureMulti.h>
//#include "elapsedMillis.h"

// Measure 3 frequencies at the same time! :-)
FreqMeasureMulti freq1;
FreqMeasureMulti freq2;
FreqMeasureMulti freq3;

double starttime = 0;
double measuredtime = 500000;

void setup() {
  Serial.begin(57600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(10);
  Serial.println("FreqMeasureMulti Begin");
  delay(10);
  freq1.begin(6);
  freq2.begin(9);
  freq3.begin(10);
}

float sum1=0, sum2=0, sum3=0;
int count1=0, count2=0, count3=0;
//elapsedMillis timeout;

void loop() {
  if (freq1.available()) {
    sum1 = sum1 + freq1.read();
    count1 = count1 + 1;
  }
  if (freq2.available()) {
    sum2 = sum2 + freq2.read();
    count2 = count2 + 1;
  }
  if (freq3.available()) {
    sum3 = sum3 + freq3.read();
    count3 = count3 + 1;
  }

  
  // print results every half second
  if ((micros() - starttime) >= measuredtime) {
    if (count1 > 0) {
      Serial.print(freq1.countToFrequency(sum1 / count1));
    } else {
      Serial.print("(no pulses)");
    }
    Serial.print(",  ");
    if (count2 > 0) {
      Serial.print(freq2.countToFrequency(sum2 / count2));
    } else {
      Serial.print("(no pulses)");
    }
    Serial.print(",  ");
    if (count3 > 0) {
      Serial.print(freq3.countToFrequency(sum3 / count3));
    } else {
      Serial.print("(no pulses)");
    }
    Serial.println();
    sum1 = 0;
    sum2 = 0;
    sum3 = 0;
    count1 = 0;
    count2 = 0;
    count3 = 0;
    //timeout = 0;
  }
}
*/
