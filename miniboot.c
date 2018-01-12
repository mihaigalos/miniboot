#include "bootloader.h"
#include "init.h"
#include "flash.h"
#include "i2c_communication.h"

static inline uint16_t getDataStartAddressInSource(uint8_t i2c_address){
  return 0;
}

static inline uint16_t getDataLength(uint8_t i2c_address){
  // multiple of SPM_PAGESIZE (128)
  return 128*30;
}

void adjustAddresses(uint16_t address, uint8_t * data){
  /*if (0 == address) {
    data[RESET_VECTOR] = static_cast<uint8_t>(jmp_instruction>>8);
    data[RESET_VECTOR] = static_cast<uint8_t>(jmp_instruction);
    data[RESET_VECTOR_ARGUMENT_ADDRESS] = static_cast<uint8_t>(BOOTLOADER_START_ADDRESS/2 >>8);
    data[RESET_VECTOR_ARGUMENT_ADDRESS] = static_cast<uint8_t>(BOOTLOADER_START_ADDRESS/2);
  }*/
  writeToPageBuffer(address, data);
  writePageBufferToFlash(address);
}


static inline void writeFlashFromI2C(uint8_t i2c_address){
  uint16_t start_address = getDataStartAddressInSource(i2c_address);
  uint16_t length = getDataLength(i2c_address);
  uint8_t buf[SPM_PAGESIZE];
  uint8_t writes = 0;
  
  for(uint16_t word_count = start_address; word_count<length/2; word_count+=2){
    
    if(word_count>0 && (0 == (word_count % SPM_PAGESIZE))){
      adjustAddresses(writes*SPM_PAGESIZE, &buf[0]);
      LED_TOGGLE();
      ++writes;
    }
    buf[word_count%SPM_PAGESIZE] = 0x55;//getWordFromSource(i2c_address, i);
    buf[(word_count+1)%SPM_PAGESIZE] = 0x55;
    
    /*
    if (word_count <BOOTLOADER_START_ADDRESS) {

      uint16_t payload = getWordFromSource(i2c_address, i);

      adjustAddresses(word_count, payload);
      if(word_count>0 && (0 == (word_count % SPM_PAGESIZE))){
        writePageBufferToFlash(word_count);
        LED_TOGGLE();
      }

    }*/
  }
}

int main(){
  init();
  //eraseApplication();
  writeFlashFromI2C(source_i2c_address_for_program);
  return 0;
}
