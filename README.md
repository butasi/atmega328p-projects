# ATmega328P Projects with External Crystal

This repository contains a collection of projects developed using the ATmega328P microcontroller with an external crystal oscillator. Each project in this repository demonstrates various functionalities and applications of the ATmega328P, focusing on embedded systems, microcontroller programming, and interfacing with various peripherals.

## Table of Contents

- [Overview](#overview)
- [Projects](#projects)
- [Setup and Usage](#setup-and-usage)
- [Hardware Requirements](#hardware-requirements)
- [License](#license)

## Overview

The ATmega328P is a popular microcontroller used in many embedded systems and DIY electronics projects. It is the same microcontroller used in the Arduino Uno board but in this repository, the focus is on projects that utilize the ATmega328P in a standalone configuration with an external crystal oscillator.

### Why Use an External Crystal?

Using an external crystal oscillator enhances the accuracy and stability of the ATmega328P’s clock, which is critical in timing-sensitive applications like real-time data processing, communication protocols, and precise PWM signal generation.

## Projects

Each subdirectory in this repository represents a different project. Below are brief descriptions of the projects:

1. **Binary Watch**: A simple binary watch using 7 LEDs to display time in binary format.
2. **Brick Game**: A classic brick-breaking game using an SSD1306 OLED display.
3. **Neural Network Simulation**: A basic neural network simulation that interacts with external inputs like buttons and potentiometers.
4. **Universal Remote**: A project to control various devices using IR signals.

*(Add more project descriptions as needed)*

## Setup and Usage

### Prerequisites

- **AVR-GCC**: The GCC compiler for AVR microcontrollers.
- **AVRDUDE**: A utility to upload code to the ATmega328P.
- **Make**: For automated builds (if using Makefiles).
- **External Crystal**: Typically a 16 MHz crystal with capacitors.

### Compilation and Upload

Each project contains its own source files and (optionally) a Makefile. To compile and upload a project:

1. Navigate to the project directory.
   ```bash
   cd BinaryWatch
