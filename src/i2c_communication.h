#pragma once

#include "I2C-master-lib/i2c_master.h"

static inline uint8_t readByte(const uint8_t source_address,
                               const uint16_t register_address) {
  uint8_t data = 0xFF;
  uint8_t write_source_address = source_address << 1;
  i2c_start(write_source_address);
  i2c_write(register_address >> 8);
  i2c_write(register_address);
  i2c_stop();

  uint8_t read_source_address = write_source_address | 0x01;
  i2c_start(read_source_address);
  data = i2c_read_ack();

  return data;
}

static inline uint16_t getWordFromSource(const uint8_t i2c_address,
                                         const uint16_t data_address) {
  uint16_t result = static_cast<uint16_t>(readByte(i2c_address, data_address))
                    << 8;
  result |= static_cast<uint16_t>(readByte(i2c_address, data_address + 1));
  return result;
}
