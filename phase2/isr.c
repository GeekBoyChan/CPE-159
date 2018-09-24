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


void TimerISR(void) {
   	outportb(PIC_CONTROL, DONE);              // notify PIC getting done

   	pcb[cur_pid].time++;                      // count up time
   	pcb[cur_pid].life++;                      // count up life

   	if(pcb[cur_pid].time == TIME_MAX)
	{
		EnQ(cur_pid, &ready_q);  //append current pid to ready queue
		//cur_pid = -1;                //reset current pid to -1
		pcb[cur_pid].state = READY;  //change state to ready
	
		cur_pid = -1;                //reset current pid to -1
  }
}
