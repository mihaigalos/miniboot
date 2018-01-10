#pragma once

#include "I2C-master-lib/i2c_master.h"

uint8_t readByte(uint8_t source_address, uint16_t register_address)
{
  uint8_t data=0xFF;
  source_address=source_address<<1 | 0x01;
  i2c_start(source_address); 
  i2c_write(register_address>>8);
  i2c_write(register_address);
  i2c_stop();
  
  i2c_start(source_address);
  data = i2c_read_ack();
  
  return data;
}

static inline uint16_t getWordFromSource(uint8_t i2c_address, uint16_t data_address){
  uint16_t result = 0xFFFF;
  result |= readByte(i2c_address, data_address);
  result |= (static_cast<uint16_t>(readByte(i2c_address, data_address)))<<8;
}
