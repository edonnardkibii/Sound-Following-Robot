#define LOG_OUT 1 // use the log output function
#define FHT_N 128 // set to 128 point fht
#include <FHT.h>

#define MIC_PIN 0

double prevVolts = 100.0;

void setup()
{
 Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop()
{
 while(1) { // reduces jitter
   for (int i = 0 ; i < FHT_N ; i++) // save 256 samples
   {
     int sample = analogRead(MIC_PIN);
     fht_input[i] = sample; // put real data into bins
   }
   fht_window(); // window the data for better frequency response
   fht_reorder(); // reorder the data before doing the fht
   fht_run(); // process the data in the fht
   fht_mag_log(); // take the output of the fht
 
   Serial.print("<");
   Serial.print(FHT_N/2);
   Serial.print(":");
   for (int curBin = 0; curBin < FHT_N/2; ++curBin)
   {
     if (curBin > 0)
       Serial.print(",");
     Serial.print(fht_log_out[curBin]);
   }
   Serial.println(">");
 }
}
