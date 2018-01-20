#pragma once

#include "config.h"

#define CONCAT(a, b) a##b
#define OUTPORT(name) CONCAT(DDR, name)
#define LED_PORT(name) CONCAT(PORT, name)

#define LED_ON() LED_PORT(LED_PORT_NAME) &= ~(1 << (LED_PIN))
#define LED_OFF() LED_PORT(LED_PORT_NAME) |= (1 << (LED_PIN))
#define LED_TOGGLE() LED_PORT(LED_PORT_NAME) ^= (1 << (LED_PIN))

#define LED_INIT()                                                             \
  LED_OFF();                                                                   \
  OUTPORT(LED_PORT_NAME) = (1 << (LED_PIN))

#define RESET_VECTOR 0x0000
#define RESET_VECTOR_ARGUMENT_ADDRESS 0x0002

static constexpr uint16_t jmp_instruction = 0x940c;
