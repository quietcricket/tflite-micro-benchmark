# Tensorflow Lite Micro Benchmark

## Aim

To test out the performance and power consumption of some ARM based boards.

## SDK and Frameworks

The machine learning framework uses Tensorflow Lite Micro. The RTOS system is Zephyr.

_My knowledge on RTOS is close to zero. Zephyr is chosen because it supports the most number of testing boards_

## Credits

The code used based on Zephyr's sample 'hello_world' tflite micro project.

## TF model

Significant effort was put into trying to use a mnist model for testing. Some bigger models, 100-200kb, managed to work but they couldn't fit into boards with less RAM. Small models either fails silently or contain unsupported TF operations.

In the end, a simpler model is used. The model predicts the y position on a sine wave when given an x value as an input. The model size is 2kb.

## Setting up the dev environment on OSX

Installation for Linux should be very similar. However, setting up the environment on windows can be challenging because the package installer `chocolatey` is not as great as OSX's `homebrew` or Linux's native `apt`.

### Zephyr

You can follow Zephyr getting started page to setup the environment. The key steps are listed here:

- Install west and other python library, use a virtual env if needed.
- Init a Zephyr workspace with `west init`
- :exclamation: tensorflow-lite-micro module is optional for Zephyr. You must run the following code to add it
  `west config manifest.group-filter -- +tflite-micro && west update`
- Download and un-zip Zephyr SDK
- Run setup.sh inside the Zephyr SDK folder to add the SDK to system path
- OpenOCD was not installed for my OSX computer. To install it, use `brew install openocd` for OSX and `sudo apt install openocd` for Linux (a guess, never tested)
- cd into Zephyr workspace `zephyr` folder
- Run `west build -p always -b YOUR-BOARD-NAME samples/basic/blinky` to build the blinky example
- Run `west flash` to upload the executable onto your board
- If there are no errors in build and flash, the environment is read to use

### minicom

- Minicom is used to display serial information sent from the boards
- Use `minicom --device /dev/XXX` to connect the board
- The boards used in this experiment tend to be named in this pattern `/dev/tty.usbmodemXXX`
- To quit minicom, press Esc+Q. The `Meta` key is `Esc` by default

## SDK/IDE tried

Little progress was made with these SDK/IDE. They were abandoned mainly because the concern of the support for multiple boards are not great.

- Microchip Studio
- MPLAB Harmony
- Keil Studio
- Keil Studio Cloud
- PlatformIO
- Arduino

## Boards

- Atmel SAMD21 PLAINED PRO: atsamd21_xpro
	- `west build -b atsamd21_xpro tflite-micro-benchmark -p`
- Atmel ATSAMV71-XULT: sam_v71_xult
	- `west build -b sam_v71_xult tflite-micro-benchmark -p`
- nRF52840 DK: nrf52840dk_nrf52840
	- `west build -b nrf52840dk_nrf52840 tflite-micro-benchmark -p`
- TI Tiva C: M4F 
	- am62x_m4_sk didn't work
- ST: STEVAL-WESU
	- Needs connector for serial connection


## Results
|Board|ms per 1000 inferences|
|-|-|
|Atmel SAMD21 PLAINED PRO			|535	|
|nRF52840 DK									|208	|
|Atmel ATSAMV71-XULT 					|28		|	