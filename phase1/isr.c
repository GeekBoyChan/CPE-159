// isr.c, 159

#include "include.h"
#include "types.h"
#include "data.h"
#include "isr.h"
#include "lib.h"
#include "proc.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to ready_q
void NewProcISR(func_p_t p) {  // arg: where process code starts
   int pid;

   if( avail_q is empty ) {    // may occur if too many been created
      cons_printf("Kernel panic: no more process!\n");
      ...                      // cannot continue, alternative: breakpoint();
   }

   ...                                       // alloc PID (1st is 0)
   ...                                       // clear PCB
   ...                                       // clear stack
   ...                                       // change process state

   ...                                       // queue it

// point TF_p to stack & fill it out
   pcb[...].TF_p =                               
   pcb[...].TF_p--;
   pcb[...].TF_p->efl = EF_DEFAULT_VALUE|EF_INTR; // enables intr
   pcb[...].TF_p->cs = get_cs();                  // duplicate from CPU
   pcb[...].TF_p->eip =                           // set to code
}

// count run time and switch if hitting time limit
void TimerISR(void) {
   outportb(...                              // notify PIC getting done

   ...                                       // count up time
   ...                                       // count up life

   if(...                          ) {       // if runs long enough
      ...                                    // move it back to ready_q
      ...                                    // change its state
      ...                                    // now no running proc
   }
}

