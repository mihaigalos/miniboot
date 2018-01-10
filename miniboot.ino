#include "init.h"
#include "flash.h"
#include "i2c_communication.h"

static inline uint16_t getDataStartAddressInSource(uint8_t i2c_address){
  return 0;
}

static inline uint16_t getDataLength(uint8_t i2c_address){
  // multiple of SPM_PAGESIZE (128)
  return 4*1024;
}

void writeToPageBuffer(uint16_t data, uint16_t address){
  if (RESET_VECTOR == address) {
    data = jmp_instruction;
  } else if (RESET_VECTOR_ARGUMENT_ADDRESS == address) {
    data = (BOOTLOADER_START_ADDRESS/2);
  } 
  writeWordToPageBuffer(data,address);
}

static inline void writeFlashFromI2C(uint8_t i2c_address){
  uint16_t start_address = getDataStartAddressInSource(i2c_address);
  uint16_t length = getDataLength(i2c_address);
  uint16_t data_byte = getWordFromSource(i2c_address, start_address);
  uint16_t destination_address = 0;
  for(uint16_t i = start_address; i<length; ++i,destination_address+=2){
    uint16_t payload = getWordFromSource(i2c_address, i);
    writeToPageBuffer(payload, destination_address);
    if(0 == destination_address % SPM_PAGESIZE){
      boot_page_write_safe(destination_address);
    }
  }
}

int main(){
  init();
  eraseFlash();
  writeFlashFromI2C(SOURCE_I2C_ADDRESS_FOR_PROGRAM_TO_WRITE);
  return 0;
}
