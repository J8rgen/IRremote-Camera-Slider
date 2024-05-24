# Stepper Motor Control with IR Remote and LCD Display

This project controls a stepper motor using an IR remote, an endstop sensor, and an LCD display. The motor can move to specified positions, respond to remote commands, and display its current position on the LCD screen.

## Features

- IR Remote Control: Control the motor's movements using an IR remote.
- Endstop Sensor: Automatically sets the home position using an endstop sensor.
- LCD Display: Displays the current position of the motor.
- Position Control: Move the motor to specific positions or continuously between two points.

## Hardware Requirements

- Arduino board (e.g., Arduino Uno)
- Stepper motor
- Stepper motor driver (e.g., DRV8825)
- IR receiver module (e.g., TSOP4838)
- Endstop switch
- 16x2 LCD display
- Connecting wires and a breadboard
- 10k ohm potentiometer
- 220 ohm resistor
- 100uF capacitor
- IR remote

## Pin Configuration

- IR Receiver Pin: 9
- Endstop Pin: 10
- Stepper Motor Pins:
  - stepPin: 7
  - dirPin: 6
  - enPin: 13 (currently not used)
- LCD Pins: 12, 11, 5, 4, 3, 2

## Useful 3D Prints

- [Arduino Uno + Half Size Breadboard Holder](https://www.thingiverse.com/thing:63900)
- [Pulley Mount](https://www.thingiverse.com/thing:1225670)
- [Linear Rail Shaft 8mm](https://www.thingiverse.com/thing:521740)
- [NEMA 17 Stepper Mount](https://www.thingiverse.com/thing:5391)
- [Adjustable Y Belt Holder for GT2](https://www.thingiverse.com/thing:745934)
- [LM8UU Holder](https://www.thingiverse.com/thing:104508)
- [Linear Bushing](https://www.thingiverse.com/thing:2202854)
- [Belt Tensioner](https://www.thingiverse.com/thing:10082)
- [EndStop MicroSwitch](https://www.thingiverse.com/thing:6004532)

![slider sketch_bb](https://github.com/J8rgen/IRremote-Camera-Slider/assets/92487999/8f815d2a-00be-4bf4-9e0c-52539b97b2b1)
