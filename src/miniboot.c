//
//  Fuses :     Ext: 0xFF, Hi: 0xD8, Lo: 0xE2
//

#include "Drivers/CRC/crc32.h"
#include "bootloader.h"
#include "eeprom.h"
#include "flash.h"
#include "i2c_communication.h"
#include "init.h"

static inline uint16_t getDataStartAddressInSource(const uint8_t i2c_address) {
  return application_byte_offset;
}

static inline uint16_t getDataLength(const uint8_t i2c_address) {
  return getWordFromSource(i2c_address, application_length_byte_offset);
}

static inline void writeToFlash(const uint16_t address, uint8_t *data,
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

static inline bool isCrcOk(const uint8_t i2c_address) {
  bool status = false;
  uint32_t crc = 0;
  uint16_t start_address = getDataStartAddressInSource(i2c_address);
  uint16_t length = getDataLength(i2c_address);

  uint32_t table[crc_table_size];
  init_table(&table[0]);

  for (uint16_t pos = 0; pos < length + 1;
       pos += 2) { // length +1 in case of odd number lengths
    if (pos >= length)
      break;

    uint16_t data = getWordFromSource(i2c_address, pos + start_address);
    if (pos == length - 1)
      data &= 0xFF00;

    uint8_t to_little_endian[2];
    to_little_endian[0] = static_cast<uint8_t>(data >> 8);
    to_little_endian[1] = static_cast<uint8_t>(data);

    crc32(reinterpret_cast<const void *>(&to_little_endian[0]), 2, &table[0],
          &crc);

    uint8_t toggle_led_every_x_bytes = 128;
    if (0 == pos % toggle_led_every_x_bytes)
      LED_TOGGLE();
  }

  uint32_t expected_crc = static_cast<uint32_t>(getWordFromSource(
                              i2c_address, application_crc_expected_index))
                          << 16;
  expected_crc |= static_cast<uint32_t>(
      getWordFromSource(i2c_address, application_crc_expected_index + 2));

  if (crc == expected_crc) {
    status = true;
  }
  return status;
}

static inline void writeFlashFromI2C(const uint8_t i2c_address,
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

[[ noreturn ]]
static inline void leaveBootloader(uint16_t &application_start) {
  // hold my beer and watch this!
  reinterpret_cast<void (*)(void)>(application_start)();
  while (1)
    ;
}

static inline bool isReflashNecessary(uint32_t &i2c_application_timestamp) {
  uint32_t current_application_timestamp =
      readLatestApplicationTimestampFromInternalEeprom();

  i2c_application_timestamp =
      static_cast<uint32_t>(getWordFromSource(
          source_i2c_address_for_program, timestamp_application_byte_offset))
      << 16;
  i2c_application_timestamp |= static_cast<uint32_t>(getWordFromSource(
      source_i2c_address_for_program, timestamp_application_byte_offset + 2));

  if (eeprom_not_programmed == current_application_timestamp)
    return true;
  if (i2c_application_timestamp != current_application_timestamp)
    return true;
  return false;
}

int main() {

  init();

  uint32_t i2c_application_timestamp;
  uint16_t application_start = 0;

  if (isReflashNecessary(i2c_application_timestamp) &&
      isCrcOk(source_i2c_address_for_program)) {
    eraseApplication();
    writeFlashFromI2C(source_i2c_address_for_program, application_start);
    writeLatestApplicationTimestampToInternalEeprom(i2c_application_timestamp);
  } else {
    uint16_t address_in_external_eeprom = getWordFromSource(
        source_i2c_address_for_program, application_start_address_byte_offset);

    application_start = address_in_external_eeprom >> 8;
    application_start |= static_cast<uint16_t>(static_cast<uint8_t>(address_in_external_eeprom))<<8;
  }
  leaveBootloader(application_start);

  return 0;
}
