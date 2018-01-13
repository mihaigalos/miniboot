//
//  Fuses :     Ext: 0xFF, Hi: 0xD9, Lo: 0xE2
//

#include "bootloader.h"
#include "flash.h"
#include "i2c_communication.h"
#include "init.h"

static inline uint16_t getDataStartAddressInSource(uint8_t i2c_address) {
  return 34;
}

static inline uint16_t getDataLength(uint8_t i2c_address) {
  return getWordFromSource(i2c_address, 32);  
}

void writeToFlash(uint16_t address, uint8_t *data) {
  if (0 == address) {
    
    data[RESET_VECTOR] = static_cast<uint8_t>(jmp_instruction);
    data[RESET_VECTOR+1] = static_cast<uint8_t>(jmp_instruction >> 8);
    data[RESET_VECTOR_ARGUMENT_ADDRESS] = static_cast<uint8_t>(static_cast<uint16_t>(BOOTLOADER_START_ADDRESS) / 2);
    data[RESET_VECTOR_ARGUMENT_ADDRESS+1] = static_cast<uint8_t>((static_cast<uint16_t>(BOOTLOADER_START_ADDRESS) / 2) >> 8);
  }
  writeToPageBuffer(address, data);
  writePageBufferToFlash(address);
}

void delay(volatile uint16_t count) {
  for (volatile uint16_t i = 0; i < count; ++i)
    ;
}

static inline void writeFlashFromI2C(uint8_t i2c_address) {
  uint16_t start_address = getDataStartAddressInSource(i2c_address);
  uint16_t length = getDataLength(i2c_address);
  uint8_t buf[SPM_PAGESIZE];
  uint8_t writes = 0;

  uint16_t word_count = length / 2 + length % 2;

  for (uint16_t pos = 0; pos < word_count; pos += 2) {

    if (pos > 0 && (0 == (pos % SPM_PAGESIZE))) {
      writeToFlash(writes * SPM_PAGESIZE, &buf[0]);
      LED_TOGGLE();
      ++writes;
    }
    uint16_t data = getWordFromSource(i2c_address, pos+start_address);
    buf[pos % SPM_PAGESIZE] = static_cast<uint8_t>(data); //
    buf[(pos + 1) % SPM_PAGESIZE] = static_cast<uint8_t>(data >> 8);    
  }
}

int main() {
  init();
  eraseApplication();
  writeFlashFromI2C(source_i2c_address_for_program);
  while(1);
  
  return 0;
}
