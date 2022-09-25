#include "LinerBot_Maker.h"



// Define note and duration for Kolej Komuniti melody.
int melodyPitch[] = {NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_B5, NOTE_B5, NOTE_DS6, NOTE_DS6, NOTE_B5, NOTE_B5, NOTE_CS6};
int melodyDuration[] = {4, 16, 8, 4, 4, 16, 8, 4, 4, 8, 4, 8, 4, 8, 4, 8, 2};



void setup() {
  Serial.begin(115200);
  LinerBot.begin();

  // Stop the robot.
  LinerBot.stop();

  // Play the Kolej Komuniti melody once.
  LinerBot.playMelody(melodyPitch, melodyDuration, 17);

  // Wait here until start button is pressed.
  while (digitalRead(START) == HIGH);
}



void loop() {
  
  
  // Turn on LED and move forward at 50% speed.
  digitalWrite(LED, HIGH);
  LinerBot.moveForward(128);
  delay(1000);

  // Turn off LED and move backward at 50% speed.
  digitalWrite(LED, LOW);
  LinerBot.moveBackward(128);
  delay(1000);

  // Turn on LED and turn left at 50% speed.
  digitalWrite(LED, HIGH);
  LinerBot.turnLeft(128);
  delay(1000);

  // Turn off LED and turn right at 50% speed.
  digitalWrite(LED, LOW);
  LinerBot.turnRight(128);
  delay(1000);
  

  //LinerBot.moveForward(128);

  //LinerBot.setMotorSpeed(MOTOR_L, 100);
  //LinerBot.setMotorSpeed(MOTOR_R, -100);
  

}
