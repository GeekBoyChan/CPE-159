// proc.c, 159
// all processes are coded here
// processes do not access kernel data, must use syscalls

// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 9

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

void ChildCode(int device) 
{  // child proc runs this
      int my_pid;
      char str[3];
  
      my_pid = GetPid();		//1. get my PID
      
      					//3. build a string based on my PID
      str[0] = '0' + my_pid/10; 	//print the first digit of mypid
      str[1] = '0' + my_pid%10; 	//print the second digit of mypid
      str[2] = ' ';
      str[3] = '\0';
	
	Write(device, "I am child pid: ");	//a. show the msg (see demo) to the same terminal as the parent
	Write(device, str);
      //... the same code except there's no loop in the end, just use these:	
      Write(device, "\n\r");
      Sleep(my_pid);
      Exit(my_pid * 5);			//// Exiting with a special exit number for parent to get
}

void ChldHandler(void)
{
	int my_pid, cpid, device;
	int ec;
	char str[3];
	
	cpid = Wait(&ec);			//1. issue Wait() call to get PID and exit code from the exiting child

	my_pid = GetPid();		//2. get my PID to build str and determine device to Write()

	device = my_pid % 2; 		// if 0 TERM0, if 1 TERM1
	if (device == 0)
		device = TERM0;
	if (device == 1)
		device = TERM1;
	
	Write(device, "my_pid ");
    	str[0] = '0' + my_pid/10; 	//print the first digit of mypid
    	str[1] = '0' + my_pid%10; 	//print the second digit of mypid
    	str[2] = '\0';
	Write(device, str);
	
	Write(device, ", cpid ");
    	str[0] = '0' + cpid/10; 	//print the first digit of mypid
    	str[1] = '0' + cpid%10; 	//print the second digit of mypid
    	str[2] = '\0';
	Write(device, str);
	
	Write(device, ", ec ");
    	str[0] = '0' + ec/10; 	//print the first digit of mypid
    	str[1] = '0' + ec%10; 	//print the second digit of mypid
    	str[2] = '\0';
	Write(device, str);
	
	Write(device, "\n\r");
	
	Signal(SIGCHLD, (func_p_t)0);
	
}

void TermProc(void)
{
	int my_pid, device, c_pid, fg;
	int ec;
	char str[3], frk[3], exc[3];
	char buff[BUFF_SIZE];
	
	Signal(SIGINT,Ouch);
	
	my_pid = GetPid();

    	str[0] = '0' + my_pid/10; 	//print the first digit of mypid
    	str[1] = '0' + my_pid%10; 	//print the second digit of mypid
    	str[2] = '\0';
	
	device = my_pid % 2; 		// if 0 TERM0, if 1 TERM1
	if (device == 0)
		device = TERM0;
	if (device == 1)
		device = TERM1;
	
	while(1)
	{
		Sleep(2);
		Write(device, str);		// Print
		Write(device,"Enter: ");	// Prompt to enter characters
		Read(device, buff);		// Read what was entered
		Write(device,"\n\rEntered: ");
		Write(device, buff);		// Print what was entered
		Write(device, "\n\r");
		
		if(StrCmp(buff, "fork"))
			fg = 1;
		else if(StrCmp(buff, "fork&"))
			fg = 0;
		else
			continue;
		if(fg == 0)
			Signal(SIGCHLD, ChldHandler);
		
		
		c_pid = Fork();
		frk[0] = '0' + c_pid/10; 	//print the first digit of mypid
    		frk[1] = '0' + c_pid%10; 	//print the second digit of mypid
    		frk[2] = '\0';
		
		switch(c_pid)
		{
			case -1:
				Write(device, "OS failed to fork! \n\r"); //call Write to write to terminal:
				break;
			case 0: 		//for child proc: call Exec() service arg device
				if( -1 == Exec(ChildCode, device))
				{
					Write(device, "OS failed to Exex()!\n\r");
					Exit(-1);
				}
				break;
			default:			// parent does this
				Sleep(my_pid * 2);	// TermProc PID 1 & 2, sleep 2 & 4 secs
				if(fg == 1)		//if foreground running:	
				{
					c_pid = Wait(&ec);	//1. call the new call Wait. Get child PID and exit code
					exc[0] = '0' + (int)ec/10; 	//print the first digit of mypid
    					exc[1] = '0' + (int)ec%10; 	//print the second digit of mypid
    					exc[2] = '\0';
					
					Write(device, "my_pid ");		//2. issue several Write() calls to print info from Wait()
					Write(device, str);
					Write(device, ", cpid ");
					Write(device, frk);
					Write(device, ", ");
					Write(device, "ec ");
					Write(device, exc);
					Write(device, "\n\r");
				}
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
	
