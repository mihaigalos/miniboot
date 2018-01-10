#pragma once

#include <Wire.h>

enum I2CTransferStatus{
  I2CTransferStatus_Ok,
  I2CTransferStatus_BufferGreaterAsPageSize,
  I2CTransferStatus_NackOnAddressTransmit,
  I2CTransferStatus_NackOnDataTransmit,
  I2CTransferStatus_OtherError
};

uint8_t readByte(uint8_t source_address, uint16_t register_address)
{
  uint8_t data=0xFF;

  I2CTransferStatus result = I2CTransferStatus_Ok;
  
  Wire.beginTransmission(source_address); 
  Wire.write(static_cast<uint8_t>(register_address>>8));
  Wire.write(static_cast<uint8_t>(register_address));
  result = Wire.endTransmission();
  
  Wire.requestFrom(source_address, static_cast<uint8_t>(1));
  while(!Wire.available());
  if(Wire.available()){
    data = Wire.read();
  }
  return data;
}

static inline uint16_t getWordFromSource(uint8_t i2c_address, uint16_t data_address){
  uint16_t result = 0xFFFF;
  result |= readByte(i2c_address, data_address);
  result |= (static_cast<uint16_t>(readByte(i2c_address, data_address)))<<8;
}
