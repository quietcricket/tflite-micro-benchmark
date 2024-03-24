# Tensorflow Lite Micro Benchmark: Zephyr Boards

## Setting up the dev environment on OSX

Installation for Linux should be very similar. However, setting up the environment on windows can be challenging because the package installer `chocolatey` is not as great as OSX's `homebrew` or Linux's native `apt`.

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


## Build and upload
- west build -b `boardname` tflite-micro-benchmark: add `-p always` to clean and re-build if necessary
- west flash

## Board names
- Atmel SAMD21 XPLAINED PRO: atsamd21_xpro
- Atmel ATSAMV71-XULT: sam_v71_xult
- nRF52840 DK: nrf52840dk_nrf52840
- STM32 Nucleo-144: nucleo_f429zi