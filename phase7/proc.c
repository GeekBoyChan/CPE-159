// proc.c, 159
// all processes are coded here
// processes do not access kernel data, must use syscalls

// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 7

#include "constants.h"        	
#include "syscalls.h"
#include "data.h"
#include "types.h"
#include "lib.h"

void InitProc(void) {
   int i;
   car_sem = SemInit(3);

   while(1) {
      SetVideo(1, 1);         	// pos video
      Write(STDOUT, ".");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU

      SetVideo(1, 1);         	// pos video
      Write(STDOUT, " ");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU
   }
}

void UserProc(void) 
{
   int my_pid;
   char str[3];

    my_pid = GetPid();           //get my PID and make a string from it (null-delimited)
    str[0] = '0' + my_pid/10;    //print the first digit of mypid
    str[1] = '0' + my_pid%10;    //print the second digit of mypid
    str[2] = '\0';               //null delimited

   SetVideo(my_pid+1,1);         //set video cursor to beginning of my row
   Write(STDOUT, "write out that extra long msg to test line wrapped and erasure YEEEEEEEEEEEEEEEEEEEEE HAAAAAAAAAAAAAAAAAAAA"); //write out that extra long msg to test line wrapped and erasure
   Sleep(2);                     //sleep for 2 seconds

   while(1) 
   {
      SetVideo(my_pid+1,1);   	//call service to set video cursor to beginning of my row
      Write(STDOUT, str); //call service to write out my PID str
      Sleep(2); //call service to sleep for 2 seconds

      SetVideo(my_pid+1,1); 	//call service to set video cursor to beginning of my row
      Write(STDOUT, "--"); 	//call service to erase my PID str (with "--")
      Sleep(2); 		//call service to sleep for 2 seconds
   }
}

void CarProc(void)
{
	int my_pid = GetPid();
	char str[3];
    	str[0] = '0' + my_pid/10; 	//print the first digit of mypid
    	str[1] = '0' + my_pid%10; 	//print the second digit of mypid
    	str[2] = '\0'; 			//null delimited

	while(1)
	{
		SetVideo(my_pid+1,1);
		Write(STDOUT, str); 	//call service to write out my PID str
		SetVideo(my_pid+1,10);
		Write(STDOUT,"I'm off              ");
		Sleep(2);
		
		SemWait(car_sem);
		SetVideo(my_pid+1,10);
		Write(STDOUT, "I'm on the bridge!");
		Sleep(2);
		SemPost(car_sem);
	}
}

void Ouch(void)
{
	int  my_pid, device;
	
	my_pid = GetPid();
	
	device = my_pid % 2;
	if (device == 0)
		device = TERM0;
	if (device == 1)
		device = TERM1;
	
	Write(device, "Ouch, don't touch that! \n\r");
}

void ChildCode(void) 
{  // child proc runs this
      int my_pid, p_pid, device;
      char str[3];
  
      my_pid = GetPid();		//1. get my PID
      
      p_pid = GetPpid();		//2. get parent's PID using new syscall
      
      					//3. build a string based on my PID
      str[0] = '0' + my_pid/10; 	//print the first digit of mypid
      str[1] = '0' + my_pid%10; 	//print the second digit of mypid
      str[2] = ' ';
      str[3] = '\0';
      
      device = p_pid % 2; 		// if 0 TERM0, if 1 TERM1
	if (device == 0)
		device = TERM0;
	if (device == 1)
		device = TERM1;
		
      while(1)				//4. loop forever:
      {
      		Write(device, "I am child pid: ");	//a. show the msg (see demo) to the same terminal as the parent
	      	Write(device, str);
	      	Write(device, "\n\r");
	      	Sleep(3);				//b. and sleep for 3 seconds
      }
}

void TermProc(void)
{
	int my_pid, device, comp, c_pid;
	char str[3];
	char buff[BUFF_SIZE];
	
	Signal(SIGINT,Ouch);
	
	my_pid = GetPid();

    	str[0] = '0' + my_pid/10; 	//print the first digit of mypid
    	str[1] = '0' + my_pid%10; 	//print the second digit of mypid
	str[2] = ':';
    	str[3] = '\0';
	
	device = my_pid % 2; 		// if 0 TERM0, if 1 TERM1
	if (device == 0)
		device = TERM0;
	if (device == 1)
		device = TERM1;
	
	while(1)
	{
		Sleep(1);
		Write(device, str);		// Print
		Write(device,"Enter: ");	// Prompt to enter characters
		Read(device, buff);		// Read what was entered
		Write(device,"\n\rEntered: ");
		Write(device, buff);		// Print what was entered
		Write(device, "\n\r");
		
		comp = StrCmp(buff,"fork");
		
		if(comp == 1)
		{
			c_pid = Fork();
			if (c_pid == -1)
				Write(device, "Error message");
			if (c_pid == 0)
				ChildCode();
			
		}
		
		
	}
}

void Wrapper(func_p_t handler_p)
{
	asm("pushal");                    // save regs
	handler_p();                      // call user's signal handler
	asm("popal");                     // restore regs
	asm("movl %%ebp, %%esp; popl %%ebp; ret $4"::); // skip Ouch addr
}
	
