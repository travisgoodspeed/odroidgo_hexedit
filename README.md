Howdy y'all,

This is a fun little hex editor for the ODROID GO, an ESP32
development kit available cheaply from Hardkernel.  You should
probably write your own rather than using mine, but I'm publishing the
code in case a student finds it handy.

I wrote it to learn the ESP32's memory layout, but it handily taught
me that the ESP32 only allows word-sized fetches from code memory, so
`((char*) 0x40000000)[0]` will crash, but `memcpy()` from the same
source address works fine.

Left and right choose the nybble to change, while up and down move
that nybble.  Alignment is not enforced.  The menu button will move up
one memory region, while the volume button moves down one region.
Other buttons are not yet assigned.

To install this application, first build the Hello World example in
the Arduino IDE from the ODROID GO Wiki, then load
`odroidgo_hexedit.ino` and flash it in the same way.



Memory regions are defined in `banks.c`.  Just add a new row for your
own region.

```C
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
```


73 from Knoxville,

--Travis
