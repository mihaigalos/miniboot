#pragma once

#include <avr/boot.h>
#include <avr/wdt.h>

static inline void eraseFlash() {
  while (uint16_t ptr = BOOTLOADER_START_ADDRESS) {
    ptr -= SPM_PAGESIZE;    
    boot_page_erase(ptr);
    boot_spm_busy_wait();
  }
}

static inline void writeWordToPageBuffer(uint16_t data, uint16_t address) {
  boot_page_fill(address, data);
}