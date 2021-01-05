#pragma once

#include <stdint.h>

#define LED_PORT_NAME B
#define LED_PIN 0

static constexpr uint8_t source_i2c_address_for_program =
    0x50; // default address for CAT24M01 I2C EEPROM