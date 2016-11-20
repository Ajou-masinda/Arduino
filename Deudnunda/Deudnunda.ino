#include "pitches.h"

int mq2_sensorValue; // GAS
int mq135_sensorValue; // CO2, CO
int buzzer_pin = 2;
int mq135_pin = 0; // A0
int mq2_pin = 3; // A3
int melody = NOTE_G4;

void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
  
}

void loop()
{
  char msg[11];
  mq2_sensorValue = analogRead(mq2_pin);      // read analog input pin 0
  mq135_sensorValue = analogRead(mq135_pin);  
  sprintf(msg, "%4d, %4d\n", mq2_sensorValue, mq135_sensorValue);
  Serial.print(msg);
  /*
  Serial.print(mq2_sensorValue);
  Serial.print(",");
  Serial.print(mq135_sensorValue);  // prints the value read
  Serial.println();
  */
  delay(1000);                        // wait 100ms for next reading
  
  if(mq2_sensorValue > 500 || mq135_sensorValue > 800)
      tone(buzzer_pin, melody, 50);
  else
    noTone(buzzer_pin);

}
