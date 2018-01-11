#pragma once

#include <avr/boot.h>
#include <avr/wdt.h>

static inline void eraseFlash() {
  uint16_t ptr = BOOTLOADER_START_ADDRESS;
  do {
    ptr -= SPM_PAGESIZE;
    boot_page_erase(ptr);
    boot_spm_busy_wait();
  }while (--ptr);
  wdt_reset();
}

static inline void writeWordToPageBuffer(uint16_t address, uint16_t data) {
  boot_page_fill(address, data);
}

static inline void writePageBufferToFlash(uint16_t address) {
  boot_page_write_safe(address);
}
