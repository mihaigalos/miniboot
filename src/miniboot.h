#ifndef MINIBOOT_H_
#define MINIBOOT_H_

#ifdef TESTING
#define METHOD_PREFIX virtual
#else
#define METHOD_PREFIX static inline
#endif


class Miniboot{
public:

  static inline bool isCrcOk(const uint8_t i2c_address);
  METHOD_PREFIX void writeToFlash(const uint16_t address, uint8_t *data,
                                  uint16_t &application_start);
  METHOD_PREFIX void writeFlashFromI2C(const uint8_t i2c_address,
                                       uint16_t &application_start);

  [[ noreturn ]]
  static inline void leaveBootloader(uint16_t &application_start) {
    // hold my beer and watch this!
    reinterpret_cast<void (*)(void)>(application_start)();
    while (1)
      ;
  }

  static inline bool isReflashNecessary(uint32_t &i2c_application_timestamp);
private :
  METHOD_PREFIX uint16_t getDataStartAddressInSource(const uint8_t i2c_address);
  METHOD_PREFIX uint16_t getDataLength(const uint8_t i2c_address);

};

#endif // MINIBOOT_H_
