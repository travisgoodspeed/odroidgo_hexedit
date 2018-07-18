/*! \file banks.h
 * \brief Banks of memory in the ESP32.
 */



struct bank {
  uint32_t adr;
  uint32_t len;
  char *name;
};


extern struct bank banks[];

