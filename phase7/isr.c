// isr.c, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 7

#include "include.h"
#include "types.h"
#include "data.h"
#include "isr.h"
#include "lib.h"
#include "proc.h"
#include "syscalls.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to ready_q

void NewProcISR(func_p_t p) 
{  // arg: where process code starts
	int pid;

	if(QisEmpty(&avail_q))
	{                                             // may occur if too many been created
		cons_printf("Kernel panic: no more process!\n");
        	return;                               // cannot continue, alternative: breakpoint();
   	}
	
   	pid = DeQ(&avail_q);                          // alloc PID (1st is 0)
   	Bzero((char *) &pcb[pid], sizeof(pcb_t));     // clear PCB
   	Bzero((char *) &stack[pid][0] , STACK_SIZE);  // clear stack
   	pcb[pid].state = READY;                       // change process state
	
	EnQ(pid, &ready_q);                           // queue it
	
// point TF_p to stack & fill it out

   pcb[pid].TF_p = (TF_t *) &stack[pid][STACK_SIZE - sizeof(TF_t)];                       
   pcb[pid].TF_p->efl = EF_DEFAULT_VALUE|EF_INTR; // enables intr
   pcb[pid].TF_p->cs = get_cs();                  // duplicate from CPU
   pcb[pid].TF_p->eip =(unsigned int) p;          // set to code
}

// count run time and switch if hitting time limit


void TimerISR(void) 
{
   	int i;
    outportb(PIC_CONTROL, DONE);                  // notify PIC getting done

   	pcb[cur_pid].time++;                      // count up time
   	pcb[cur_pid].life++;                      // count up life
	  sys_ticks++;			          // upcount current os uptime

   	if(pcb[cur_pid].time == TIME_MAX)
	  {
		EnQ(cur_pid, &ready_q);           //append current pid to ready queue
		pcb[cur_pid].state = READY;       //change state to ready
		cur_pid = -1;                     //reset current pid to -1
  	}

	
	for(i=0; i<PROC_MAX; i++)                  // Loop thru entire PCB array
	{
		//If process is asleep and wake time has arrived
		if((pcb[i].state==SLEEPY)&&(sys_ticks==pcb[i].wake_time)) 
		{
			EnQ(i, &ready_q);          // EnQ process I to rdy_q
			pcb[i].state = READY;
		}
	}
			
}

void GetPidISR(void)
{
        pcb[cur_pid].TF_p->ebx = cur_pid; 
}

void SleepISR(void)
{
        int sleepSec = pcb[cur_pid].TF_p->ebx;                   
        pcb[cur_pid].wake_time =  sys_ticks + sleepSec * 100; 
        pcb[cur_pid].state = SLEEPY;                //change state to SLEEPY
        cur_pid = -1;                               //reset cur_pid
}

void SetVideoISR(void)
{
	int row, col;
	row = (pcb[cur_pid].TF_p->ebx);
	col = (pcb[cur_pid].TF_p->ecx);

	video_p = (unsigned short*) (HOME_POS + (row-1) * 80 + (col-1));
}

void WriteISR(void)
{
	//int i;
	int device = pcb[cur_pid].TF_p->ebx;
	char *str = (char *) pcb[cur_pid].TF_p->ecx;
	//if(sizeof(*str)==0)
	if(*str == '\0')
	{
		return;
	}
	if(device == TERM0 || device == TERM1)
	{
		//set first character to 'io'
		outportb(term_if[device-35].io, *str);
		//set tx_p to second character in 'str'
		term_if[device-35].tx_p = &str[1];
		//block cur_pid to the tx_wait_q
		EnQ(cur_pid, &term_if[device-35].tx_wait_q);
		pcb[cur_pid].state = WAIT;
		cur_pid = -1;
	}
	if(device == STDOUT)
	{
		while(*str != '\0')
    		{ 
			if(video_p == END_POS)                    // Reach end, return
			{
				video_p = HOME_POS;
			}
			if((video_p - HOME_POS) % 80 == 0 )       //Clear if at start of line
			{
          		Bzero((char *) video_p,  160);            // Clear line with Bzero
			}
			if(*str != '\n')                          //if end of string
			{
				*video_p = *str + VGA_MASK;
				video_p++;
			}
						
      			else                                      //move video_p to start of next line
			{
				int colPos, rst;
				colPos = (video_p - HOME_POS)%80; //find 'col pos' of current video_p
				rst = 80 - colPos;		
				video_p = video_p + rst;
			}
      			str++;
			
		}
	}
}

