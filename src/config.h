#pragma once

#define LED_PORT_NAME B
#define LED_PIN 0
static constexpr uint8_t source_i2c_address_for_program =
    0x50; // default address for CAT24M01 I2C EEPROM

static constexpr uint32_t eeprom_not_programmed =
    0xFFFFFFFF; // default value in eeprom when not programmed

static constexpr uint16_t eeprom_address_offset = 0x0000;

static constexpr uint16_t timestamp_application_byte_offset = 20 + eeprom_address_offset;
static constexpr uint16_t application_length_byte_offset = 32 + eeprom_address_offset;
static constexpr uint16_t application_byte_offset = 34 + eeprom_address_offset;
static constexpr uint16_t application_start_address_byte_offset = 36 + eeprom_address_offset;
static constexpr uint16_t application_crc_expected_index = 28 + eeprom_address_offset;