#pragma once

#include "i2c_master.h"
#include "avr-bootloader-common/init.h"

static inline void init()
{
    initCommon();
    i2c_init();
}