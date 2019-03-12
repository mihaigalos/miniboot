//
//  Fuses :     Ext: 0xFF, Hi: 0xD8, Lo: 0xE2
//

#include "Drivers/CRC/crc32.h"
#include "bootloader.h"
#include "eeprom.h"
#include "flash.h"
#include "i2c_communication.h"
#include "init.h"

#include "miniboot.h"
#include "kernel.h"


uint16_t Miniboot::getDataStartAddressInSource(const uint8_t i2c_address) {
  return application_byte_offset;
}

uint16_t Miniboot::getDataLength(const uint8_t i2c_address) {
  return getWordFromSource(i2c_address, application_length_byte_offset);
}


bool Miniboot::isCrcOk(const uint8_t i2c_address) {
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


bool Miniboot::isReflashNecessary(uint32_t &i2c_application_timestamp) {
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
  Miniboot miniboot;
  init();

  uint32_t i2c_application_timestamp;
  uint16_t application_start = 0;

  if (miniboot.isReflashNecessary(i2c_application_timestamp) &&
      miniboot.isCrcOk(source_i2c_address_for_program)) {
    eraseApplication();
    miniboot.writeFlashFromI2C(source_i2c_address_for_program, application_start);
    writeLatestApplicationTimestampToInternalEeprom(i2c_application_timestamp);
  } else {
    uint16_t address_in_external_eeprom = getWordFromSource(
        source_i2c_address_for_program, application_start_address_byte_offset);

    application_start = address_in_external_eeprom >> 8;
    application_start |= static_cast<uint8_t>(address_in_external_eeprom);
  }
  miniboot.leaveBootloader(application_start);

  return 0;
}
