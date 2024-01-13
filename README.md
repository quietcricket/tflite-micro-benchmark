# Tensorflow Lite Micro Benchmark

## Aim
To test out the performance and power consumption of some ARM based boards. 

## SDK and Frameworks
The machine learning framework uses Tensorflow Lite Micro. The RTOS system is Zephyr. 

*My knowledge on RTOS is close to zero. Zephyr is chosen because it supports the most number of testing boards* 

## Credits
The code used is mostly from [Taro Yoshino](https://github.com/YoshinoTaro/spresense-arduino-tensorflow/tree/main), with minor modification.

## Setting up the dev environment on OSX
Installation for Linux should be very similar. However, setting up the environment on windows can be challenging because the package installer `chocolatey` is not as great as OSX's `homebrew` or Linux's native `apt`.

You can follow Zephyr getting started page to setup the environment. The key steps are listed here:
* Install west and other python library. *I didn't use virtual env because I find it troublesome to activate/switch env for different projects. So far I've never had any issue sharing the same lib for all my projects.*
* Init a Zephyr workspace with `west init`
* :exclamation: tensorflow-lite-micro module is optional for Zephyr. You must run the following code to configure it. I got stuck here for days
`west config manifest.group-filter -- +tflite-micro && west update`
* Download and un-zip Zephyr SDK
* Run setup.sh inside the Zephyr SDK folder to add the SDK to system path
* OpenOCD was not installed for my OSX computer. To install it, use `brew install openocd` for OSX and `sudo apt install openocd` for Linux (a guess, never tested)
* cd into Zephyr workspace `zephyr` folder
* Run `west build -p always -b YOUR-BOARD-NAME samples/basic/blinky` to build the blinky example
* Run `west flash` to upload the executable onto your board
* If there are no errors in build and flash, the environment is read to use

## SDK/IDE tried
Little progress was made with these SDK/IDE. They were abandoned mainly because the concern of the support for multiple boards are not great.

* Microchip Studio
* MPLAB Harmony
* Keil Studio
* Keil Studio Cloud
* PlatformIO
* Arduino


## Boards
Name of the board and name used in zephyr
* Atmel SAMD21 PLAINED PRO: atsamd21_xpro
* nRF52840 DK: nrf52840dk_nrf52840