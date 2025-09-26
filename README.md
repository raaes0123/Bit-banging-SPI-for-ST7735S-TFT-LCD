## ST7735S TFT LCD Driver
This repository contains a software-based SPI (bit-banging) implementation to drive an ST7735S TFT LCD using GPIO pins on an STM32 microcontroller

## Building and Flashing
Build and flash the microcontroller. It uses stm32flash which is a small open-source command-line utility for flashing STM32 microcontrollers over the built-in UART bootloader. <br>
<pre> make build </pre>

## Starting the debugger
Inside the port directory in a new terminal run <br>
<pre> make gdb </pre>