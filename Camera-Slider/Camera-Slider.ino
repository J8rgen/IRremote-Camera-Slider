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

void moveMotorToPosition(int targetPosition) {
  // Move the motor in the appropriate direction
  if (homePosition < targetPosition) {
    direction = true; // Clockwise
  } else {
    direction = false; // Counterclockwise
  }
  setDirection(); // Call the function to handle direction
  
  motorEnabled = true;
  // Move the motor until it reaches the target position
  while (homePosition != targetPosition && (direction ? homePosition < targetPosition : homePosition > targetPosition)) {
    moveMotor(); // Call the function to handle motor movement
    // Print current position
    Serial.print("Current Position: ");
    Serial.println(homePosition);
  }
  motorEnabled = false; // Stop the motor
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
    // Print current position
    Serial.print("Current Position: ");
    Serial.println(homePosition);
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
      case 12: // Move to position 100
        moveMotorToPosition(100);
        break;
      case 24: // Move to position 200
        moveMotorToPosition(200);
        break;
      case 94: // Move to position 300
        moveMotorToPosition(300);
        break;
      case 8: // Move to position 400
        moveMotorToPosition(400);
        break;
      case 28: // Move to position 500
        moveMotorToPosition(500);
        break;
      case 90: // Move to position 600
        moveMotorToPosition(600);
        break;
      case 66: // Move to position 700
        moveMotorToPosition(700);
        break;
      case 82: // Move to position 800
        moveMotorToPosition(800);
        break;
      case 74: // Move to position 900
        moveMotorToPosition(900);
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
