/*****************************************************************************
*
* This file is part of E2PROM.
*
* E2PROM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* E2PROM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with E2PROM.  If not, see <http://www.gnu.org/licenses/>.
*
******************************************************************************/
#pragma once
#include "Arduino.h"
#include <Wire.h>
#include <stdint.h>

#ifndef CLOCK_SPEED_400KHZ
#define CLOCK_SPEED_400KHZ 400000
#endif

typedef enum {
  TEEepromResult_Ok,
  TEEepromResult_BufferGreaterAsPageSize,
  TEEepromResult_NackOnAddressTransmit,
  TEEepromResult_NackOnDataTransmit,
  TEEepromResult_OtherError
} TEEepromResult;

class E2PROM {
  static uint8_t ownAddress_;

public:
  static const uint8_t eeprom_page_size = 16;
  E2PROM(uint8_t ownAddress, uint32_t speed = CLOCK_SPEED_400KHZ);

  uint8_t readByte(uint16_t registerAddress);
  TEEepromResult writeByte(uint16_t registerAddress, uint8_t data);
  TEEepromResult writePage(uint16_t registerAddress, uint8_t *buffer,
                           uint8_t byteCount);
  TEEepromResult readPage(uint16_t registerAddress, uint8_t *buffer,
                          uint8_t byteCount);
  void dump(int8_t columnCount = -1);
};
