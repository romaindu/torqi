# Torqi

Torqi is a force feedback wheel actuator for racing simulators.
It is designed around a NEMA 34 sized stepper motor and encoder.

## Table of Contents

- [About](#about)
    - [History](#history)
    - [Mindset](#mindset)
    - [Details](#details)
    - [Some pictures](#some-pictures)
- [Content](#content)
    - [Availability](#availability)
- [License](#license)

## About

### History

Many attempts have been made by the author over the years (since 2016) to build a direct drive force feedback wheel that does not use expensive BLAC motors. The first one was based on a car alternator, but it took too many amps to get a decent torque. Has also been tried a magnetic spring, a washing machine universal motor, a pulley driven encoder on a stepper... Not much to be proud of...

The stepper motor was a cheap way to get much torque, but it does requires a decent encoder to minimise cogging and get consistent torque. Fortunately, we can find so-called *"closed-loop steppers"* with a rear mounted 1000CPR encoder.

All that remained was to design the control electronics.

### Mindset

The main idea is to have a PCB compatible with hand-assembly, easily sourced parts, cheap debugger... For the sake of simplicity (it is already quite complicated for a DIY project), it only does USB&Power to Torque conversion. Any idea for a wheel with buttons and LEDs is left to another project.

### Details

- **MCU:** ATSAMD21 ARM Cortex M0 @48MHz
- **MOTOR:** 6 A/phase NEMA34 (up to 8.5 N.m depending on motor), 1000CPR encoder
- **BRAKING:** 10W constant, 100W peak
- **USB:** Full-speed, 1kHz refresh rate, plug'n'play (Windows HID drivers)
- **GAMES:** Tested on Assetto Corsa, ACC, Automobilista 1&2, Race07, R3E

### Some pictures

*Prototype (top-left) and final design of the stepper based solution.*

![Torqi Force Feedback Wheel](https://drive.google.com/uc?export=view&id=1NK0lfnQvTbe-NgocOcBJjiW1Odlj7CQM)

## Content

### Availability

As of today this repo is limited to the firmware. Electronics, BOM, mechanical hardware, and other stuff might be released in the future either as a kit or as sources, depending on people's enthusiasm for the project.

## License

[Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0)
