// lib.c, 159

#include "include.h"
#include "types.h"
#include "data.h"

// clear DRAM data block, zero-fill it
void Bzero(char *p, int bytes) {
   ...
}

int QisEmpty(q_t *p) { // return 1 if empty, else 0
   ...
}

int QisFull(q_t *p) { // return 1 if full, else 0
   ...
}

// dequeue, 1st integer in queue
// if queue empty, return -1
int DeQ(q_t *p) { // return -1 if q[] is empty
   ...

   if(QisEmpty(p)) {
      ...
   }

   ...
   ...
   ...

   ...
}

// if not full, enqueue integer to tail slot in queue
void EnQ(int to_add, q_t *p) {
   if(QisFull(p)) {
      cons_printf("Kernel panic: queue is full, cannot EnQ!\n");
      ...
   }

   ...
   ...
   ...
}

