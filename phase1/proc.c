// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use syscalls

#include "include.h"
#include "data.h"
#include "proc.h"

void InitProc(void) {
   int i;
   unsigned short *p = 0xb8000; // upper-left corner of display

   while(1) 
   {
      *p = '.' + VGA_MASK;  //show the dot
      //wait for half of LOOP: loop on asm("inb $0x80");
      for(i=0; i<LOOP/2; i++)
      {
        asm("inb $0x80");
      }

      *p = ' ' + VGA_MASK; //erase above writing
      //wait for half of LOOP: loop on asm("inb $0x80");
      for(i=0; i<LOOP/2;i++)
      {
        asm("inb $0x80");
      }
   }
}

void UserProc(void) 
{
   int i;
   unsigned short *p = 0xb8000; // upper-left corner of display
   
   while(1) 
   {
      int offset = (cur_pid *160); //point p to (0xb8000 + offset according to its PID)
      p = (0xb8000 + offset);
      *p = ((cur_pid/10) +'0') + VGA_MASK;//show 1st digit of its PID
      p++; //move p to next column
      *p = ((cur_pid%10) + '0') + VGA_MASK; // show 2nd digit of its PID
      //wait for half of LOOP: loop on asm("inb $0x80");
      for(i=0;i<LOOP/2;i++)
      {
        asm("inb $0x80");
      }
      //erase above writing
      *p = ' ' + VGA_MASK; //Clears the 2nd digit
      p--;                //Move back a column
      *p = ' ' +VGA_MASK;  //Clears the 1st digit
      //wait for half of LOOP: loop on asm("inb $0x80");
      for(i=0;i<LOOP/2;i++)
      {
         asm("inb $0x80");
      }
   }
}
