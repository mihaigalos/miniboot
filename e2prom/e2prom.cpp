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
#include "e2prom.h"

using namespace std;

uint8_t E2PROM::ownAddress_;

E2PROM::E2PROM(uint8_t ownAddress, uint32_t speed) {
  Wire.setClock(speed);
  Wire.begin();
  E2PROM::ownAddress_ = ownAddress;
}

uint8_t E2PROM::readByte(uint16_t registerAddress) {
  uint8_t data = 0x55;

  TEEepromResult result = TEEepromResult_Ok;
  do {
    Wire.beginTransmission(ownAddress_);
    Wire.write(static_cast<uint8_t>(registerAddress >> 8));
    Wire.write(static_cast<uint8_t>(registerAddress));
    result = Wire.endTransmission();
  } while (TEEepromResult_Ok != result);

  // Ask the I2C device for data
  Wire.requestFrom(ownAddress_, static_cast<uint8_t>(1));
  while (!Wire.available())
    ;
  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}

void E2PROM::dump(int8_t columnCount) {
  Serial.begin(9600);
  for (auto i = 0; i < 4 * 1024; i++) {
    Serial.print(static_cast<char>(readByte(i)));
  }

  Serial.print('\n');
}

TEEepromResult E2PROM::writeByte(uint16_t registerAddress, uint8_t data) {
  TEEepromResult result = TEEepromResult_Ok;
  Wire.beginTransmission(ownAddress_);
  Wire.write(static_cast<uint8_t>(registerAddress >> 8));
  Wire.write(static_cast<uint8_t>(registerAddress));
  Wire.write(data);
  result = Wire.endTransmission();
  delay(5);
  return result;
}

TEEepromResult E2PROM::writePage(uint16_t registerAddress, uint8_t *buffer,
                                 uint8_t byteCount) {
  TEEepromResult result = TEEepromResult_Ok;

  if (byteCount > 32)
    result = TEEepromResult_BufferGreaterAsPageSize;
  else {

    Wire.beginTransmission(ownAddress_);
    Wire.write(static_cast<uint8_t>(registerAddress >> 8));
    Wire.write(static_cast<uint8_t>(registerAddress));
    for (auto i = 0; i < byteCount; i++) {
      Wire.write(buffer[i]);
      delay(5);
    }
    result = Wire.endTransmission();
    delay(5);
  }
  Wire.end();
  return result;
}

TEEepromResult E2PROM::readPage(uint16_t registerAddress, uint8_t *buffer,
                                uint8_t byteCount) {
  TEEepromResult result = TEEepromResult_Ok;

  if (byteCount > 32)
    result = TEEepromResult_BufferGreaterAsPageSize;
  else {
    do {
      Wire.beginTransmission(ownAddress_);
      Wire.write(static_cast<uint8_t>(registerAddress >> 8));
      Wire.write(static_cast<uint8_t>(registerAddress));
      result = Wire.endTransmission();

      Wire.requestFrom(ownAddress_, byteCount);
    } while (TEEepromResult_Ok != result);

    for (auto i = 0; i < byteCount; i++) {
      buffer[i++] = Wire.read();
    }
  }
  Wire.end();

  return result;
}
