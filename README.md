# ATmega328P Projects with External Crystal

This repository contains a collection of projects developed using the ATmega328P microcontroller with an external crystal oscillator. Each project showcases different functionalities and applications, focusing on embedded systems, microcontroller programming, and interfacing with various peripherals.

## Table of Contents

- [Overview](#overview)
- [Project Descriptions](#project-descriptions)
- [Setup and Usage](#setup-and-usage)
- [Hardware Requirements](#hardware-requirements)
- [License](#license)

## Overview

The ATmega328P is a versatile microcontroller used in many embedded systems and DIY electronics projects. This repository focuses on standalone projects using the ATmega328P with an external crystal oscillator to improve clock accuracy and stability.

### Why Use an External Crystal?

An external crystal oscillator provides a more stable and accurate clock signal compared to the internal oscillator. This is particularly important for timing-sensitive applications like real-time data processing, communication protocols, and precise PWM signal generation.

## Project Descriptions

The following is a list of projects included in this repository. Each project is represented by its corresponding `.ini` and source files on the main page:

- **Binary Watch**: Displays time in binary format using 7 LEDs. The time is set and adjusted using buttons.
- **Brick Game**: A classic brick-breaking game controlled by potentiometers and buttons, displayed on an SSD1306 OLED screen.
- **Neural Network Simulation**: Simulates a simple neural network with interactions controlled by external inputs, visualized on the OLED and LEDs.
- **Universal Remote**: Controls various devices using IR signals, programmed through the ATmega328P.

*(Add descriptions for additional projects as necessary)*

## Setup and Usage

### Prerequisites

- **AVR-GCC**: Compiler for AVR microcontrollers.
- **AVRDUDE**: Utility to upload compiled code to the ATmega328P.
- **Make**: Optional, for automated builds.
- **External Crystal**: Typically a 16 MHz crystal with accompanying capacitors.

### Compilation and Upload

1. Clone the repository:
   ```bash
   git clone https://github.com/butasi/atmega328p-projects.git
   cd atmega328p-projects
