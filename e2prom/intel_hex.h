#pragma once

#include "Arduino.h"
#include <stdint.h>

class IntelHex{
public:
    void IntelHex::write_to_eeprom_i2c(uint8_t eeprom_i2c_address=0x50, uint16_t destination_start_byte=0);
#ifdef _DEBUG_
    void parse_intel_hex_from_flash();
#endif // _DEBUG_
};
