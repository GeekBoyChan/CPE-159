// main.c, 159
// OS bootstrap and The Kernel for OS phase 1
//
// Team Name: LIGMAOS (Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso)

#include "include.h"  // given SPEDE stuff
#include "types.h"    // kernle data types
#include "lib.h"      // small handly functions
#include "proc.h"     // all user process code here
#include "isr.h"      // kernel ISR code
#include "entry.h"    // entries to kernel (TimerEntry, etc.)

// kernel data are all here:
int cur_pid;                        // current running PID; if -1, none selected
q_t ready_q, avail_q;               // avail PID and those created/ready to run
pcb_t pcb[PROC_MAX];                // Process Control Blocks
char stack[PROC_MAX][STACK_SIZE];   // process runtime stacks

void InitKernel(void) {             // init and set up kernel!
   int i;
   struct i386_gate *IVT_p;         // IVT's DRAM location

   IVT_p   = get_idt_base();          // get IVT location
   fill_gate(&IVT_p[TIMER],(int)TimerEntry,get_cs(),ACC_INTR_GATE,0);                  // fill out IVT for timer
   outportb(PIC_MASK,MASK);                   // mask out PIC for timer

   Bzero((char*)&ready_q,sizeof(q_t));                      // clear 2 queues
   Bzero((char*)&avail_q,sizeof(q_t));
   for(i=0;i<PROC_MAX;i++)                    // add all avail PID's to the queue
     EnQ(i, &avail_q);
   cur_pid = -1;
}

void Scheduler(void) 
{             // choose a cur_pid to run
   //if cur_pid is greater than 0, just return; // a user PID is already picked
   if(cur_pid > 0) return;

   //if ready_q is empty && cur_pid is 0, just return; // InitProc OK
   //Change if statement to: if(QisEmpty(&ready_q) && curpid == 0) return;
   if(ready_q.size==0 && cur_pid==0) return;

   //if ready_q is empty && cur_pid is -1 
   //Change if statement to: if(QisEmpty(&ready_q) && curpid == -1)
   if(ready_q.size==0 && cur_pid==-1)
   {
      cons_printf("Kernel panic: no process to run!\n");
      breakpoint();                                  // to GDB we go
   }
   
   if(cur_pid!=-1) //of cur_pid not -1
   {
     EnQ(cur_pid, &ready_q);       // 1. append cur_pid to ready_q aka suspend cur_pid
     pcb[cur_pid].state = READY;  // 2. Change its state
     //cur_pid = DeQ(&ready_q); 
   }

   cur_pid = DeQ(&ready_q); // Pick user proc
   pcb[cur_pid].time = 0;  //reset process time
   pcb[cur_pid].state = RUN; //change its state
}

int main(void) {                       // OS bootstraps
   InitKernel(); //initialize the kernel-related stuff by calling ...
  
   NewProcISR(InitProc);            // create InitProc
   Scheduler();
   Loader(pcb[cur_pid].TF_p);         // load proc to run

   return 0; // statement never reached, compiler needs it for syntax
}

void TheKernel(TF_t *TF_p) {           // kernel runs
   char ch;

   pcb[cur_pid].TF_p = TF_p; // save TF addr

   //call Timer ISR;                     // handle tiemr event
   TimerISR();

   if (cons_kbhit()) 
   {                  // if keyboard pressed
      ch = cons_getchar();
      if (ch == 'b')
      {                     // 'b' for breakpoint
         //outportb(PIC_CONTROL,DONE);                        // go into GDB
         breakpoint();
         //Add break; here
      }
      if(ch=='n')                     // 'n' for new process
         NewProcISR(UserProc); //NewProc ISR (with UserProc as argument)creates a UserProc
  
   }
   Scheduler(); // which may pick another proc
   Loader(pcb[cur_pid].TF_p); // load proc to run! With TF_p of schedule process
}

