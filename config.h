#pragma once

// hexadecimal address for bootloader start section. To calculate the best value:
// - make clean; make main.hex; ### take output, let's say it's 2123 (or similar)
// - for the size of your device (8kb = 1024 * 8 = 8192) minus above value 2123... = 6069
// - Which yelds 6069 / 64 (mega328p page size in bytes) = 94.828125 pages of flash memory
// - round it down to 94 - our new bootloader address is 94 * 64 = 6016, in hex = 1780h
// - put the value in the BOOTLOADER_START_ADDRESS macro: #define BOOTLOADER_START_ADDRESS 1780

#define BOOTLOADER_START_ADDRESS 6800

#define LED_PORT_NAME D
#define LED_PIN 1
#define SOURCE_I2C_ADDRESS_FOR_PROGRAM_TO_WRITE 0x50 // default address for CAT24M01 I2C EEPROM


