# Virtual Train Project

## Basic Information
- **Institution:** UNCC
- **Course:** Embedded Systems
- **Project:** Lab 3 - Virtual Train

## Project Description
This project simulates a virtual train using a Seven Segment Display (SSD) from the Grove Starter Kit with the MSP-EXP430FR2355. The train moves around the perimeter of the SSD, with its direction and motion controlled using push buttons. The green and red LEDs indicate whether the train is in motion or stopped.

## Materials Used
- **MSP-EXP430FR2355** - TI's MSP430 development board
- **Seeed Studio Grove Seven Segment Display** - Used to display the moving train
- **Grove connection cable**
- **USB micro cable**
- **Laptop (Windows preferable)**

## Skills Showcased
- Implementing a software state machine
- Using enumerations and custom structures
- GPIO manipulation for buttons and LEDs
- Utilizing a custom library for SSD control
- Timer-based event handling
- Debouncing switch inputs

## How to Run
1. Import the required SSD library files (`Four_Digit_Display.h` and `Four_Digit_Display.c`) into your project.
2. Initialize the SSD and configure the GPIO for the buttons and LEDs.
3. Implement the train movement logic:
   - The train is represented by three consecutive segments on the SSD.
   - The train starts in a stopped state.
   - Pressing **SW1** toggles the train's movement.
   - Pressing **SW2** changes the train's direction.
   - The train moves at a rate of one segment per second.
4. Implement the state machine to handle train movement and button interactions.
5. Compile and flash the firmware onto the MSP430.

## Additional Information
More details about this project can be found in the **Lab 3 - Virtual Train** PDF.

## Author
Nathan Culler

Developed as part of an Embedded Systems course at UNCC.
