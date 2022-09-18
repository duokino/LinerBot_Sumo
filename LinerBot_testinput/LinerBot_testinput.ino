#include "LinerBot.h"



// Define note and duration for Kolej Komuniti melody.
int melodyPitch[] = {NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_B5, NOTE_B5, NOTE_DS6, NOTE_DS6, NOTE_B5, NOTE_B5, NOTE_CS6};
int melodyDuration[] = {4, 16, 8, 4, 4, 16, 8, 4, 4, 8, 4, 8, 4, 8, 4, 8, 2};



void setup() {
  Serial.begin(115200);
  LinerBot.begin();

  // Play the Super Mario melody once.
  LinerBot.playMelody(melodyPitch, melodyDuration, 17);
}



void loop() {
  // Print value for Start Button.
  Serial.print("START:");
  Serial.print(digitalRead(START));

  
  // Print value for Opponent Sensors.
  Serial.print("  OPP_L:");
  Serial.print(digitalRead(OPP_L));

  Serial.print("  OPP_FL:");
  Serial.print(digitalRead(OPP_FL));

  Serial.print("  OPP_FC:");
  Serial.print(digitalRead(OPP_FC));

  Serial.print("  OPP_FR:");
  Serial.print(digitalRead(OPP_FR));

  Serial.print("  OPP_R:");
  Serial.print(digitalRead(OPP_R));
  
  
  // Print value for Edge Sensors (Analog).
  Serial.print("      EDGE_L:");
  Serial.print(analogRead(EDGE_L));

  Serial.print("  EDGE_R:");
  Serial.print(analogRead(EDGE_R));
  
  
  // Print value for Potentiometer
  Serial.print("  POT:");
  Serial.print(analogRead(POT));

/*
  // Print value for DIP Switch Setting.
  Serial.print("      MODE:");
  Serial.print(LinerBot.readDipSwitch());
*/
/*
  // Print value for Battery Voltage.
  Serial.print("  BATT:");
  Serial.print(LinerBot.readBatteryVoltage());
*/

  // Print line feed.
  Serial.println("");


  delay(250);
}
