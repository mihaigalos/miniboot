#include "intel_hex.h"
#include "e2prom.h"

#define debug(X) Serial.print(X)
#define debug2(X, Y) Serial.print(X, Y)
#define debugn(X) Serial.println(X)

const uint8_t hex_file_line_length = 43;
const uint8_t metadata_length = 11; // inclues <':'-1 character><size-2
                                    // characters><address-4 characters><type-2
                                    // characters><CRC-2 characters>

typedef enum { Intel_Hex_Data = 0, Intel_Hex_EndOfFile } TRecordType;

uint32_t hex2int(const char *source_address, uint8_t start, uint8_t count) {
  uint32_t val = 0;
  for (uint8_t i = 0; i < count; ++i) {
    uint8_t byte = pgm_read_byte(source_address + start + i);
    if (byte >= '0' && byte <= '9')
      byte = byte - '0';
    else if (byte >= 'A' && byte <= 'F')
      byte = byte - 'A' + 10;
    val = (val << 4) | (byte & 0xF);
  }
  return val;
}

const uint8_t blink_hex[] PROGMEM = {
    ":100000000C945E000C9470000C9470000C947000C2\
:100010000C9470000C9470000C9470000C947000A0\
:100020000C9470000C9470000C9470000C94700090\
:100030000C9470000C9470000C9470000C94700080\
:100040000C94BA010C9470000C9470000C94700025\
:100050000C9470000C9470000C9470000C94700060\
:100060000C9470000C947000000000080002010065\
:100070000003040700000000000000000000000072\
:10008000250028002B0000000000240027002A0083\
:10009000040404040404040402020202020203032E\
:1000A000030303030202010204081020408001023E\
:1000B00004081020010204081020408011241FBEF3\
:1000C000CFEFD8E0DEBFCDBF21E0A0E0B1E001C0BE\
:1000D0001D92A930B207E1F70E9404020C9493022A\
:1000E0000C94000090E0FC01EA55FF4F2491FC01C4\
:1000F000E057FF4F8491882399F090E0880F991F73\
:10010000FC01EA57FF4FA591B491FC01E458FF4F61\
:10011000859194918FB7F894EC91E22BEC938FBF7B\
:10012000089590E0FC01E859FF4F3491FC01EA5535\
:10013000FF4F2491FC01E057FF4FE491EE2309F4B7\
:100140003BC0332339F1333091F038F43130A9F02A\
:10015000323001F584B58F7D12C0373091F03830E0\
:10016000A1F03430B9F4809180008F7D03C080917C\
:1001700080008F77809380000DC084B58F7784BD19\
:1001800009C08091B0008F7703C08091B0008F7D4F\
:100190008093B000F0E0EE0FFF1FE458FF4FA591F1\
:1001A000B4918FB7F894EC91611103C020952E2380\
:1001B00001C02E2B2C938FBF08951F93CF93DF93F5\
:1001C000182FEB010E947200209711F460E004C028\
:1001D000CF3FD10539F461E0812FDF91CF911F919D\
:1001E0000C949100E12FF0E0E859FF4FE491E330E7\
:1001F000F9F048F4E130B9F0E230A1F584B580625D\
:1002000084BDC8BD34C0E730E9F0E83019F1E4300E\
:1002100049F580918000806280938000D0938B00AC\
:10022000C0938A0024C084B5806884BDC7BD1FC048\
:1002300080918000806880938000D0938900C09373\
:10024000880015C08091B00080688093B000C09392\
:10025000B3000DC08091B00080628093B000C09365\
:10026000B40005C0C038D1050CF0B5CFAFCFDF91D9\
:10027000CF911F9108953FB7F89480910501909117\
:100280000601A0910701B091080126B5A89B05C001\
:100290002F3F19F00196A11DB11D3FBFBA2FA92F05\
:1002A000982F8827820F911DA11DB11DBC01CD0182\
:1002B00042E0660F771F881F991F4A95D1F708956E\
:1002C0008F929F92AF92BF92CF92DF92EF92FF9266\
:1002D0000E943B014B015C0188EEC82E83E0D82EC2\
:1002E000E12CF12C0E943B01DC01CB01881999091A\
:1002F000AA09BB09883E9340A105B10558F021E049\
:10030000C21AD108E108F10888EE880E83E0981E31\
:10031000A11CB11CC114D104E104F10419F7FF9030\
:10032000EF90DF90CF90BF90AF909F908F90089507\
:10033000EF92FF920F931F93CF93DF937B018A017C\
:10034000CFEFD0E0BE01681B790B88E00E94DD0092\
:10035000BE016E197F0981E00E94DD00BE01601BB5\
:10036000710B80E0DF91CF911F910F91FF90EF9083\
:100370000C94DD001F920F920FB60F9211242F9351\
:100380003F938F939F93AF93BF938091010190917F\
:100390000201A0910301B09104013091000123E01A\
:1003A000230F2D3720F40196A11DB11D05C026E8AD\
:1003B000230F0296A11DB11D20930001809301011E\
:1003C00090930201A0930301B09304018091050171\
:1003D00090910601A0910701B09108010196A11D1D\
:1003E000B11D8093050190930601A0930701B0937E\
:1003F0000801BF91AF919F918F913F912F910F90E5\
:100400000FBE0F901F901895789484B5826084BDBC\
:1004100084B5816084BD85B5826085BD85B5816008\
:1004200085BD80916E00816080936E001092810086\
:100430008091810082608093810080918100816041\
:100440008093810080918000816080938000809102\
:10045000B10084608093B1008091B000816080938E\
:10046000B00080917A00846080937A0080917A0055\
:10047000826080937A0080917A00816080937A0014\
:1004800080917A00806880937A001092C10088E0A1\
:100490000E94720081E00E94720080E00E9472005F\
:1004A000C0E0D0E040E050E060E070E08FEF90E02E\
:1004B0000E9498010E94600140E050E06FEF70E000\
:1004C00080E090E00E9498010E9460014FEF50E0B0\
:1004D00060E070E080E090E00E9498010E9460017E\
:1004E00040E050E06FEF70E08FEF90E00E949801E5\
:1004F0000E94600140E550E060E070E080E590E03F\
:100500000E9498010E9460014FEF50E06FEF70E091\
:1005100080E090E00E9498010E946001209711F213\
:0A0520000E940000BFCFF894FFCF47\
:00000001FF"};

