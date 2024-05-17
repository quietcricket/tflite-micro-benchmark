
# Tensorflow Lite Micro Benchmark 

Some boards works with platformio and unfortunately Zephyr support in platformio is not working with tflite. So the codes are separated in two groups. The pure Zephyr based boards are located in this root folder. Boards using Arduino framework are located in `platformio-board` folder. 


## Results

| Board                                     | Framework | ms per 1000 inferences | CPU         | Frequency | Power |
|-------------------------------------------|-----------|------------------------|-------------|-----------|-------|
| Atmel SAMD21 XPLAINED PRO (atsamd21_xpro) | Zephyr    | 535                    | Cortex-M0+  | 48        | 0.69  |
| nRF52840 DK (nrf52840dk_nrf52840)         | Zephyr    | 208                    | Cortex-M4F  | 64        | 0.13  |
| STM32 Nucleo-144 (nucleo_f429zi)          | Arduino   | 88                     | Cortex-M4   | 180       | 0.79  |
| Atmel ATSAMV71-XULT (sam_v71_xult)        | Zephyr    | 28                     | Cortex-M7   | 300       | 1.21  |
| Raspberry Pi Pico                         | Arduino   | 167                    | Cortex-M0+  | 133       | 0.13  |
| ESP32-S                                   | Arduino   | 86                     | Xtensa® LX6 | 240       | 0.39  |
| Teensy 4.1                                | Arduino   | 9                      | Cortex-M7   | 600       | 0.52  |
| Arduino Portenta H7                       | Arduino   | 23                     | Cortex-M7   | 480       | 1.03  |



## Zephyr
### dev environment setup on OSX

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


### Build and upload
- west build -b `boardname` tflite-micro-benchmark: add `-p always` to clean and re-build if necessary
- west flash

### Board names
- Atmel SAMD21 XPLAINED PRO: atsamd21_xpro
- Atmel ATSAMV71-XULT: sam_v71_xult
- nRF52840 DK: nrf52840dk_nrf52840
- STM32 Nucleo-144: nucleo_f429zi
- Novoton IOT M487: nuvoton_pfm_m487 (unable to flash)


## Arduino 

Boards using Arduino framework are built with platformio. Installation and build process are very straightforward. Even though platformio supports Zephyr, however it is very hard to fix all the compilation errors in tflite. 

Some boards have problem with Arduino.h abs macro, e.g. Arduino Portenta H7. Comment out the abs macro and change WMath.h srandom random to srand and rand fixes the problem.
