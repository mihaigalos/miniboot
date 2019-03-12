#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>

#define TESTING
#define SPM_PAGESIZE 128

#include "../src/io.h"
#undef LET_INIT
#undef LET_ON
#undef LET_OFF
#undef LET_TOGGLE

#define LED_INIT()
#define LED_ON()
#define LED_OFF()
#define LED_TOGGLE()

#include "../src/bootloader.h"
#include "../src/miniboot.h"

void writeToPageBuffer(const uint16_t address, uint8_t *data) {}
void writePageBufferToFlash(const uint16_t address) {}

class BinFileReader{
public:
  BinFileReader(std::string filename) {
    file_.reset(new std::ifstream{filename, std::ios::binary});
  }
  uint16_t GetWord(){
    char buffer[sizeof(uint16_t)];
    memset(buffer, 0, sizeof(buffer));

    file_->read(buffer, sizeof(buffer));

    positon_+=sizeof(buffer);

    uint16_t result = static_cast<uint16_t>(buffer[0])<<8;
    result |= static_cast<uint16_t>(static_cast<uint8_t>(buffer[1]));

    return result;
  }
private:
  std::unique_ptr<std::ifstream> file_{nullptr};
  uint16_t positon_{0};
};


uint16_t getWordFromSource(const uint8_t i2c_address,
                                         const uint16_t data_address){
  static BinFileReader bin_file_reader{"Slave_bootloaderMiniboot.bin"};
  return bin_file_reader.GetWord();
}



#include "../src/kernel.h"

uint16_t Miniboot::getDataStartAddressInSource(const uint8_t i2c_address){return 0;}
uint16_t Miniboot::getDataLength(const uint8_t i2c_address){return 0;}

class MinibootMock : public Miniboot{
public:
  uint16_t getDataStartAddressInSource(const uint8_t i2c_address) { return 0;}
  uint16_t getDataLength(const uint8_t i2c_address) { return 22*1024;}
};



int main(){
  MinibootMock miniboot_mock;
  uint16_t application_start{0};
  miniboot_mock.writeFlashFromI2C(42, application_start);

  std::cout << "application_start: " << std::hex<< application_start << std::endl;
}