void ReadISR(void)
{
	int device = pcb[cur_pid].TF_p->ebx;
	char *buff = (char *) pcb[cur_pid].TF_p->ecx;
	
	//set the RX pointer of the interface to 'buff'
	term_if[device-35].rx_p = buff;
	//"block' the current process to the RX wait queue of the interface
	EnQ(cur_pid, &term_if[device-35].rx_wait_q);
	pcb[cur_pid].state = WAIT;
	cur_pid = -1;
}

void SemInitISR(void)
{
	unsigned short *p;
	int sem_id, passes;
	sem_id = DeQ(&sem_q);
	if(sem_id == -1)
	{
		cons_printf("no more sems");
		breakpoint();
	}
	passes = pcb[cur_pid].TF_p->ebx;
	Bzero((char *) &sem[sem_id], sizeof(sem_t));
	sem[sem_id].passes = passes;
	p =HOME_POS + 21 * 80;
	*p = sem[sem_id].passes + '0' + VGA_MASK;
}

void SemWaitISR(void)
{
	unsigned short *p;
	int sem_id = pcb[cur_pid].TF_p->ebx;
	if(sem[sem_id].passes > 0)
	{
		sem[sem_id].passes--;
	}
	else
	{
		EnQ(cur_pid, &sem[sem_id].wait_q);
		pcb[cur_pid].state = WAIT;
		cur_pid = -1;
	}
	p = HOME_POS + 21 * 80;
	*p = sem[sem_id].passes + '0' + VGA_MASK;
}

void SemPostISR(void)
{
	unsigned short *p;
	int pid; 
	int sem_id = pcb[cur_pid].TF_p->ebx;
	if(QisEmpty(&sem[sem_id].wait_q))
	{
		sem[sem_id].passes++;
	}
	else
	{
		pid = DeQ(&sem[sem_id].wait_q);
		EnQ(pid, &ready_q);
		pcb[cur_pid].state = READY;
		
	}
	p = HOME_POS + 21 * 80;
	*p = sem[sem_id].passes + '0' + VGA_MASK;
}

void TermISR(int index)
{
	int event = inportb(term_if[index].io + IIR);
	if(event == IIR_TXRDY)
	{
		TermTxISR(index);
	}
	else if(event == IIR_RXRDY)
	{
		TermRxISR(index);
	}
	
	if(index == 0)
	{
		outportb(PIC_CONTROL, TERM0_DONE);
	}
	if(index == 1)
	{
		outportb(PIC_CONTROL, TERM1_DONE);
	}
}

void TermTxISR(int index)
{
	int pid;
	if(QisEmpty(&term_if[index].tx_wait_q))
	{
		return;
	}
	if(*term_if[index].tx_p != '\0')
	{
		outportb(term_if[index].io, *term_if[index].tx_p);
		term_if[index].tx_p++;
		return;
	}
	else
	{
		//Release waiting proc from tx_wait_q (3 steps)
		pid = DeQ(&term_if[index].tx_wait_q);
		EnQ(pid, &ready_q);
		pcb[pid].state = READY;
	}
}

void TermRxISR(int interface_num)
{
	int pid;
	//1. read the character in from the 'io' of the terminal interface
	char in = inportb(term_if[interface_num].io);
	//2. if the character is NOT '\n' or '\r' (not Enter or Return):
	if(!(in == '\n' || in == '\r' || in==3))
	{
		//2.a. write it back to the 'io' of the interface (echo)
		outportb(term_if[interface_num].io, in);
		//2.b. if the RX wait queue is not empty:
		if(!QisEmpty(&term_if[interface_num].rx_wait_q))
		{
			//2.b.1 using the RX pointer of the interface to append it to 'buff'
			*term_if[interface_num].rx_p = in;
			//2.b.2 advance the RX pointer
			term_if[interface_num].rx_p++;
		}
		//2.c return
		return;
	}
	if(in == 3)
	{
		//if the terminal RX wait queue is empty: just 'return;'
		if(QisEmpty(&term_if[interface_num].rx_wait_q) == 1)
		{
			return;
		}
		//release the 1st waiting process in the queue (3 steps)
		pid = DeQ(&term_if[interface_num].rx_wait_q);
		EnQ(pid, &ready_q);
		pcb[pid].state = READY;
		//delimited the process terminal input buffer with a null character
		*term_if[interface_num].rx_p = '\0';
		//if the process has a handler requested, call WrapperISR with its PID and handler address
		if(pcb[pid].sigint_handler_p != NULL)
		{
			WrapperISR(pid, pcb[pid].sigint_handler_p);	
		}
		return;
	}
	//3. if there is a waiting process in the RX wait queue of the interface:
	if(QisEmpty(&term_if[interface_num].rx_wait_q) == 0)
	{
		//3.a delimit 'buff' with a null character
		*term_if[interface_num].rx_p = '\0';
		//3.b release the waiting process
		pid = DeQ(&term_if[interface_num].rx_wait_q);
		EnQ(pid, &ready_q);
		pcb[pid].state = READY;
	}
	
}

