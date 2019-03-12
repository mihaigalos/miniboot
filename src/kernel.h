#ifndef KERNEL_H_
#define KERNEL_H_


void Miniboot::writeToFlash(const uint16_t address, uint8_t *data,
                                uint16_t &application_start) {

  if (0 == address && 0 == application_start) {
    application_start = static_cast<uint16_t>(data[RESET_VECTOR_ARGUMENT_ADDRESS]) << 8;
    application_start |=
        static_cast<uint8_t>(data[RESET_VECTOR_ARGUMENT_ADDRESS + 1]);

    data[RESET_VECTOR] = static_cast<uint8_t>(jmp_instruction);
    data[RESET_VECTOR + 1] = static_cast<uint8_t>(jmp_instruction >> 8);

    data[RESET_VECTOR_ARGUMENT_ADDRESS] = static_cast<uint8_t>(
        static_cast<uint16_t>(BOOTLOADER_START_ADDRESS) / 2);
    data[RESET_VECTOR_ARGUMENT_ADDRESS + 1] = static_cast<uint8_t>(
        (static_cast<uint16_t>(BOOTLOADER_START_ADDRESS) / 2) >> 8);
  }

  writeToPageBuffer(address, data);
  writePageBufferToFlash(address);
}


void Miniboot::writeFlashFromI2C(const uint8_t i2c_address,
                                     uint16_t &application_start) {
  uint16_t start_address = getDataStartAddressInSource(i2c_address);
  uint16_t length = getDataLength(i2c_address);
  uint8_t buf[SPM_PAGESIZE];
  uint16_t writes = 0;

  for (uint16_t pos = 0; pos < length; pos += 2) {
    if (pos > 0 && (0 == (pos % SPM_PAGESIZE))) {
      writeToFlash(writes * SPM_PAGESIZE, &buf[0], application_start);
      LED_TOGGLE();
      ++writes;
    }
    uint16_t data = getWordFromSource(i2c_address, pos + start_address);
    buf[pos % SPM_PAGESIZE] = static_cast<uint8_t>(data >> 8);
    buf[(pos + 1) % SPM_PAGESIZE] = static_cast<uint8_t>(data);
  }

  for (uint16_t pos = SPM_PAGESIZE -
                      (static_cast<uint16_t>(writes + 1) *
                       static_cast<uint16_t>(SPM_PAGESIZE)) %
                          length;
       pos < SPM_PAGESIZE; ++pos) {
    buf[pos] = 0xFF; // reset contents, since these bytes were not filled in
                     // this page and have value from previous page
  }

  writeToFlash(writes * SPM_PAGESIZE, &buf[0], application_start);
  LED_OFF();
}

#endif //KERNEL_H_
