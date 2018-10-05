// types.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 2

#ifndef __TYPES__
#define __TYPES__

#include "constants.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {AVAIL, READY, RUN, SLEEPY} state_t;

typedef struct {
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
   unsigned int entry;
   unsigned int eip,cs,efl;
} TF_t;

typedef struct {
   TF_t * TF_p;                       // read in 1.html
   int time;
   int life;
   int wake_time;
   state_t state;
} pcb_t;                     

typedef struct {             // generic queue type
  int q[Q_SIZE];                        // for a circular queue
  int head, tail, size;  
} q_t;

typedef struct { 
   int passes;
   q_t wait_q;
} sem_t;

#endif
