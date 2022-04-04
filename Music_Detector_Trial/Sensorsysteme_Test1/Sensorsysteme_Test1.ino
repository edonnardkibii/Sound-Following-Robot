#include <SPI.h>
#include <SdFat.h>
#define SPI_SPEED SD_SCK_MHZ(50)

#define LIN_OUT8 1  //use the log output function
#define FHT_N 256   //set to 256 point fht
#define SCALE 256

#include <FHT.h>  //include the library
#include <avr/pgmspace.h>
#include "EEPROM.h"
SdFat SD;
File file;            // An dieser Stelle wird die Variable "file" als File (dts. Datei) deklariert.

int nb = 1;
unsigned int average = 0;
byte keyPoint[256];
boolean modeRecognition = true;
String music_id;
byte max_def = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F(" -Shazam-Uno- "));  //SerialPlotter
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  if (!SD.begin(10, SPI_SPEED)) //Wenn die SD-Karte nicht(!SD.beginn)gefunden werden kann;....
  {
    Serial.println(F("############################################################################"));
    Serial.println(F("Initialisierung fehlgeschlagen! Keine SD-Karte entdeckt oder falsches Format")); // soll eine Fehlermeldung ausgegeben werden..
    Serial.println(F("#                                                                          #"));
    Serial.println(F("############################################################################"));
    Serial.println(F("############################################################################"));
    return;
  }
  keyPoint[0] = 0;
  delay(12000);
  
  Serial.println(F("The System is Ready"));  //SerialPlotter
  ADCSRA = 0xe5; //set the adc to free running mode
  ADMUX = 0x40; //use adc0
  DIDR0 = 0x01; //turn off the digital input for adc0
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(F("##### Menue #####"));  //SerialPlotter
  Serial.print(F("Aktuell sind "));  //SerialPlotter
  Serial.print(EEPROM.read(0));  //SerialPlotter
  Serial.println(F(" song's gespeichert"));  //SerialPlotter
  Serial.println(F("Auswahl"));  //SerialPlotter
  Serial.println(F("Musik scannen      (r)"));  //SerialPlotter
  Serial.println(F("Musik hinzufügen   (b)"));  //SerialPlotter
  while (1)
  {
    if (Serial.available() > 0)
    {
      char c = Serial.read();
      if (c == 'r')
      {
        Serial.println(F("Musik erkennung aktiviert"));  //SerialPlotter
        modeRecognition = true;
        break;
      }
      if (c == 'b')
      {
        Serial.println(F("Musik hinzufügen"));  //SerialPlotter
        modeRecognition = false;
        break;
      }
    }
  }

  if (!modeRecognition)
  {
    EEPROM.write(0, EEPROM.read(0) + 1);
    music_id = String(EEPROM.read(0));
    file = SD.open("music_id.txt", O_CREAT | O_WRITE);//#############geändert .txt
    delay(5000);
    Serial.print(F("Scanne Musik...."));  //SerialPlotter
    Serial.println(EEPROM.read(0));  //SerialPlotter
    file.close();
  }
  Serial.read();

  while (1)
  {
    while (nb <= 255)
    {
      for (int i = 0; i < FHT_N; i++) //Save 256 samples
      {
        while (!(ADCSRA & 0x10));    //wait for adc to be ready
        ADCSRA = 0xf5;              //restart adc
        byte m = ADCL;
        byte j = ADCH;
        int k = (j << 8) | m;       //form into an int
        k -= 0x0200;                //form into a signed int
        k <<= 6;                    //form into a 16b signed int
        fht_input[i] = k;           //put real data into bins
      }
      fht_window();                 //window the data for better frequency response
      fht_reorder();                //reorder the data before doing the fht
      fht_run();                    //process the data in the fht
      fht_mag_lin8();               //take the output of the fht

      //Calculate average
      average = 0;
      for (unsigned char l = 20; l < (FHT_N / 2); l++)
      {
        average = average + fht_lin_out8[l];
      }
      average = 10 + average /(FHT_N / 2);

      //Find the index of the dominant peak
      byte indice_max = 0, maximum = 0;

      for (unsigned char j = 20; j <= (FHT_N / 2) - 1; j++)
      {
        if (fht_lin_out8[j] > maximum && fht_lin_out8[j] > 3 * average)
        {
          maximum = fht_lin_out8[j];
          indice_max = j;
        }
      }

      if (indice_max != 0)
      {
        keyPoint[nb] = indice_max;
        nb++;
        Serial.println(indice_max);

        if (!modeRecognition)
        {
          if (Serial.available() > 0)
          {
            nb = 300;
            Serial.println(F("Aufnahme beendet"));
          }
        }
      }
    }

    if (modeRecognition) {

      break;
    }
    else
    {
      file = SD.open("music_id.txt", O_CREAT | O_APPEND | O_WRITE);//#############geändert .txt
      file.write(keyPoint, sizeof(keyPoint));
      Serial.print(F("Größe: "));  //SerialPlotter
      Serial.println(file.size());
      file.close();
      if (nb = 300)
      {
        byte h = 0;
        //Register name of the song
        Serial.println(F("Name des Titels eingeben"));  //SerialPlotter
        Serial.read();

        while (1)
        {
          if (Serial.available() > 0)
          {
            fht_lin_out8[h] = Serial.read();
            Serial.print(char(fht_lin_out8[h]));

            if (fht_lin_out8[h] == '|')//eventuell i
            {
              break;
            }
            h++;
          }
        }
        //Store
        file = SD.open("m.txt", O_CREAT | O_APPEND | O_WRITE);//#######geändert .txt
        file.write(fht_lin_out8, 256);
        Serial.println(file.size());
        file.close();
        Serial.println(F("Musik gespeichert"));  //SerialPlotter
        break;
      }
      for (nb = 0; nb <= 255; nb++)
      {
        keyPoint[nb] = 0;
      }
      nb = 0;
    }
  }

  //Compare with data in the database
  if (modeRecognition)
  {
    byte music_found = 0;
    average = 0;

    for (byte p = 1; p <= EEPROM.read(0); p++)
    {
      Serial.print(F("Suche in Datenbank n° "));  //SerialPlotter
      Serial.println(p);
      
      music_id = String(p);
      
      
      file = SD.open("music_id.txt", FILE_READ);//#######geändert .txt und""
      byte nbr_buffer = file.size() / 256;
      byte maximum, id_max = 0;
      nb = 0;

      for (byte i = 0; i < nbr_buffer; i++)
      {
        file = SD.open("music_id.txt", FILE_READ);//#######geändert .txt und""
        file.seek(i * 256);
        file.read(fht_lin_out8, 256);
        byte simi = 0;
        maximum = 0;

        for (byte j = 0; j != 255; j++)
        {
          simi = 0;
          for (byte k = 0; k != 255; k++)
          {
            if (j + k <= 255)
            {
              if ((fht_lin_out8[j + k] + 1 >= keyPoint[k]) && (keyPoint[k] >= fht_lin_out8[j + k] - 1))
              {
                simi++;
              }
            }
          }
          if (simi > maximum)
          {
            maximum = simi;
            id_max = j;
          }
        }
        //Calculate the percentage for the index of the n-th buffer on the rest of the database
        if (!(id_max == 0 || nbr_buffer - 1 == i))
        {
          file.seek((i + 1) * 256);
          file.read(fht_lin_out8, 256);
          file.close();
          simi = 0;
          for (byte k = 1; k <= id_max; k++)
          {
            if ((fht_lin_out8[k] + 1 >= keyPoint[255 - id_max + k]) && (keyPoint[255 - id_max + k] >= fht_lin_out8[k] - 1))
            {
              simi++;
            }
          }

          maximum = maximum + simi;
          if (maximum > nb)
          {
            nb = maximum;
            Serial.print(F("Das maximum ist: "));  //SerialPlotter
            Serial.println(nb);
          }
        }
        else
        {
          max_def = maximum;
        }
      }
      if (nb > average)
      {
        average = nb;
        music_found = p;
      }
    }

    file = SD.open("m.txt", FILE_READ);
    file.seek((music_found - 1) * 256);
    file.read(fht_lin_out8, 256);
    file.close();

    nb = 0;
    Serial.println(F("Der gefunden Song lautet: "));  //SerialPlotter
    while (char(fht_lin_out8[nb]) != '|')
    {
      Serial.print(char(fht_lin_out8[nb]));
      nb++;
    }
  }
  Serial.println(F(""));  //SerialPlotter
  nb = 0;
  average = 0;
}
