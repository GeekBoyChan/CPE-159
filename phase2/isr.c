// isr.c, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 2

#include "include.h"
#include "types.h"
#include "data.h"
#include "isr.h"
#include "lib.h"
#include "proc.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to ready_q

void NewProcISR(func_p_t p) 
{  // arg: where process code starts
	int pid;

	if(avail_q.size == 0 )
	{    // may occur if too many been created
		cons_printf("Kernel panic: no more process!\n");
        	return;        // cannot continue, alternative: breakpoint();
   	}
	
   	pid = DeQ(&avail_q);                 // alloc PID (1st is 0)
   	Bzero((char *) &pcb[pid], sizeof(pcb_t));       // clear PCB
   	Bzero((char *) &stack[pid][0] , STACK_SIZE);  // clear stack
   	pcb[pid].state = READY;              // change process state
	
  //if(pid)
	EnQ(pid, &ready_q);              // queue it
	
// point TF_p to stack & fill it out

   pcb[pid].TF_p = (TF_t *) &stack[pid][STACK_SIZE - sizeof(TF_t)];                       
   pcb[pid].TF_p--;
   pcb[pid].TF_p->efl = EF_DEFAULT_VALUE|EF_INTR; // enables intr
   pcb[pid].TF_p->cs = get_cs();                  // duplicate from CPU
   pcb[pid].TF_p->eip =(unsigned int) p;                          // set to code
}

// count run time and switch if hitting time limit


void TimerISR(void) 
{
   	outportb(PIC_CONTROL, DONE);              // notify PIC getting done

   	pcb[cur_pid].time++;                      // count up time
   	pcb[cur_pid].life++;                      // count up life
	sys_ticks++;			// upcount current os uptime

   	if(pcb[cur_pid].time == TIME_MAX)
	{
		EnQ(cur_pid, &ready_q);  //append current pid to ready queue
		//cur_pid = -1;                //reset current pid to -1
		pcb[cur_pid].state = READY;  //change state to ready
	
		cur_pid = -1;                //reset current pid to -1
  	}

	int i;
	for(i=0; i<PROC_MAX; i++) // Loop thru entire PCB array
	{
		//If process is asleep and wake time has arrived
		if((pcb[i].state==SLEEPY)&&(sys_ticks==pcb[i].wake_time)) 
		{
			EnQ(i, &ready_q); // EnQ process I to rdy_q(?)
			pcb[i].state = READY;
		}
	}
			
}

void GetPidISR(void)
{
        pcb[cur_pid].TF_p->ebx = cur_pid; //What is this I dont even
}

void SleepISR(void)
{
        int sleepSec = pcb[cur_pid].ebx                    //I dont know
        pcb[cur_pid].wake_time =  sys_ticks + sleepSec * 100; //What is sleep second
        pcb[cur_pid].state = SLEEPY;         //change state to SLEEPY
        cur_pid = -1;                        //reset cur_pid
}

void SetVideo(void)
{
	unsigned short row, col;
	row = unsigned short(pcb[cur_pid].ebx);
	col = unsigned short(pcb[cur_pid].ecx);

	video_p = HOME_POS + (row-1) * 80 + (col-1);
}

void WriteISR(void)
{
	int device = pcb[cur_pid].ebx;
	int *str = pcb[cur_pid].ecx;
	int i,j;

	if(device == STDOUT)
	{
		for(i=0;i<size_of(str);i++) //Not sure about size_of
		{ 
			if(video_p == END_POS)  // Reach end, return 
				video_p = HOME_POS;
			
			if(video_p == HOME_POS) //FIX,Clear if at start of line
				for(j=0;j<80;j++)
					Write out ' ',
			
			if(str[i] != '\n') //if end of string
			{
				use video_p to write out 'c'
				video_p++;
			}
			else //move video_p to start of next line
			{
				find 'col pos' of current video_p
				the 'rest' = 80 - current 'col pos'		
				incr video_p by 'rest'
			}
		}
	}


