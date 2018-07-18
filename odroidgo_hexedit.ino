/* Howdy y'all!
 *  
 * This is a handy little hex editor for the ODROID GO, an ESP32
 * devkit that is shaped like a Gameboy.  It lets you explore internal
 * memory, to get a feel for the memory map and what might go where.
 */

#include <odroid_go.h>

#include <cstring>

#include "banks.h"

//! Target of the hex editor.
uint32_t target=banks[0].adr;
//! Target bank.
uint32_t banki=0;

//! Selected nybble of the address.
uint32_t targetnyb=2;

//! Number of lines to draw;
#define LINES 16


//! Throwaway string buffer.
#define SBUFLEN 1024
char sbuf[SBUFLEN];

//! Draws one line of the hex.
void drawhexline(uint32_t adr){
  int pos=0;

  /* The ESP32 can't fetch with byte-alignment from word addreses, so
   * we precopy a buffer and render from there.
   */
  uint8_t bytebuf[16];
  memcpy(bytebuf,(void*) adr, 16);
  
  //Wipe the old character buffer.
  memset(sbuf,SBUFLEN,0);

  //Populate the bytes.
  for(int i=0; i<16; i++){
    snprintf(sbuf+pos, SBUFLEN-pos,
             "%02x   ", bytebuf[i]);
    switch(i&0x7){
    case 0x7:
      pos+=4;
      break;
    case 0x3:
      pos+=3;
      break;
    default:
      pos+=2;
      break;
    }
  }

  //Populate the ASCII.
  pos--;
  for(int i=0; i<16; i++){
    /* We can do character fetches from bytebuf[]
     * but not from ((char*) adr)[] when the address
     * is in the code memory.
     */
    char c=bytebuf[i];
    
    if(c<0x20 || c>0x7f)
      c='.';
    sbuf[pos++]=c;
  }
  sbuf[pos++]='\0';

  //Draw the string.
  GO.lcd.print(sbuf);
  GO.lcd.print("\n");
}

//! Draws just the address.  We'll redraw this on each frame.
void drawtarget(uint32_t adr){
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextSize(2);
  snprintf(sbuf,SBUFLEN,
           "%08x: %s\n", adr, banks[banki].name);
  GO.lcd.setTextColor(RED);
  GO.lcd.print(sbuf);

  //Highlight the active nybble.
  GO.lcd.setTextColor(GREEN);
  GO.lcd.setCursor(12*(7-targetnyb), 2);
  GO.lcd.print("_");

  GO.lcd.setTextColor(WHITE);
  GO.lcd.setCursor(0, 30);
}

//! Draws a full screen of the hex contents.
void drawhexpage(uint32_t adr){
  //Draw the title.
  GO.lcd.fillScreen(BLACK);
  GO.lcd.clear();
  drawtarget(adr);

  //Draw each line.
  GO.lcd.setTextSize(1);
  for(int i=0; i<LINES; i++){
    drawhexline(adr+16*i);
  }
}

//! Initial setup code.
void setup() {
  //Begin the ODROID GO library.
  GO.begin();

  //Play a little jig at startup so we can recognize a crash.
  GO.Speaker.setVolume(3);
  GO.Speaker.playMusic(m5stack_startup_music, 25000);
}

//! Jumps to the next bank of memory.
void nextbank(){
  //Reset to zero if we pass the end.
  if(!banks[++banki].name)
    banki=0;
  target=banks[banki].adr;
}
//! Jumps to the previous bank of memory.
void prevbank(){
  //Reset to end if we pass the beginning.
  if(!banki){
    //Advance to the null terminator.
    while(banks[++banki].name);
  }
  //Step back one.
  banki--;
  target=banks[banki].adr;
}

//! Main application loop.
void loop() {
  //Local copy, so we know if we need to redraw in the loop.
  static uint32_t sadr = ~target;
  static int redraw=0;
  
  // Redraw on a change.
  if(sadr!=target || redraw){
    drawhexpage(sadr=target);
    redraw=0;
  }

  
  // Handle Input
  GO.update();
  switch(GO.JOY_X.isAxisPressed()){
  case 2:
    targetnyb++;
    redraw++;
    break;
  case 1:
    targetnyb--;
    redraw++;
    break;
  }
  switch(GO.JOY_Y.isAxisPressed()){
  case 2:
    target+=1<<(targetnyb*4);
    redraw++;
    break;
  case 1:
    target-=1<<(targetnyb*4);
    redraw++;
    break;
  }
  if(GO.BtnMenu.wasPressed()){  //Menu jumps to the next bank.
    nextbank();
    redraw++;
  }
  if(GO.BtnVolume.wasPressed()){
    prevbank();
    redraw++;
  }

  //We don't use these buttons yet.
  if(GO.BtnSelect.wasPressed()){
    
  }
  if(GO.BtnStart.wasPressed()){
    
  }
  if(GO.BtnA.wasPressed()){
    
  }
  if(GO.BtnB.wasPressed()){
    
  }
  
  delay(100);
}

