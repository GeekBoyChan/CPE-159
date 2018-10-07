// lib.c, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 2

#include "include.h"
#include "types.h"
#include "data.h"

// clear DRAM data block, zero-fill it
void Bzero(char *p, int size) 
{   
   int count = 0;
   while(count <= size-1)
   {
     *p = (char)0;
     p++;
     count++;
   }
}

int QisEmpty(q_t *p)  // return 1 if empty, else 0
{
   if(p -> size  == 0)
   {
     return 1;
   }
   return 0;
}

int QisFull(q_t *p) 
{ // return 1 if full, else 0
  if(p->size == Q_SIZE)
  {
    return 1;
  }
  return 0;
}

// dequeue, 1st integer in queue
// if queue empty, return -1
int DeQ(q_t *p) 
{ // return -1 if q[] is empty
   int i;
   int next;

   if(QisEmpty(p))
   {
      return -1;
   }

   next = p->q[head]; // copy first in queue to be returned later
   p->size--;
   /*
   for(i = 0; i<p->size; i++) //decrease size of queue
   {
     p->q[i]=p->q[i+1]; // shift all by 1
   }
   */
   p->head = (p->head + 1)%Q_SIZE;
   return next;
}

// if not full, enqueue integer to tail slot in queue
void EnQ(int to_add, q_t *p) 
{
   if(QisFull(p)) 
   {
      cons_printf("Kernel panic: queue is full, cannot EnQ!\n");
      return;
   }

   p->q[p->tail] = to_add; //add int to next queue slot
   p->size++;
   p->tail = (p->tail + 1)%Q_SIZE;
   return;
}

