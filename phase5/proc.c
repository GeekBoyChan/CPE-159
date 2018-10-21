// proc.c, 159
// all processes are coded here
// processes do not access kernel data, must use syscalls

// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 5

#include "constants.h"        	// include only these 2 files
#include "syscalls.h"
#include "data.h"

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

void TermProc(void)
{
	int my_pid, device, i;
	char str[3];
	char buff[BUFF_SIZE];
	//i = 0;
	
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
		i = 0;
		Write(device, str);
		Write(device,"Enter: ");
		Read(device, buff);
		Write(device,"\n\rEntered: ");
		Write(device, buff);
		Write(device, "\n\r");
		while(buff[i] != '\0') {
			buff[i] = '\0';
			i++;
		}
		

		/*if(i == 5) 			//Every 5 lines write() to my device some special symbols
		{
			i = 0;
			SetVideo(my_pid+1,10);
			Write(device,"----->\n\r");
			
		}
		if (i != 5)
		{
			i++;
			Write(device, str); 	//Write 'str' to my device
			SetVideo(my_pid+1,10); 	
			Write(device," A lengthier message to my device to show that it works\n\r");
			Sleep(3);	
		}*/
		
	}
}
