# LED Flow Program

## Description

This project demonstrates a basic LED flow control with GPIO on an embedded system. The program uses 8 LEDs to simulate a "shift" or "flow" effect, where an LED lights up in sequence, and the direction can be reversed with a button press. The program continuously shifts the lit LED position along the series, creating a visually engaging flow pattern.

## Features
- **LED Shift Effect**: LEDs light up one at a time in sequence, creating a smooth shifting effect.
- **Button-Controlled Direction**: A button press changes the direction of the flow, toggling between left-to-right and right-to-left shifts.
- **Simple Wrapping Logic**: The code wraps the LED position when it reaches the ends, ensuring continuous flow.

## Software

### Dependencies
This code is designed for use with the [Analog Devices MSDK](https://analogdevicesinc.github.io/msdk/USERGUIDE/) and requires the following libraries:
- **gpio.h** for GPIO functionality.
- **mxc_delay.h** for delay handling.
- **mxc_device.h** for device-specific definitions.

### Project Setup and Usage

Refer to the [MSDK User Guide](https://analogdevicesinc.github.io/msdk/USERGUIDE/) for general instructions on building, flashing, and debugging.

### Code Overview
- **GPIO Configuration**: Sets up 8 GPIO pins for LEDs and 1 GPIO pin for a button.
- **Direction Control**: Checks the button's state to control LED shifting direction.
- **LED Update**: Updates the LED state every second, creating a 1Hz shifting effect.

## Required Connections

If using the MAX32690EVKIT:
1. Connect a USB cable between the PC and the CN2 (USB/PWR) connector.
2. Install JP7 (RX_EN) and JP8 (TX_EN) headers.
3. Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.
4. Close jumper JP5 (LED1 EN) and JP6 (LED2 EN) headers.

If using the MAX32690FTHR:
1. Connect a USB cable between the PC and the J5 (USB/PWR) connector.
2. Open a terminal application and connect to the device's console UART at 115200, 8-N-1.

## Expected Behavior

Once flashed, you will observe:
- A single LED lit at any time, moving across the 8 LEDs in a flow pattern.
- The flow direction reversing with each button press.
- Terminal output in the console, with debug messages for configuration status.

## Code Structure

- **configure_gpio_pins()**: Initializes GPIO configurations for LEDs and button.
- **set_led()**: Controls each LED individually.
- **clear_all_leds()**: Ensures all LEDs are turned off between shifts.
- **is_button_pressed()**: Checks button state (active low) to determine direction.
- **led_shift()**: Core loop that manages LED shifting based on direction and button input.

## Using ADALM2000 and Scopy for Simulation

In this project, the LED flow effect was simulated using the ADALM2000 device alongside Scopy software. Here’s how you can set it up:

1. **ADALM2000 Setup**:
   - Connect ADALM2000 to the PC via USB.
   - Assign output channels in Scopy to represent each LED. For example, you can assign GPIO outputs from the microcontroller to channels on ADALM2000, simulating the on/off state of each LED.

2. **Scopy Visualization**:
   - Using Scopy’s digital I/O functionality, observe the LED flow effect in real-time. Each “LED” output in the program will light up in sequence on Scopy, mimicking the physical LED behavior.

This setup provides a flexible way to test and observe GPIO-based LED flow patterns without needing physical LEDs, making it ideal for simulations and testing.

Enjoy experimenting with the LED flow simulation using ADALM2000 and Scopy!
