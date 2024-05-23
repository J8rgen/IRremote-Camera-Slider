#include "IRremote.h"

#define IR_RECEIVE_PIN 7
#define ENDSTOP_PIN 4  
const int stepPin = 3;
const int dirPin = 2;
const int enPin = 8;
const int maxPosition = 1000; // Maximum position limit

bool direction = true;      // true for clockwise, false for counterclockwise
bool motorEnabled = false;  // flag to control motor movement, initially off
int motorSpeed = 3000;      // default speed, Bigger = slower (in microseconds)
int homePosition = 0;       // variable to store the home position

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ENDSTOP_PIN, INPUT_PULLUP); 

  digitalWrite(enPin, LOW);
}

void moveMotor() {
  if (motorEnabled) {
    // Check for maximum position limit
    if ((direction && homePosition < maxPosition) || (!direction && homePosition > -maxPosition)) {
      digitalWrite(stepPin, HIGH); // Step high
      delayMicroseconds(motorSpeed); // Delay
      digitalWrite(stepPin, LOW); // Step low
      delayMicroseconds(motorSpeed); // Delay
      if (direction) {
        homePosition++; // Increment position if moving clockwise
      } else {
        homePosition--; // Decrement position if moving counterclockwise
      }
    }
  } else {
    digitalWrite(stepPin, LOW); // Disable motor movement
  }
}

void setDirection() {
  // Handle direction
  if (direction) {
    digitalWrite(dirPin, HIGH); // Set direction clockwise
  } else {
    digitalWrite(dirPin, LOW); // Set direction counterclockwise
  }
}

void handleEndstopTriggered() {
  motorEnabled = false;
  moveMotor(); // Call the function to handle motor movement
  delay(100);

  direction = true; // Clockwise
  setDirection(); // Call the function to handle direction

  motorEnabled = true;
  unsigned long startTime = millis(); // Get the current time
  while (millis() - startTime < 1000) { // Move for 1 sec
    moveMotor(); // Call the function to handle motor movement
  }

  motorEnabled = false;
  moveMotor(); // Call the function to handle motor movement
  delay(100);
  
  // Save home position
  homePosition = 0;
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command);
    switch (IrReceiver.decodedIRData.command) {
      case 70:
        direction = true; // Clockwise
        motorEnabled = true;
        break;
      case 21:
        direction = false; // Counterclockwise
        motorEnabled = true;
        break;
      default:
        motorEnabled = false;
        break;
    }
    IrReceiver.resume();
  }

  moveMotor(); // Call the function to handle motor movement
  setDirection(); // Call the function to handle direction

  // Print current position
  Serial.print("Current Position: ");
  Serial.println(homePosition);

  // Check endstop status
  if (digitalRead(ENDSTOP_PIN) == LOW) {
    handleEndstopTriggered();
    delay(100);
  }
}