#ifdef _DEBUG_

void IntelHex::parse_intel_hex_from_flash() {

  Serial.begin(9600);

  uint8_t *p = &blink_hex[0];
  uint8_t step_in_file = hex_file_line_length;

  for (uint16_t i = 0;; ++i, p += step_in_file) {
    uint8_t page_size = hex2int(p, 1, 2);
    if (page_size * 2 + metadata_length < hex_file_line_length)
      step_in_file = page_size * 2 + metadata_length;

    debug("step: ");
    debug2(step_in_file, DEC);

    debug("    size: ");
    debug2(page_size, HEX);

    uint32_t address = hex2int(p, 3, 4);
    debug("    address: ");

    if (address <= 0xF)
      debug("000");
    else if (address <= 0xFF)
      debug("00");
    else if (address <= 0x0FFF)
      debug("0");
    debug2(address, HEX);

    uint8_t record_type = hex2int(p, 7, 2);
    debug("    type: ");
    debug2(record_type, HEX);

    debug("    payload: ");
    if (0 == page_size &&
        Intel_Hex_EndOfFile == static_cast<TRecordType>(record_type)) {
      debug("FF");
    } else {
      for (int j = 0; j < page_size * 2; j += 4) {
        uint32_t payload = hex2int(p, 9 + j, 4);

        if (payload <= 0xF)
          debug("000");
        else if (payload <= 0xFF)
          debug("00");
        else if (payload <= 0x0FFF)
          debug("0");
        debug2(payload, HEX);
        debug(" ");
      }
    }
    debugn("");

    if (0 == page_size)
      break; // end
  }
}
#endif // _DEBUG_

void IntelHex::write_to_eeprom_i2c_old(uint8_t eeprom_i2c_address,
                                       uint16_t destination_start_byte) {
  E2PROM e(eeprom_i2c_address);
  uint16_t total_size = sizeof(blink_hex) / sizeof(blink_hex[0]);
  Serial.begin(9600);
  Serial.print("Total example intel hex file raw size: ");
  Serial.print(total_size);
  Serial.println(" bytes.");
  Serial.println(">>> start i2c write");

  uint8_t step_in_file = hex_file_line_length;
  uint8_t current_page = 0;

  for (uint16_t position_in_file = 0; position_in_file < total_size;
       position_in_file += step_in_file, ++current_page) {
    uint8_t page_size = hex2int(&blink_hex[position_in_file], 1, 2);
    uint8_t payload_position_in_page = 9;
    uint8_t crc_position_in_page = 41;

    if (page_size * 2 + metadata_length < hex_file_line_length)
      step_in_file = page_size * 2 + metadata_length;

    char buffer[E2PROM::eeprom_page_size];
    memset(buffer, 0, E2PROM::eeprom_page_size);

    for (uint8_t j = 0; j < E2PROM::eeprom_page_size - 1; ++j) {
      buffer[j] = pgm_read_byte(
          &blink_hex[position_in_file + payload_position_in_page + j]);
      // e.writeByte(current_page*E2PROM::eeprom_page_size+j, buffer[j]);
    }
    uint8_t crc_hi_nibble =
        pgm_read_byte(&blink_hex[position_in_file + crc_position_in_page]);
    e.writeByte(current_page * E2PROM::eeprom_page_size +
                    E2PROM::eeprom_page_size,
                crc_hi_nibble);
    uint8_t crc_lo_nibble =
        pgm_read_byte(&blink_hex[position_in_file + crc_position_in_page + 1]);
    e.writeByte(current_page * E2PROM::eeprom_page_size +
                    E2PROM::eeprom_page_size,
                crc_lo_nibble);
  }
  Serial.println(">>> end i2c write");
}

