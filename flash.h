#pragma once

#include <avr/boot.h>
#include <avr/wdt.h>

#include "bootloader.h"

static inline void eraseApplication() {
  uint16_t ptr = BOOTLOADER_START_ADDRESS;
  do {
    ptr -= SPM_PAGESIZE;
    boot_page_erase(ptr);
    boot_spm_busy_wait();
  } while (ptr);
  wdt_reset();
}

static inline void erasePage(const uint16_t address) {
  boot_page_erase(address);
  boot_spm_busy_wait();
}

static inline void writeToPageBuffer(const uint16_t address, uint8_t *data) {
  erasePage(address);
  for (uint8_t i = 0; i < SPM_PAGESIZE; i += 2) {
    uint16_t w = *data++;
    w += (*data++) << 8;
    boot_page_fill(address + i, w);
  }
}

static inline void writePageBufferToFlash(const uint16_t address) {
  boot_page_write(address);
  boot_spm_busy_wait();
  boot_rww_enable();
}
