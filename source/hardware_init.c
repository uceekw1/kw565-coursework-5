/*
Assignment 5 for Embedded System for IoT (4B25)
A UCL & Cambridge student
Date: 8/1/2022

hardware_init function implementation is in this file.
 */

#include "board.h"
#include "pin_mux.h"
#include "fsl_clock_manager.h"

void hardware_init(void) {

  /* enable clock for PORTs */
  CLOCK_SYS_EnablePortClock(PORTA_IDX);
  CLOCK_SYS_EnablePortClock(PORTB_IDX);

  // configure i2c pins
  configure_i2c_pins(0u);

  // Configure SPI pins
  configure_spi_pins(0);
}
