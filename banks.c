/*! \file banks.c
 * \brief Memory banks for the ESP32.
 */

#include <stdint.h>

#include "banks.h"

struct bank banks[]={
  //Only data memories for now.
  {.adr=0x3ff80000, .name="RTC Fast Memory"},
  {.adr=0x3ff90000, .name="Internal ROM 1"},
  {.adr=0x3ffAE000, .name="Internal SRAM 2"},
  {.adr=0x3ffE0000, .name="Internal SRAM 1"},

  /* Code memories in the 0x4000 page requires word
   * sized fetches.  Byte fetches fail.
   */
  {.adr=0x40000000, .name="Internal ROM 0r"},
  {.adr=0x40008000, .name="Internal ROM 0"},
  {.adr=0x40070000, .name="Internal SRAM 0"},
  

  //Others
  {.adr=0x50000000, .name="RTC Slow Memory"},
  {.adr=0x60000000, .name="Unknown 6"},
  {.adr=0x70000000, .name="Unknown 7"},
  

  //End on a null terminator.
  {.name=0, .adr=0}
};


