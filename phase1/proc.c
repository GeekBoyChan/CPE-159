// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use syscalls

#include "include.h"
#include "data.h"
#include "proc.h"

void InitProc(void) {
   int i;
   unsigned short *p;

   point p to 0xb8000; // upper-left corner of display

   while(1) {
      show the dot
      wait for half of LOOP: loop on asm("inb $0x80");

      erase above writing
      wait for half of LOOP: loop on asm("inb $0x80");
   }
}

void UserProc(void) {
   int i;
   unsigned short *p;

   while(1) {
      point p to (0xb8000 + offset according to its PID)
      show 1st digit of its PID
      move p to next column
      show 2nd digit of its PID
      wait for half of LOOP: loop on asm("inb $0x80");

      erase above writing
      wait for half of LOOP: loop on asm("inb $0x80");
   }
}
