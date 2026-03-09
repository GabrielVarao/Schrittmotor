# 01_SchrittMot

Simple Stepmotor controller with speed and direction change.

## Requirements

- Microchip Studio with support for AVR and XC8
- ATmega 2560 with 
controller for Stepmotor

## Initial Setup

1. Put the [AVRdude](https://github.com/ml3rc/01_SchrittMot/tree/main/readme/AVRdude) tool into `C:\Program Files (x86)\Atmel\Studio\7.0\tools\` (correct the version number)

2. Correct the Paths and COM-port in `Microchip Studio > Project > 01_SchrittMot Properties... > Tool > Custom programming tool > Command` to match your repo location.

## Building

After the Initial Setup you can build the project with `Start without debugging` (green start sign with a hole in the middle)

## State Event

![Diagram-State-Event.drawio.svg](./readme/State-Event.drawio.svg)

## Connection Diagramm

The following connection diagramm is for a PICado ATmega 2560 board with a corresponding break-out-board and a stepper motor module containing a driver.

![Diagram-HW-Connection.drawio.svg](./readme/HW-Connection.drawio.svg)

## Step Motor

The stepper motor used for this project has following schematic:

![IMG-Stepper-Motor](https://plc247.com/wp-content/uploads/2020/07/variable-reluctance-stepper.jpg)

The Vcc on the dev board is 5V, the dev board also contains the driver.

The maximal step rate is 300 steps per second. The default step rate is specified to be 20 steps per second. The Task specified following sequence for driving the motor:

```c
const uint8_t STEPS[] = {
		0b00001001,
		0b00000011,
		0b00000110,
		0b00001100
};
```

But this way of driving the motor would miss half of the steps available so the sequence used is the following:

```c
const uint8_t STEPS[] = {
		0b00001001,
		0b00000001,
		0b00000011,
		0b00000010,
		0b00000110,
		0b00000100,
		0b00001100,
		0b00001000
};
```

The following gif represents the inner working of the stepper motor, note the the inbetween-steps where two coils are on are not shown...:

![GIF-Inner-Working-Stepper-Motor](https://electricdiylab.com/wp-content/uploads/2019/12/STEPPER-2.gif)

## Why this Solution?

A FSM(finite state machine) was chosen, because of the ease of documentation and implementation. For example when a new mode has to be added named `IDLE` it can just be added as a new state without much work.

The speed change feature was also added, because it's a easy function to add and it provides much more control over the stepper motor.

If there are any questions why the solution was chosen like this, just ask.

## Sources

All sources for images are as links in the README.md when view raw.