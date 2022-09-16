#include <EEPROM.h>
#include "LinerBot.h"



void KKBPJLinerBot::begin(void)
{
  // Setting pin direction.
  pinMode(PWM_L, OUTPUT);
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
  
  pinMode(LED, OUTPUT);
//  pinMode(SERVO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(START, INPUT_PULLUP);
  
  // Clear all ouputs.
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 0);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, LOW);
  
  digitalWrite(LED, LOW);
//  digitalWrite(SERVO, LOW);
  digitalWrite(BUZZER, LOW);
  
  // Attach servo.
//  _servo.attach(SERVO);
}


/*
int KKBPJLinerBot::readDipSwitch(void)
{
  int adc = analogRead(MODE);
  if (adc > 933) return 0;
  if (adc > 773) return 1;
  if (adc > 658) return 2;
  if (adc > 563) return 3;
  if (adc > 487) return 4;
  if (adc > 440) return 5;
  if (adc > 400) return 6;
  return 7;
}
*/


/*
float KKBPJLinerBot::readBatteryVoltage(void)
{
  int adc = analogRead(VBATT);
  float adcVolt = (float)adc * (5.0f / 1023.0f);
  float vBatt = adcVolt * (1.0f + (10.0f / 3.9f));
  return vBatt;
}
*/


void KKBPJLinerBot::stop(void)
{
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 0);
}


void KKBPJLinerBot::moveForward(int speed)
{
  analogWrite(PWM_L, speed);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  analogWrite(PWM_R, speed);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
}


void KKBPJLinerBot::moveBackward(int speed)
{
  analogWrite(PWM_L, speed);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  analogWrite(PWM_R, speed);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
}


void KKBPJLinerBot::turnLeft(int speed)
{
  analogWrite(PWM_L, speed);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  analogWrite(PWM_R, speed);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
}


void KKBPJLinerBot::turnRight(int speed)
{
  analogWrite(PWM_L, speed);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  analogWrite(PWM_R, speed);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
}



void KKBPJLinerBot::setMotorSpeed(int side, int speed)
{
  int direction_1;
  int direction_2;
  if (speed >= 0) {
    direction_1 = LOW;
    direction_2 = HIGH;
  }
  else {
    speed = -speed;
    direction_1 = HIGH;
    direction_2 = LOW;
  }
  
  switch (side) {
    case MOTOR_L:
      analogWrite(PWM_L, speed);
      digitalWrite(L_IN1, direction_1);
      digitalWrite(L_IN2, direction_2);
      break;
      
    case MOTOR_R:
      analogWrite(PWM_R, speed);
      digitalWrite(R_IN1, direction_1);
      digitalWrite(R_IN2, direction_2);
      break;
  }
}



/*
void KKBPJLinerBot::setServoPosition(int position)
{
  _servo.write(position);
}
*/


void KKBPJLinerBot::playTone(int pitch, int duration)
{
  if (pitch == 0) {
    delay(duration);
    return;
  }
  
  long halfPeriod = 1000000L / (long)pitch / 2L;
  for (long i = 0; i < (long)duration * 1000L; i += halfPeriod * 2L) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(halfPeriod);
  }
}


void KKBPJLinerBot::playMelody(const int *pitch, const int *duration, int length)
{
  for (int i = 0; i < length; i++) {
    // To calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000 / 8, etc.
    int noteDuration = 1000 / duration[i];
    playTone(pitch[i], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well.
    int pauseBetweenNotes = noteDuration * 0.30;
    delay(pauseBetweenNotes);
  }
}



void KKBPJLinerBot::calibrateEdgeSensors(void)
{
  int minL = 1024;
  int minR = 1024;
  int maxL = 0;
  int maxR = 0;
  
  int timestamp = millis();
  
  // Loop until start button is pressed.
  do {
    // Get the minimum and maximum value.
    int tempL = analogRead(EDGE_L);
    int tempR = analogRead(EDGE_R);
    
    if (minL > tempL) minL = tempL;
    if (minR > tempR) minR = tempR;
    
    if (maxL < tempL) maxL = tempL;
    if (maxR < tempR) maxR = tempR;
      
    // Blink LED.
    if (millis() - timestamp > 100) {
      timestamp +=  100;
      digitalWrite(LED, !digitalRead(LED));
    }
  } while (digitalRead(START) == HIGH);
  while (digitalRead(START) == LOW);
  digitalWrite(LED, LOW);
  
  // Calculate and save the threshold to EEPROM.
  if (maxL > minL) {
    int threshold = ((maxL - minL) * 3 / 5) + minL;
    EEPROM.put(EEADD_EDGE_L, threshold);
  }
  
  if (maxR > minR) {
    int threshold = ((maxR - minR) * 3 / 5) + minR;
    EEPROM.put(EEADD_EDGE_R, threshold);
  }
}




int KKBPJLinerBot::readEdgeSensorThreshold(int side)
{
  int eepromAddress;
  if (side == EDGE_L) {
    eepromAddress = EEADD_EDGE_L;
  }
  else if (side == EDGE_R) {
    eepromAddress = EEADD_EDGE_R;
  }
  else {
    return 0;
  }
  
  int threshold;
  EEPROM.get(eepromAddress, threshold);
  
  // Use default value if the value in EEPROM is invalid.
  if ((threshold <= 0) || (threshold >= 1023)) {
    threshold = DEFAULT_EDGE_THRESHOLD;
  }
  
  return threshold;
}




bool KKBPJLinerBot::isEdgeDetected(int side)
{
  if (analogRead(side) < readEdgeSensorThreshold(side)) {
    return true;
  }
  else {
    return false;
  }
}




// Static instance.
KKBPJLinerBot LinerBot;
