#pragma once

#include <avr/wdt.h>
#include "io.h"

static void initIO(){
  LED_INIT();
}

static void disableWatchdog(){
  wdt_reset();
  MCUSR=0;
  WDTCSR|= (1<<WDCE) | (1<<WDE);
  WDTCSR=0;
  asm("cli");
}

static void init()
{
  disableWatchdog();
  initIO();
}
