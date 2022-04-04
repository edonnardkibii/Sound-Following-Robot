
#include <FreqCount.h>                                //FreqCount library for you know...counting frequencies
                                                    //find it here: https://github.com/PaulStoffregen/FreqCount
 // pin d5 verwenden
void setup()   {                
 Serial.begin(9600);

  delay(2000);                                        //bask in the glory of LadyAda

  FreqCount.begin(1000);                              //start counting 1,2,3,4...
 
}
 
 
void loop() {
 
 
  if (FreqCount.available()) {                        //if the code if working
    float count = FreqCount.read();                   //create float var called count and populate it with current frequency count
    float period = (1/count);                         //create float var called period and populate it with the inverse of the frequency
    Serial.print(count);                             // Printed die aktuelle frequenz
    Serial.print("\t\t");
    Serial.println(period*1000);                      // printed die periode in millisekunden
  }
  
  
  
}                                                     //That is all, carry on.
