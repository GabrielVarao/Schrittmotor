
# 01_SchrittMot

![AVR](https://img.shields.io/badge/Platform-AVR-blue)
![Microcontroller](https://img.shields.io/badge/MCU-ATmega2560-green)
![Language](https://img.shields.io/badge/Language-C-orange)
![Build](https://img.shields.io/badge/Build-Microchip%20Studio-red)
![Status](https://img.shields.io/badge/Status-Stable-success)

A lightweight **stepper motor controller implementation** for the **ATmega2560**.
The firmware provides runtime control over **motor direction and stepping speed**, using a **finite state machine (FSM)** architecture for clarity and extensibility.

The project was developed and tested using a **PICado ATmega2560 development board**, paired with a **stepper motor driver module**.

---

# Table of Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Project Setup](#project-setup)
- [Building and Flashing](#building-and-flashing)
- [System Architecture](#system-architecture)
- [Hardware Setup](#hardware-setup)
- [Stepper Motor Operation](#stepper-motor-operation)
- [Design Decisions](#design-decisions)
- [Validation and Testing](#validation-and-testing)
- [References](#references)

---

# Overview

This project demonstrates a minimal but structured implementation of **stepper motor control firmware**.

The firmware supports:

- Clockwise and anticlockwise rotation
- Dynamic speed adjustment
- Deterministic motor control via a **finite state machine**

The system is intentionally designed to remain **simple and maintainable**, while still demonstrating good embedded software practices.

---

# Requirements

To compile and run this project, the following tools and hardware are required.

## Development Tools

- **Microchip Studio**
  - AVR toolchain
  - XC8 compiler support

## Hardware

- **ATmega2560 development board**
- **Stepper motor driver module**
- Compatible **stepper motor**

## Documentation Tools (Optional)

For editing diagrams and documentation:

- **VS Code**
- **Draw.io extension**

---

# Project Setup

Before building the project, some configuration is required.

## 1. Install AVRdude

Download and place the AVRdude tool from:

https://github.com/ml3rc/01_SchrittMot/tree/main/readme/AVRdude

into:

C:\Program Files (x86)\Atmel\Studio\7.0\tools\

Adjust the directory if your Microchip Studio version differs.

---

## 2. Configure Programming Tool

Inside Microchip Studio:

Project → 01_SchrittMot Properties

Navigate to:

Tool → Custom programming tool → Command

Update the following parameters:

- Repository path
- COM port used by the development board

---

# Building and Flashing

Once the environment has been configured, the project can be built and uploaded directly from **Microchip Studio**.

Use the option:

Start Without Debugging

This will:

1. Compile the firmware
2. Flash it onto the ATmega2560
3. Start execution immediately

---

# System Architecture

The control logic is implemented as a **Finite State Machine (FSM)**.

This approach provides:

- Predictable execution flow
- Clear system states
- Easy extensibility for future features

For example, introducing an additional mode such as `IDLE` or `PAUSE` would only require defining a new state and transition.

## State Event Diagram

![Diagram-State-Event.drawio.svg](./readme/State-Event.svg)

---

# Hardware Setup

The firmware was tested using the following hardware stack:

- PICado ATmega2560 development board
- Breakout board
- Stepper motor driver module
- Stepper motor

## Hardware Connection Diagram

![Diagram-HW-Connection.drawio.svg](./readme/HW-Connection.svg)

The development board provides a **5V supply voltage**, and the driver circuitry required for the stepper motor is integrated on the board.

---

# Stepper Motor Operation

The stepper motor used in this project is illustrated below.

![IMG-Stepper-Motor](https://plc247.com/wp-content/uploads/2020/07/variable-reluctance-stepper.jpg)

## Operating Parameters

| Parameter | Value |
|-----------|------|
| Default step rate | 20 steps/sec |
| Maximum step rate | 300 steps/sec |
| Supply voltage | 5V |

---

## Original Step Sequence

The original task specification defined the following step sequence:

```c
const uint8_t STEPS[] = {
    0b00001001,
    0b00000011,
    0b00000110,
    0b00001100
};
```

This sequence only utilizes **half of the motor’s possible stepping positions**.

---

## Improved Step Sequence

To achieve smoother rotation and increased resolution, an **extended half-step sequence** was implemented:

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

This method activates **intermediate coil states**, resulting in:

- smoother motion
- finer positioning
- improved mechanical behavior

---

## Stepper Motor Visualization

The animation below demonstrates the internal working principle of a stepper motor.

Note that intermediate states where **two coils are energized simultaneously** are not represented in the animation.

![GIF-Inner-Working-Stepper-Motor](https://www.amci.com/index.php/download_file/view_inline/2441/)

---

# Design Decisions

Several architectural decisions were made to keep the implementation clean and understandable.

## Finite State Machine

The FSM structure simplifies:

- system reasoning
- debugging
- future feature expansion

## No Interrupt Usage

Interrupts were intentionally avoided in this implementation.

The application logic is straightforward and deterministic, so introducing interrupt-based control would unnecessarily complicate the design.

## Adjustable Speed Control

Speed adjustment was included as an additional feature because:

- it requires minimal additional complexity
- it significantly improves system usability

---

# Validation and Testing

All functionality was verified on the hardware configuration described above.

| Step | Action | Expected Result | Result | Pass/Fail |
|:-:|:-:|:-:|:-:|:-:|
| 0 | Power on | Motor rotates clockwise at 20 steps/sec | Motor rotates clockwise at 20 steps/sec | Pass |
| 1 | Press B_DIRECTION_ANTICLOCKWISE | Motor rotates anticlockwise | Motor rotates anticlockwise | Pass |
| 2 | Press B_DIRECTION_CLOCKWISE | Motor rotates clockwise | Motor rotates clockwise | Pass |
| 3 | Press B_SPEEDUP | Motor speed increases | Motor speed increases | Pass |
| 4 | Press B_SPEEDDOWN twice | Motor speed decreases | Motor speed decreases | Pass |

---

# References

All images used in this documentation are linked directly in the README file and reference their original sources when viewed in raw format.
