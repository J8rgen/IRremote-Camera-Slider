#include "IRremote.h"

#define IR_RECEIVE_PIN 7
#define ENDSTOP_PIN 4

//screen setup--
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;
//---

const int stepPin = 3;
const int dirPin = 2;
const int enPin = 8;
const int maxPosition = 1000; // Maximum position limit

bool direction = true;      // true for clockwise, false for counterclockwise
bool motorEnabled = false;  // flag to control motor movement, initially off
int motorSpeed = 3000;      // default speed, Bigger = slower (in microseconds)
int homePosition = 0;       // variable to store the home position

//PRINTING
//int printCounter = 0; // Counter for printing
int screenUpdateCounter = 0;
const int SCREEN_UPDATE_THRESHOLD = 150;


void setup() {
  Serial.begin(9600); // 9600   115200
  IrReceiver.begin(IR_RECEIVE_PIN);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
  digitalWrite(enPin, LOW);

  //screen setup:
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  // Set I2C clock speed
  oled.setI2cClock(400000); // Set I2C clock speed to 400 kHz


  oled.setFont(System5x7);
  oled.clear();
  oled.print("Hello");
  //---

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

    //printCounter++;
    //if (printCounter >= 50) {
    //  oled.clear();
    //  //oled.print("CURRENT POSITION: ");
    //  //oled.println(homePosition);
    //  oled.println("CURRENT POSITION: " + String(homePosition));
    //  printCounter = 0; // Reset the counter
    //}
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
  while (millis() - startTime < 500) { // Move for 0.5 sec
    moveMotor(); // Call the function to handle motor movement
    // Print current position
    Serial.println(homePosition);
  }

  motorEnabled = false;
  moveMotor(); // Call the function to handle motor movement
  delay(100);

  // Save home position
  homePosition = 0;
}




void moveMotorEndlessly() {
  int targetPosition = 100;
  bool EDirection = true; // Flag to indicate the direction of movement (true for 100 to 900, false for 900 to 100)

  // Move the motor in the appropriate direction
  if (homePosition < targetPosition) {
    direction = true; // Clockwise
  } else {
    direction = false; // Counterclockwise
  }
  setDirection(); // Call the function to handle direction

  motorEnabled = true;



  while (true) {
    if (EDirection) {
      targetPosition = 900;
      direction = true; // Clockwise
    } else {
      targetPosition = 100;
      direction = false; // Counterclockwise
    }
    setDirection(); // Call the function to handle direction

    // Move the motor until it reaches the target position
    while (homePosition != targetPosition && (direction ? homePosition < targetPosition : homePosition > targetPosition)) {
      // Check for IR signal during motor movement
      moveMotor(); // Call the function to handle motor movement
      if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.command != 64) {
          // Stop the motor
          motorEnabled = false;
          // Resume IR receiver
          IrReceiver.resume();
          // Exit the function
          return;
        }
        IrReceiver.resume(); // without this wont read while going up
      }


      // Print current position
      Serial.print("Current Position: ");
      Serial.println(homePosition);
    }

    IrReceiver.resume(); // without this wont read while going down?
    EDirection = !EDirection; // Toggle the direction
  }
  //motorEnabled = false; // Stop the motor
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
      case 64: // Move back and forth between positions 100 and 900 endlessly
        moveMotorEndlessly();
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


  // PRINTING
  // Move the motor only if it's not moving
  // Increment screen update counter
  screenUpdateCounter++;
  // Check if it's time to update the screen
  if (screenUpdateCounter >= SCREEN_UPDATE_THRESHOLD && !motorEnabled) {
    oled.clear();
    oled.println("CURRENT POSITION: " + String(homePosition));
    // Reset the screen update counter
    screenUpdateCounter = 0;
  }
  // Print current position to serial monitor
  Serial.print("Current Position: ");
  Serial.println(homePosition);
}
