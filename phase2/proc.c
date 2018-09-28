// proc.c, 159
// all processes are coded here
// processes do not access kernel data, must use syscalls

// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 2

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

    my_pid = GetPid; //get my PID and make a string from it (null-delimited)

   SetVideo(1,1); //set video cursor to beginning of my row
   Write(STDOUT, "write out that extra long msg to test line wrapped and erasure YEEEEEEEEEEEEEEEEEEEEE HAAAAAAAAAAAAAAAAAAAA"); //write out that extra long msg to test line wrapped and erasure
   Sleep(2) //sleep for 2 seconds

   while(1) {
      SetVideo(1,1);//call service to set video cursor to beginning of my row
      Write(STDOUT, my_pid); //call service to write out my PID str
      Sleep(2); //call service to sleep for 2 seconds

      SetVideo(1,1); //call service to set video cursor to beginning of my row
      Write(STDOUT, "--"); //call service to erase my PID str (with "--")
      Sleep(2); //call service to sleep for 2 seconds
   }
}
