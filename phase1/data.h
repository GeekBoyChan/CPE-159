// data.h, 159
// kernel data are all declared in main.c during bootstrap
// kernel .c code reference them as 'extern'

// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso

#ifndef __DATA__
#define __DATA__

#include "types.h"             // defines q_t, pcb_t, PROC_MAX, STACK_SIZE

extern int cur_pid;                     // PID of current selected running process
extern q_t ready_q, avail_q;            // avail PID and those created/ready to run
extern pcb_t pcb[PROC_MAX];             //Process control blocks 
extern char stack[PROC_MAX][STACK_SIZE]; // Process runtime stacks

#endif                         // endif of ifndef