void IntelHex::write_vector(uint8_t eeprom_i2c_address,
                            uint16_t destination_start_byte, uint8_t *vector,
                            uint8_t size_in_bytes) {
  E2PROM e(0x50);
#ifdef _DEBUG_
  Serial.print("Writing to ");
  Serial.print(eeprom_i2c_address, HEX);
  Serial.print(":   @");
  Serial.print(destination_start_byte);
  Serial.print("    :");
#endif
  for (uint8_t i = 0; i < size_in_bytes; ++i) {
    e.writeByte(destination_start_byte + i, vector[i]);
#ifdef _DEBUG_
    Serial.print("    ");
    Serial.print(vector[i], HEX);
#endif
  }
}

void IntelHex::write_variable(uint8_t eeprom_i2c_address,
                              uint16_t destination_start_byte, uint8_t *data,
                              uint8_t size_in_bytes) {
  E2PROM e(0x50);
#ifdef _DEBUG_
  Serial.print(":Writing to ");
  Serial.print(eeprom_i2c_address, HEX);
  Serial.print(":   @");
  Serial.print(destination_start_byte);
  Serial.print("    :");
#endif
  uint8_t j = 0;
  for (int8_t i = size_in_bytes - 1; i >= 0; --i, ++j) {
    e.writeByte(destination_start_byte + j, data[i]);
#ifdef _DEBUG_
    Serial.print("    ");
    Serial.print(data[i], HEX);
#endif
  }
}

uint32_t IntelHex::GetValueFromSerial(char *message, uint8_t base) const {
  String inString = "";
  uint32_t result = 0;
  bool ready = false;

  Serial.print(message);
  do {
    while (Serial.available() > 0) {
      uint8_t inChar = Serial.read();
      if (10 == base && isDigit(inChar)) {
        inString += static_cast<char>(inChar);
      } else if (16 == base && isHexadecimalDigit(inChar)) {
        inString += static_cast<char>(inChar);
      }
      if ('\n' == inChar) {
        if (10 == base)
          result = inString.toInt();
        else if (16 == base)
          result = strtol(inString.c_str(), 0, 16);
        ready = true;
      }
    }
  } while (!ready);
  return result;
}
void IntelHex::write_preamble(uint8_t eeprom_i2c_address,
                              uint16_t destination_start_byte) {
  uint8_t preamble[] = "miniboot";
  uint8_t base10 = 10, base16 = 16;
  write_vector(eeprom_i2c_address, destination_start_byte, preamble,
               sizeof(preamble) / sizeof(preamble[0]) - 1);

  uint8_t application_name[] = "blink\xFF\xFF\xFF\xFF\xFF";
  write_vector(eeprom_i2c_address, destination_start_byte + 8, application_name,
               sizeof(application_name) / sizeof(application_name[0]) - 1);

  uint32_t application_timestamp =
      GetValueFromSerial("Application unix timestamp (decimal): ", base10);
  write_variable(eeprom_i2c_address, destination_start_byte + 18,
                 reinterpret_cast<uint8_t *>(&application_timestamp),
                 sizeof(application_timestamp));

  uint32_t current_time =
      GetValueFromSerial("Current unix timestamp (decimal): ", base10);
  write_variable(eeprom_i2c_address, destination_start_byte + 22,
                 reinterpret_cast<uint8_t *>(&current_time),
                 sizeof(current_time));

  uint32_t crc = GetValueFromSerial("CRC32 (hex): ", base16);
  write_variable(eeprom_i2c_address, destination_start_byte + 26,
                 reinterpret_cast<uint8_t *>(&crc), sizeof(crc));

  last_written_file_length_ = static_cast<uint16_t>(
      GetValueFromSerial("Application size (bytes, decimal): ", base10));
  write_variable(eeprom_i2c_address, destination_start_byte + 30,
                 reinterpret_cast<uint8_t *>(&last_written_file_length_),
                 sizeof(last_written_file_length_));
}

void IntelHex::write_to_eeprom_i2c(uint8_t eeprom_i2c_address,
                                   uint16_t destination_start_byte) {

  Serial.println("-- Miniboot EEPROM uploader --");
  Serial.println("");

  write_preamble(eeprom_i2c_address, destination_start_byte);

  Serial.println("");
  Serial.print("Header written. Please switch to binary mode and send a file "
               "of byte length ");
  Serial.print(last_written_file_length_);
  Serial.println(".");
  E2PROM e(eeprom_i2c_address);

  uint16_t pos = 32 + destination_start_byte;

  do {
    if (Serial.available() > 0) {
      char incomingByte = Serial.read(); // read the incoming byte:
      e.writeByte(pos++, incomingByte);
    }

  } while (1);
}
