#pragma once

#include "io.h"
#include <avr/wdt.h>

static inline void initIO() { LED_INIT(); }

static inline void disableWatchdog()
{
  wdt_reset();
  MCUSR = 0;
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = 0;
  asm("cli");
}

static inline void initCommon()
{
  disableWatchdog();
  initIO();
}
