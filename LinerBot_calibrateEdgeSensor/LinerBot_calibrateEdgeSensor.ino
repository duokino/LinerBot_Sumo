#include <EEPROM.h>
#include "LinerBot.h"



// Note and duration for Kolej Komuniti melody.
int melodyPitch[] = {NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_B5, NOTE_B5, NOTE_DS6, NOTE_DS6, NOTE_B5, NOTE_B5, NOTE_CS6};
int melodyDuration[] = {4, 16, 8, 4, 4, 16, 8, 4, 4, 8, 4, 8, 4, 8, 4, 8, 2};

// Note and duration for edge sensor calibration.
int calPitch[] = {NOTE_C5, NOTE_G5};
int calDuration[] = {10, 10};



void setup() {
  Serial.begin(115200);
  LinerBot.begin();

  
  // Enter edge sensor calibration mode if the START button is pressed
  // during reset.
  if (digitalRead(START) == LOW) {
    // Beep to indicate we are in the calibration mode.
    LinerBot.playMelody(calPitch, calDuration, 2);

    // Wait until start button is released.
    while ((digitalRead(START) == LOW));
    delay(100);

    // Calibrate the edge sensors.
    LinerBot.calibrateEdgeSensors(); 
  }

  
  // Play the Kolej Komuniti melody once.
  LinerBot.playMelody(melodyPitch, melodyDuration, 17);
}



void loop() {
  // Turn on LED if edge is detected.
  if (LinerBot.isEdgeDetected(EDGE_L) || LinerBot.isEdgeDetected(EDGE_R)) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }

  // Print the sensors' threshold via serial port.
  Serial.print ("Left: ");
  Serial.print (LinerBot.readEdgeSensorThreshold(EDGE_L));
  Serial.print ("    Right: ");
  Serial.println (LinerBot.readEdgeSensorThreshold(EDGE_R));

  delay(250);
}
