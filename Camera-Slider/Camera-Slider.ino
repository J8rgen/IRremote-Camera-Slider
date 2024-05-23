#include "IRremote.h"

#define IR_RECEIVE_PIN 9 
#define ENDSTOP_PIN 10  

const int stepPin = 7; 
const int dirPin = 6;  
const int enPin = 13;  //not used

const int maxPosition = 1000; // Maximum position limit

bool direction = true;      // true for clockwise, false for counterclockwise
bool motorEnabled = false;  // flag to control motor movement, initially off
int motorSpeed = 6000;      // default speed, Bigger = slower (in microseconds)
int homePosition = 0;       // variable to store current position (0 is home)

// LCD screen setup
#include <LiquidCrystal.h>
// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Adjusted pin numbers

// PRINTING
int printCounter = 0; // Counter for printing while moving
int screenUpdateCounter = 0; //update while still
const int SCREEN_UPDATE_THRESHOLD = 50;


void setup() {
  Serial.begin(115200); // 9600   115200
  IrReceiver.begin(IR_RECEIVE_PIN);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
  digitalWrite(enPin, LOW);

  // Initialize the LCD screen
  lcd.begin(16, 2);
  lcd.print("hello, world!");

  // endstop Setting home position
  if (digitalRead(ENDSTOP_PIN) == HIGH) {
    // If endstop is not triggered, move the motor counterclockwise
    direction = false;
    setDirection();
    motorEnabled = true;
    while (digitalRead(ENDSTOP_PIN) == HIGH) {
      moveMotor();
    }
  }
  // Handle endstop condition
  handleEndstopTriggered();
  delay(100);
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
    printCounter++;
    if (printCounter >= SCREEN_UPDATE_THRESHOLD) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Position: ");
      lcd.print(homePosition);
      printCounter = 0; // Reset the counter
    }

    // Check endstop status
    if (digitalRead(ENDSTOP_PIN) == LOW) {
      handleEndstopTriggered();
      delay(100);
      return;
    }

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
  while (millis() - startTime < 500) { // Move for 0.5 sec (or something like that idk)
    moveMotor(); // Call the function to handle motor movement
    // Print current position
    //Serial.println(homePosition);
  }

  motorEnabled = false;
  moveMotor(); // Call the function to handle motor movement
  delay(100);

  // Save home position
  homePosition = 0;
}


void moveMotorEndlessly() {
  int targetPosition = 100;
  bool EDirection = true; // Flag to indicate the direction of movement (true for 100 to 1000, false for 1000 to 100)
  
  while (true) {
    if (EDirection) {
      targetPosition = 1000;
      direction = true; // Clockwise
    } else {
      targetPosition = 100;
      direction = false; // Counterclockwise
    }
    setDirection(); // Call the function to handle direction
    motorEnabled = true;

    // Move the motor until it reaches the target position
    while (homePosition != targetPosition && (direction ? homePosition < targetPosition : homePosition > targetPosition)) {
      moveMotor(); // Call the function to handle motor movement

      //check for any new IR commands
      if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.command != 25) {
          // Stop the motor
          motorEnabled = false;
          // Resume IR receiver
          IrReceiver.resume();
          // Exit the function
          return;
        }
        IrReceiver.resume(); // without this won't read while going up
      }

      // Check endstop status
      if (digitalRead(ENDSTOP_PIN) == LOW) {
        handleEndstopTriggered();
        delay(100);
        return;
      }

      // Print current position
      Serial.print("Current Position: ");
      Serial.println(homePosition);
    }

    IrReceiver.resume(); // without this won't read while going down?
    EDirection = !EDirection; // Toggle the direction
  }
}


void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command);
    switch (IrReceiver.decodedIRData.command) {
      case 67: //enne 70
        direction = true; // Clockwise
        motorEnabled = true;
        break;
      case 68: //enne 21
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
      case 25: // Move back and forth between positions 100 and 1000 endlessly
        moveMotorEndlessly();
        break;
      default:
        motorEnabled = false;
        break;
    }
    IrReceiver.resume();
  }

  // needed for continuous movement
  moveMotor(); // Call the function to handle motor movement
  setDirection(); // Call the function to handle direction

  // Check endstop status
  if (digitalRead(ENDSTOP_PIN) == LOW) {
    handleEndstopTriggered();
    delay(100);
  }

  //PRINTING, screen
  screenUpdateCounter++; 
  // Check if it's time to update the screen
  if (screenUpdateCounter >= SCREEN_UPDATE_THRESHOLD && !motorEnabled) {
    // Print current position to LCD screen
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Position: ");
    lcd.print(homePosition);
    // Reset the screen update counter
    screenUpdateCounter = 0;
    printCounter = 0; // while moving print counter also to 0
  }
  // Print current position to serial monitor
  Serial.print("Current Position: ");
  Serial.println(homePosition);
}
