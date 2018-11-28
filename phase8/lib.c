// lib.c, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 7

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

int QisEmpty(q_t *p)        // return 1 if empty, else 0
{
   if(p -> size  == 0)
   {
     return 1;
   }
   return 0;
}

int QisFull(q_t *p)         // return 1 if full, else 0
{ 
  if(p->size == Q_SIZE)
  {
    return 1;
  }
  return 0;
}

// dequeue, 1st integer in queue
// if queue empty, return -1
int DeQ(q_t *p) 
{// return -1 if q[] is empty
   int next;

   if(QisEmpty(p))
   {
      return -1;
   }

   next = p->q[p->head];            // copy first in queue to be returned later
   p->size--;
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

   p->q[p->tail] = to_add;           //add int to next queue slot
   p->size++;
   p->tail = (p->tail + 1)%Q_SIZE;
   return;
}

int StrCmp(char *s1, char *s2)
{
   //printf("We're inside StrCmp \n\r");
   int i;
   for(i = 0; s1[i] && s2[i]; i++)
   {
      if(s1[i] != s2[i])
      {
         return 0;
      }
   }
   return 1;
}

void DelQ(int delete, q_t *p)
{
   if(QisEmpty(p))
   {
      return;
   }
   int count;
   int i;
   int * ptr;
   ptr = &(p->q[p->head]);
   while(count <= p->size-1)
   {
      if(*ptr == delete)
      {
         for(i = count; i < p->tail - 1; i++)
         {
            p->q[count] = p->q[count+1];
            i++;
         }
         p->tail--;
         p->size--;
      }
      ptr++;
      count++;
   }
   return;
}
      
int InQ(int in, q_t *p)
{
   int count = 0;
   int * ptr;
   ptr = &(p->q[p->head]);
   while(count <= p->size-1)
   {
     if (*ptr == in) return 1;
     ptr++;
     count++;
   }
   return 0;
}

void MemCpy(char *dst, char *src, int size)
{
   // Copies 'size' bytes from memory location 'src' to location 'dst'
   char *source = (char *)src;
   char *dest = (char *)dst;
   int i;
   
   for(i =0; i<size; i++)
   {
      dest[i] = source[i];
   }
   return;
}

