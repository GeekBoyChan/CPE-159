// proc.c, 159
// all processes are coded here
// processes do not access kernel data, must use syscalls

#include "constants.h" // include only these 2 files
#include "syscalls.h"

void InitProc(void) {
   int i;

   while(1) {
      SetVideo(1, 1);         // pos video
      Write(STDOUT, ".");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU

      SetVideo(1, 1);         // pos video
      Write(STDOUT, " ");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU
   }
}

void UserProc(void) {
   int my_pid;
   char str[3];

   get my PID and make a string from it (null-delimited)

   set video cursor to beginning of my row
   write out that extra long msg to test line wrapped and erasure
   sleep for 2 seconds

   while(1) {
      call service to set video cursor to beginning of my row
      call service to write out my PID str
      call service to sleep for 2 seconds

      call service to set video cursor to beginning of my row
      call service to erase my PID str (with "--")
      call service to sleep for 2 seconds
   }
}
