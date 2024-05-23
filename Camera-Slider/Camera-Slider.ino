#include "IRremote.h"

#define IR_RECEIVE_PIN 7
#define ENDSTOP_PIN 4  // Pin connected to the common (COM) terminal of the endstop switch
const int stepPin = 3;
const int dirPin = 2;
const int enPin = 8;

// Motor driver interface type
#define motorInterfaceType 1

bool direction = true;      // true for clockwise, false for counterclockwise
bool motorEnabled = false;   // flag to control motor movement, initially off
int motorSpeed = 2000;      // default speed, adjust as needed (in microseconds)

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  // Initialize IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN);

  // Set pin modes for motor control
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  // Set pin mode for endstop
  pinMode(ENDSTOP_PIN, INPUT_PULLUP); // Using internal pull-up resistor

  // Disable motor driver initially
  digitalWrite(enPin, LOW);
}

void loop() {
  // Check if IR signal is received
  if (IrReceiver.decode()) {
    // Print decoded IR command
    Serial.println(IrReceiver.decodedIRData.command);

    // Switch case based on IR command
    switch (IrReceiver.decodedIRData.command) {
      case 70:
        // Change direction to clockwise
        direction = true;
        // Enable motor movement
        motorEnabled = true;
        break;

      case 21:
        // Change direction to counterclockwise
        direction = false;
        // Enable motor movement
        motorEnabled = true;
        break;

      // Add more cases for other IR commands if needed

      default:
        // Disable motor movement for unknown IR commands
        motorEnabled = false;
        break;
    }
    // Resume IR receiver for the next IR signal
    IrReceiver.resume();
  }

  // Check if motor movement is enabled
  if (motorEnabled) {
    // Check if endstop is not triggered
    if (digitalRead(ENDSTOP_PIN) == HIGH) {
      // Rotate motor based on current direction
      if (direction) {
        digitalWrite(dirPin, HIGH); // Set direction clockwise
      } else {
        digitalWrite(dirPin, LOW); // Set direction counterclockwise
      }
      digitalWrite(stepPin, HIGH); // Step high
      delayMicroseconds(motorSpeed);      // Delay
      digitalWrite(stepPin, LOW);  // Step low
      delayMicroseconds(motorSpeed);      // Delay
    } else {
      // Disable motor movement if endstop is triggered
      motorEnabled = false;
    }
  } else {
    // Disable motor movement
    digitalWrite(stepPin, LOW);
  }
}
