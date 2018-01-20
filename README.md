# Miniboot - an I2C bootloader for Arduino [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE)

Miniboot started out as a weekend project early in 2018.
It is an I2C bootloader for Arduino, tested on AtMega328p. It is designed to reflash
the Mega328p with code residing in an external I2C memory or another I2C device. For testing,
CAT24M01 32kByte external EEPROM was used.

# Usage

## Get the sources

`git clone --recursive  https://github.com/mihaigalos/miniboot.git`

If you've downloaded the sources manually, make sure to also manually download the dependencies (i.e. : I2C-master-lib), they are not automatically downloaded!

## Build the hex
`make rebuild`

This will produce the miniboot.hex file you can then flash to your Arduino. The bootloader
automatically starts on a reset and looks for an I2C device at address 0x50 (CAT24M01's default address).

It then reads the length of the data and it starts reflashing the microcontroller with the information
stated there. It expects the following memory layout in the EEPROM:

- [last free byte pointer]          : 2 bytes - should point to first byte after the appplication
- ['m' 'i' 'n' 'i' 'b' 'o' 'o' 't'] : 8 bytes
- [name of application]             : 10 bytes - user defined
- [timestamp application]           : 4 bytes - unix timestamp when the application in the I2C memory was generated
- [timestamp of write]              : 4 bytes - unix timestamp when the application was flashed to the I2c memory
- [CRC32]                           : 4 bytes - generated with polynomial representation 0xEDB88320. Details in Drivers/CRC.
- [length]                          : 2 bytes - amount of bytes for the application
- [application]                     : n bytes - actual payload of the application code

Miniboot computes a CRC32 checksum on the payload, excuding the header metadata and starting with the first byte of
the application.
It is important that the length be at byte location 32 and the application start at byte 34.

# Bootloader start address

Miniboot can be modified to place the bootloader code at a specific address, via
the macro BOOTLOADER_START_ADDRESS. Please ensure that the address lies in the bootloader
section, specified by the High fusebits. I reccommend using at least 1024 words (2048 bytes) for
the bootloader section (Hi fuse : 0xD8). If you choose to modify miniboot, please ensure that your new bootloader
fits in the section you specify with the Hi fuse.

Computing the hexadecimal address for bootloader start section:
- make clean; make main.hex; take output, let's say it's 2123 (or similar)
- for the size of your device (8kb = 1024 * 8 = 8192) minus above value 2123... = 6069
- Which yelds 6069 / 64 (mega328p page size in bytes) = 94.828125 pages of flash memory
- round it down to 94 - our new bootloader address is 94 * 64 = 6016, in hex = 1780h
- put the value in the BOOTLOADER_START_ADDRESS macro in the Makefile of manually edit bootloader.h

# Internal EEPROM last application timestamp

After running, miniboot writes the application timestamp (specified earlier) to 4 bytes in the microcontroller's
internal EEPROM. The next time the system restarts, it will compare the application's timestamp with the information
it reads from the internal eeprom and will only rewrite it again if the timestamp is newer or the internal eeprom
is unprogrammed (4 bytes of 0xFF). This prevents a new rewrite on each system restart.

The variable EEPROM_CONFIGURATION_START_BYTE can be edited to generate the desired macro in bootloader.h for the above logic.

# Storing the application in the EEPROM

To write an application to the I2C memory, one can use a USB to UART bridge.

## Prepare the UART to I2C Bridge

The arduino needs to be flashed with the miniboot_uart_to_eeprom_uploader.hex sketch which can be found in the Drivers/Eeprom/bin folder in this repo. This will read from the UART and will put the information in the external I2C memory.

## Create the binary file

Next, export the hex file of the application (the one you want to transfer to the I2C memory) to binary before continuing:

`avr-objcopy.exe -I ihex application.hex -O binary application.bin`

## Send it!
We finally want to send it over UART to the microcontroller which will write it to the I2C memory.

Br@y's Terminal is broken when using the send file feature.
Use YAT Terminal to send the file via UART instead.

In YAT, go to Terminal->Settings and select your baudrate:

- If your Arduino is running at 16Mhz : select 600 bits per second.
- If your Arduino is running at 8Mhz or have a custom board runing at 8Mhz: select 300 bits per second.

The low baudrate ensures the external EEPROM has time to write the payload it receives.

Follow the instructions to input the application unix timestamp, the unix timestamp of the time of writing, crc value and the data length.
For the unix timestamp, you can use the [epoch converter](https://www.epochconverter.com/).
For computing the crc32 on your binary file, drag and drop application.bin created earlier in the webpage found [here](http://emn178.github.io/online-tools/crc32_checksum.html).

You will next be prompted to switch to binary mode and send the binary file.

YAT tries to parse the \r\n sequence when sending, which leads to an error being shown and the transmission is interrupted, the file is not fully sent.
To overcome this, ->select Terminal Type: binary. Select Binary Settings... and uncheck everythig.

Next select the exported binary file (application.bin) and click Send File.

You should now be able to send the file in full via UART.

# License
This software is freely available under the GNU GPL v3.0 aegis, please consult the LICENSE file for further information.