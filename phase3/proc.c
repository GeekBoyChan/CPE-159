// proc.c, 159
// all processes are coded here
// processes do not access kernel data, must use syscalls

// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 2

#include "constants.h" // include only these 2 files
#include "syscalls.h"
#include "data.h"

void InitProc(void) {
   int i;
   int car_sem = SemInit(3);

   while(1) {
      SetVideo(1, 1);         // pos video
      Write(STDOUT, ".");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU

      SetVideo(1, 1);         // pos video
      Write(STDOUT, " ");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU
   }
}

void UserProc(void) 
{
   int my_pid;
   char str[3];

    my_pid = GetPid(); //get my PID and make a string from it (null-delimited)
    str[0] = '0' + my_pid/10; //print the first digit of mypid
    str[1] = '0' + my_pid%10; //print the second digit of mypid
    str[2] = '\0'; //null delimited

   SetVideo(my_pid+1,1); //set video cursor to beginning of my row
   Write(STDOUT, "write out that extra long msg to test line wrapped and erasure YEEEEEEEEEEEEEEEEEEEEE HAAAAAAAAAAAAAAAAAAAA"); //write out that extra long msg to test line wrapped and erasure
   Sleep(2); //sleep for 2 seconds

   while(1) 
   {
      SetVideo(my_pid+1,1);//call service to set video cursor to beginning of my row
      Write(STDOUT, str); //call service to write out my PID str
      Sleep(2); //call service to sleep for 2 seconds

      SetVideo(my_pid+1,1); //call service to set video cursor to beginning of my row
      Write(STDOUT, "--"); //call service to erase my PID str (with "--")
      Sleep(2); //call service to sleep for 2 seconds
   }
}

void CarProc(int car_sem)
{
	int my_pid = GetPid();
	char str[3];
    	str[0] = '0' + my_pid/10; //print the first digit of mypid
    	str[1] = '0' + my_pid%10; //print the second digit of mypid
    	str[2] = '\0'; //null delimited

	while(1)
	{
		SetVideo(my_pid+1,10);
		Write(STDOUT,"I'm off              ");
		Sleep(2);
		SemWait(car_sem);
		SetVideo(my_pid +1, 10);
		Write(STDOUT, "I'm on the bridge!");
		Sleep(2);
		SemPost(car_sem);
	}
}

