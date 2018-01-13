#pragma once

#include <avr/wdt.h>
#include "io.h"
#include "I2C-master-lib/i2c_master.h"

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
  i2c_init();
  initIO();
}
