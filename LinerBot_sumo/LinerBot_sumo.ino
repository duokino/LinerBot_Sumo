#include "LinerBot.h"



// Edge Sensor Threshold.
#define EDGE_L_THRESHOLD  720
#define EDGE_R_THRESHOLD  638

// Constants for direction.
#define LEFT    0
#define RIGHT   1



// Searching direction.
int searchDir = LEFT;

// Define note and duration for Kolej Komuniti melody.
int melodyPitch[] = {NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_B5, NOTE_B5, NOTE_DS6, NOTE_DS6, NOTE_B5, NOTE_B5, NOTE_CS6};
int melodyDuration[] = {4, 16, 8, 4, 4, 16, 8, 4, 4, 8, 4, 8, 4, 8, 4, 8, 2};



/*******************************************************************************
 * Setup
 * This function runs once after reset.
 *******************************************************************************/
void setup() {
  LinerBot.begin();

  // Play the Super Mario melody once.
  LinerBot.playMelody(melodyPitch, melodyDuration, 17);

  // Wait until start button is pressed and released.
  while (digitalRead(START));
  while (!digitalRead(START));

  // Turn on the User LED.
  digitalWrite(LED, HIGH);
  
  // Start routine..
  startRoutine();
}



/*******************************************************************************
 * Main program loop.
 *******************************************************************************/
void loop() {
  // Edge is detected on the left.
  if (analogRead(EDGE_L) < EDGE_L_THRESHOLD) {
    // Back off and make a U-Turn to the right.
    backoff(RIGHT);

    // Toggle the search direction.
    searchDir ^= 1;
  }
  
  // Edge is detected on the right.
  else if (analogRead(EDGE_R) < EDGE_R_THRESHOLD) {
    // Back off and make a U-Turn to the left.
    backoff(LEFT);
    
    // Toggle the search direction.
    searchDir ^= 1;
  }

  // Edge is not detected.
  else {
    // Keep searching if opponent is not detected.
    if ( (digitalRead(OPP_FC) == HIGH) &&
         (digitalRead(OPP_FL) == HIGH) &&
         (digitalRead(OPP_FR) == HIGH) &&
         (digitalRead(OPP_L) == HIGH)  &&
         (digitalRead(OPP_R) == HIGH) ) {
      search(searchDir);
    }
    
    // Attack if opponent is in view.
    else {
      attack();
    }
  }

  // Stop the robot if the button is pressed.
  if (digitalRead(START) == 0) {
    // Stop the motors.
    LinerBot.setMotorSpeed(MOTOR_L, 0);
    LinerBot.setMotorSpeed(MOTOR_R, 0);

    // Loop forever here.
    while (1);
  }

}



/*******************************************************************************
 * Start Routine
 * This function should be called once only when the game start.
 *******************************************************************************/
void startRoutine() {
  // Start delay.
  delay(1000);
  
  // Turn right around 45 degrees.
  LinerBot.setMotorSpeed(MOTOR_L, 50);
  LinerBot.setMotorSpeed(MOTOR_R, 0);
  delay(200);
  
  // Go straight.
  LinerBot.setMotorSpeed(MOTOR_L, 80);
  LinerBot.setMotorSpeed(MOTOR_R, 80);
  delay(500);
  
  // Turn left until opponent is detected.
  LinerBot.setMotorSpeed(MOTOR_L, 0);
  LinerBot.setMotorSpeed(MOTOR_R, 50);
  
  unsigned long startTimestamp = millis();
  while (digitalRead(OPP_FC)) {
    // Quit if opponent is not found after timeout.
    if (millis() - startTimestamp > 1000) {
      break;
    }
  }
}



/*******************************************************************************
 * Search
 *******************************************************************************/
void search(int dir) {
  // Move in circular motion.
  if (dir == LEFT) {
    LinerBot.setMotorSpeed(MOTOR_L, 60);
    LinerBot.setMotorSpeed(MOTOR_R, 80);
  } else {
    LinerBot.setMotorSpeed(MOTOR_L, 80);
    LinerBot.setMotorSpeed(MOTOR_R, 60);
  }
}



/*******************************************************************************
 * Attack
 * Track and attack the opponent in full speed.
 * Do nothing if opponent is not found.
 *******************************************************************************/
void attack() {
  // Opponent in front center.
  // Go straight in full speed.
  if (digitalRead(OPP_FC) == LOW) {
    LinerBot.setMotorSpeed(MOTOR_L, 255);
    LinerBot.setMotorSpeed(MOTOR_R, 255);
  }

  // Opponent in front left.
  // Turn left.
  else if (digitalRead(OPP_FL) == LOW) {
    LinerBot.setMotorSpeed(MOTOR_L, 0);
    LinerBot.setMotorSpeed(MOTOR_R, 255);
  }

  // Opponent in front right.
  // Turn right.
  else if (digitalRead(OPP_FR) == LOW) {
    LinerBot.setMotorSpeed(MOTOR_L, 255);
    LinerBot.setMotorSpeed(MOTOR_R, 0);
  }

  // Opponent in left.
  // Turn left.
  else if (digitalRead(OPP_L) == LOW) {
    LinerBot.setMotorSpeed(MOTOR_L, -100);
    LinerBot.setMotorSpeed(MOTOR_R, 100);
    delay(300);
  }

  // Opponent in right.
  // Turn right.
  else if (digitalRead(OPP_R) == LOW) {
    LinerBot.setMotorSpeed(MOTOR_L, 100);
    LinerBot.setMotorSpeed(MOTOR_R, -100);
    delay(300);
  }
}



/*******************************************************************************
 * Back Off
 * This function should be called when the ring edge is detected.
 *******************************************************************************/
void backoff(int dir) {
  // Reverse
  LinerBot.setMotorSpeed(MOTOR_L, -50);
  LinerBot.setMotorSpeed(MOTOR_R, -50);
  delay(300);

  // Rotate..
  if (dir == LEFT) {
    // Rotate left backward.
    LinerBot.setMotorSpeed(MOTOR_L, -50);
    LinerBot.setMotorSpeed(MOTOR_R, 50);
  } else {
    // Rotate right backward.
    LinerBot.setMotorSpeed(MOTOR_L, 50);
    LinerBot.setMotorSpeed(MOTOR_R, -50);
  }
  delay(600);

  // Stop the motors.
  LinerBot.setMotorSpeed(MOTOR_L, 0);
  LinerBot.setMotorSpeed(MOTOR_R, 0);
  delay(50);
}
