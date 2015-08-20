# RPI-UART-Boot
Upload your kernel for Raspberry Pi over UART.
Works on RaspberryPI 1 (teseted on Model B+)

##Usage
* Install `python2` dependency `pyserial`
* Install `arm-none-eabi-gcc` (if you want to compile the firmware)
* Compile firmware:
````bash
cd firmware
make
````
**OR** 
use precompiled `firmware/kernel.img` just because I'm a good guy.

* Mount `boot` partition of your SD card (burned with some OS, like `Raspbian`) and replace `kernel.img` with the one you get after compilation step.
* Connect UART (GPIO pins 14 and 15) to PC via USB-to-TTL connector (or anything else you prefer).
* Open virtual terminal (like `GNU screen`) at `/dev/ttyUSBX` (`X` is a number), `115200` baud.
* Insert MicroSD card and power up Pi.
* Note message 'waiting for image on UART'.
* run `client-tool/rpi-uart-boot.py <path/to/your/kernel.img>`.
* You kernel will be uploaded to address `0x8000`, as if it was booted from MicroSD card.
* Enjoy!

#Licence
Licenced under MIT licence (see `LICENCE`).
Developed by RostakaGmfun for personal purposes.

