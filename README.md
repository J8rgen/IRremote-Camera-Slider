Stepper Motor Control with IR Remote and LCD Display

This project controls a stepper motor using an IR remote, an endstop sensor, and an LCD display. The motor can move to specified positions, respond to remote commands, and display its current position on the LCD screen.

Features

    IR Remote Control: Control the motor's movements using an IR remote.
    Endstop Sensor: Automatically sets the home position using an endstop sensor.
    LCD Display: Displays the current position of the motor.
    Position Control: Move the motor to specific positions or continuously between two points.

Hardware Requirements

    Arduino board (e.g., Arduino Uno)
    Stepper motor and driver
    IR remote and receiver module
    Endstop switch
    16x2 LCD display
    Connecting wires and a breadboard

Pin Configuration

    IR Receiver Pin: 9
    Endstop Pin: 10
    Stepper Motor Pins:
        stepPin: 7
        dirPin: 6
        enPin: 13 (currently not used)
    LCD Pins: 12, 11, 5, 4, 3, 2
