// types.h, 159

#ifndef __TYPES__
#define __TYPES__

#include "constants.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {AVAIL, READY, RUN} state_t;

typedef struct {
   unsigned int reg[8];
   unsigned int eip;
   unsigned int cs;
   unsigned int efl;
} TF_t;

typedef struct {
   ...                       // read in 1.html
   ...
   ...
   ...
} pcb_t;                     

typedef struct {             // generic queue type
  ...                        // for a circular queue
  ...  
  ...  
  ...  
} q_t;

#endif