void SignalISR(void)
{
	//register the handler address to the PCB
	pcb[cur_pid].sigint_handler_p = (func_p_t)pcb[cur_pid].TF_p->ecx; 
}
void WrapperISR(int pid, func_p_t handler_p)
{
	int *p1, *p2;
	TF_t tmp;
	p1 = &pcb[pid].TF_p->efl;
	p2 = &pcb[pid].TF_p->cs;
	//copy process trapframe to a temporary trapframe (local)
	tmp = *pcb[pid].TF_p;
	//lower the trapframe location info (in PCB) by 8 bytes
	(int)pcb[pid].TF_p -= 8;
	//copy temporary trapframe to the new lowered location
	*pcb[pid].TF_p = tmp;
	//the vacated 8 bytes: put 'handler_p,' and 'eip' of
        //the old trapframe there
	*p1 = (int) handler_p;
	*p2 = tmp.eip;
	//change 'eip' in the moved trapframe to Wrapper() address
	pcb[pid].TF_p->eip = (int)Wrapper;
}

void GetPpidISR(void) 
{
       pcb[cur_pid].TF_p->ebx = pcb[cur_pid].ppid; 
}

void ForkISR(void) 
{
	//cons_printf("We made it to ForkISR!!!\n");
	//return;
      //get (DeQ) a new child PID and put it into
      //the ebx of calling process trapframe (for Fork() to return)
      int cpid;
	int adj;
	int *p;
	
	cpid = DeQ(&avail_q);
	pcb[cur_pid].TF_p -> ebx = cpid;
      //if new child PID obtained is -1:
      //  1. show on TargetPC: Kernel Panic: no more process!
      //  2. just return
      if(cpid == -1)
      {
	      cons_printf("Kernel Panic: no more process!");
	      return;
      }
	
      //copy new child process' PCB from its parent's PCB
      pcb[cpid] = pcb[cur_pid];
	
      //set its state to the correct one
	pcb[cpid].state = READY;
	
      //enqueue its PID to ready queue
	EnQ(cpid, &ready_q);
	
      //set its ppid to the parent PID
      	pcb[cpid].ppid = cur_pid;
	
      //copy its parent's runtime stack
	MemCpy(stack[cpid], stack[pcb[cpid].ppid], STACK_SIZE);
	
      //calc the location distance between the two stacks, and
	adj = stack[cpid] - stack[cur_pid];
	
      //apply the distance to the child's TF_p
	(int)pcb[cpid].TF_p += adj;
	
      //then set ebx of its trapframe to 0 (child process gets 0 from Fork())
	pcb[cpid].TF_p -> ebx = 0;
	
      //apply the distance to esp, ebp, esi, edi in the child's trapframe
	pcb[cpid].TF_p ->esp += adj;
	pcb[cpid].TF_p ->ebp += adj;
	pcb[cpid].TF_p ->esi += adj;
	pcb[cpid].TF_p ->edi += adj;
	
      //(change all ebp if currently in a nested call stack:)
      //use an integer pointer p, set it to ebp (caller EBP)
	p =(int *)pcb[cpid].TF_p ->ebp;
	
      //loop: if what p points to is not 0:
      //   adjust what it points to with the distance
      //   change p to what it points (caller EBP still)
      //   jmp loop
	while(*p)
	{
		*p += adj;
		p = (int *) *p;
	}
		
}
	 
