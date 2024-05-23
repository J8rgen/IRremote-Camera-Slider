#include "IRremote.h"

#define IR_RECEIVE_PIN 7
#define ENDSTOP_PIN 4  
const int stepPin = 3;
const int dirPin = 2;
const int enPin = 8;

bool direction = true;      // true for clockwise, false for counterclockwise
bool motorEnabled = false;  // flag to control motor movement, initially off
int motorSpeed = 3000;      // default speed, Bigger = slower (in microseconds)

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
    digitalWrite(stepPin, HIGH); // Step high
    delayMicroseconds(motorSpeed); // Delay
    digitalWrite(stepPin, LOW); // Step low
    delayMicroseconds(motorSpeed); // Delay
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

  // Check endstop status
  if (digitalRead(ENDSTOP_PIN) == LOW) {
    handleEndstopTriggered();
    delay(100);
  }
}
