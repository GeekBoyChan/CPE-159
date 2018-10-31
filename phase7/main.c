// main.c, 159
// OS bootstrap and The Kernel for OS phase 7
//
// Team Name: LIGMAOS (Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso)

#include "include.h"  // given SPEDE stuff
#include "types.h"    // kernel data types
#include "lib.h"      // small handly functions
#include "proc.h"     // all user process code here
#include "isr.h"      // kernel ISR code
#include "entry.h"    // entries to kernel (TimerEntry, etc.)

// kernel data are all here:
int cur_pid;                        // current running PID; if -1, none selected
q_t ready_q, avail_q;               // avail PID and those created/ready to run
pcb_t pcb[PROC_MAX];                // Process Control Blocks
char stack[PROC_MAX][STACK_SIZE];   // process runtime stacks
int sys_ticks;                      // OS time(timer ticks), starting at 0
unsigned short *video_p;            // PC VGA video pointer, starting HOME_POS
sem_t sem[SEM_MAX];                 // kernel has these semaphores
q_t sem_q;                          // semaphore ID's are initially queued here
int car_sem;                        // to hold a semaphore ID for testing
term_if_t term_if[TERM_MAX];        // Terminal interface
char *rx_p;                         // point to inside the buffer
q_t rx_wait_q;                      // for PID waiting for term KB input

void TermInit(int which)
{
   // Clear terminal interface
   if(which == 0)
   {
      term_if[which].io = TERM0_IO;
      term_if[which].done = TERM0_DONE;
   }
   else
   {
      term_if[which].io = TERM1_IO;
      term_if[which].done = TERM0_DONE;
   }

   outportb(term_if[which].io+CFCR, CFCR_DLAB);             // CFCR_DLAB is 0x80
   outportb(term_if[which].io+BAUDLO, LOBYTE(115200/9600)); // period of each of 9600 bauds  
   outportb(term_if[which].io+BAUDHI, HIBYTE(115200/9600));
   outportb(term_if[which].io+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);
   outportb(term_if[which].io+IER, 0);
   outportb(term_if[which].io+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
  
   asm("inb $0x80");
   outportb(term_if[which].io+IER, IER_ERXRDY|IER_ETXRDY);  // enable TX & RX intr
   
   asm("inb $0x80");
   inportb(term_if[which].io); // clear key entered at PROCOMM screen
   
   
}

void InitKernel(void) {             // init and set up kernel!
   int i;
   struct i386_gate *IVT_p;         // IVT's DRAM location

   IVT_p   = get_idt_base();        // get IVT location
   fill_gate(&IVT_p[TIMER],(int)TimerEntry,get_cs(),ACC_INTR_GATE,0);         // Fill out IVT for timer
   fill_gate(&IVT_p[SYSCALL],(int)SyscallEntry,get_cs(),ACC_INTR_GATE,0);     // Fill out gate for SysCall
      
   fill_gate(&IVT_p[TERM0],(int)Term0Entry,get_cs(),ACC_INTR_GATE,0);         // Fill out gate for IVT Entry # Term0
   fill_gate(&IVT_p[TERM1],(int)Term1Entry,get_cs(),ACC_INTR_GATE,0);         // Fill out IVT Entry # TERM1
   outportb(PIC_MASK,MASK);         // mask out PIC

   Bzero((char*)&ready_q,sizeof(q_t));          // clear queues
   Bzero((char*)&avail_q,sizeof(q_t));
   Bzero((char*)&sem_q,sizeof(sem_t));
   for(i=0;i<PROC_MAX;i++)                      // add all avail PID's to the queue
     EnQ(i, &avail_q);
   for(i=0;i<SEM_MAX;i++)                       // add all avail PID's to the queue
     EnQ(i, &sem_q);
   cur_pid = -1;
   sys_ticks = 0;
   video_p = HOME_POS;
   
   TermInit(0);                     // Initialize terminal 0
   TermInit(1);                     // Initialize terminal 1
}

void Scheduler(void) 
{                                               // choose a cur_pid to run
   //if cur_pid is greater than 0, just return; // a user PID is already picked
   if(cur_pid > 0) return;

   //if ready_q is empty && cur_pid is 0, just return; // InitProc OK
   if(QisEmpty(&ready_q) && cur_pid == 0) return;

   //if ready_q is empty && cur_pid is -1 
   if(QisEmpty(&ready_q) && cur_pid == -1)
   {
      cons_printf("Kernel panic: no process to run!\n");
      breakpoint();                                  // to GDB we go
   }
   
   if(cur_pid!=-1) //of cur_pid not -1
   {
     EnQ(cur_pid, &ready_q);           // 1. append cur_pid to ready_q aka suspend cur_pid
     pcb[cur_pid].state = READY;       // 2. Change its state
     //cur_pid = DeQ(&ready_q); 
   }

   cur_pid = DeQ(&ready_q);      // Pick user proc
   pcb[cur_pid].time = 0;        //reset process time
   pcb[cur_pid].state = RUN;     //change its state
}

int main(void) {                       // OS bootstraps
   InitKernel(); //initialize the kernel-related stuff by calling ...
  
   NewProcISR(InitProc);               // create InitProc
   Scheduler();
   Loader(pcb[cur_pid].TF_p);          // load proc to run

   return 0;                           // statement never reached, compiler needs it for syntax
}

void TheKernel(TF_t *TF_p) {           // kernel runs
   char ch;

   pcb[cur_pid].TF_p = TF_p;           // save TF addr

   switch(TF_p->entry)                 //call Designated ISR
   {
      case TIMER:
        TimerISR();
        break;
      case WRITE:
        WriteISR();
        break;
      case READ:
        ReadISR();
        break;
      case GETPID:
        GetPidISR();
        break;
      case SETVIDEO:
        SetVideoISR();
        break;
      case SLEEP:
        SleepISR();
        break;
      case SEMINIT:
         SemInitISR();
         break;
      case SEMWAIT:
         SemWaitISR();
         break;
      case SEMPOST:
         SemPostISR();
         break;
      case TERM0:
         TermISR(0);
         break;
      case TERM1:
         TermISR(1);
         break;
      case SIGNAL:
         SignalISR();
         break;
   }

   if (cons_kbhit()) 
   {                             // if keyboard pressed
      ch = cons_getchar();
      switch(ch)
      {
         case 'b':               // 'b' for breakpoint
            breakpoint(); break; // go into GDB
         case 'n':               // 'n' for NewProcISR
            NewProcISR(UserProc); break;
         case 'c':               //'c' for carproc
            NewProcISR(CarProc); break;
         case 't':
            NewProcISR(TermProc); break;  //'t' for TermProc
      }
   }
   Scheduler();                  // which may pick another proc
   Loader(pcb[cur_pid].TF_p);    // load proc to run! With TF_p of schedule process
}


