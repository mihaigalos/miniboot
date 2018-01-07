#pragma once

#include "Arduino.h"
#include <stdint.h>

class IntelHex{
public:
  void parse_intel_hex_from_flash();
};