#pragma once

#define LED_PORT_NAME B
#define LED_PIN 0
static constexpr uint8_t source_i2c_address_for_program =
    0x50; // default address for CAT24M01 I2C EEPROM

static constexpr uint32_t eeprom_not_programmed =
    0xFFFFFFFF; // default value in eeprom when not programmed

static constexpr uint8_t timestamp_application_byte_offset = 20;
static constexpr uint8_t application_length_byte_offset = 32;
static constexpr uint8_t application_byte_offset = 34;
static constexpr uint8_t application_start_address_byte_offset = 36;