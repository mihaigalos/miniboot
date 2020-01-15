#pragma once

#include "I2C-master-lib/i2c_master.h"
#include "config.h"

static inline uint8_t readByte(const uint16_t register_address) {
  uint8_t data = 0xFF;
  uint8_t write_source_address = source_i2c_address_for_program << 1;
  i2c_start(write_source_address);
  i2c_write(register_address >> 8);
  i2c_write(register_address);
  i2c_stop();

  uint8_t read_source_address = write_source_address | 0x01;
  i2c_start(read_source_address);
  data = i2c_read_ack();

  return data;
}

static inline uint16_t getWordFromSource(const uint16_t data_address) {
  uint16_t result = static_cast<uint16_t>(readByte(data_address))
                    << 8;
  result |= static_cast<uint16_t>(readByte(data_address + 1));
  return result;
}
