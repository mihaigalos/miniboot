#pragma once

#include "Arduino.h"
#include <stdint.h>

//#define _DEBUG_

class IntelHex {
public:
  IntelHex() : last_written_file_length_(0) {}
  void write_to_eeprom_i2c_old(uint8_t eeprom_i2c_address = 0x50,
                               uint16_t destination_start_byte = 2);
  void write_to_eeprom_i2c(uint8_t eeprom_i2c_address = 0x50,
                           uint16_t destination_start_byte = 2);

#ifdef _DEBUG_
  void parse_intel_hex_from_flash();
#endif // _DEBUG_

private:
  void write_preamble(uint8_t eeprom_i2c_address,
                      uint16_t destination_start_byte);
  void write_vector(uint8_t eeprom_i2c_address, uint16_t destination_start_byte,
                    uint8_t *vector, uint8_t size);
  void write_variable(uint8_t eeprom_i2c_address,
                      uint16_t destination_start_byte, uint8_t *data,
                      uint8_t size_in_bytes);

  uint32_t GetValueFromSerial(char *message, uint8_t base = 10) const;

  uint16_t last_written_file_length_;
};
