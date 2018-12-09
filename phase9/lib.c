// lib.c, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 9

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
   int i = 0;
   
   while(s1[i] != '\0')
   {
      if(s1[i] != s2[i])
         return 0;
      i++;
   }
   return 1;
}

void DelQ(int delete, q_t *p)
{
   int i;
   q_t tmp_q;
   
   Bzero((char *)&tmp_q, sizeof(q_t));
   
   for(i=0;i<p->size; i++)
   {
      if (delete == p->q[(p->head +i) % Q_SIZE]) 
         continue;
       EnQ(DeQ(p), &tmp_q);
   }
   
   *p = tmp_q;
}
      
int InQ(int in, q_t *p)
{
   int i;
   
   for(i = 0; i < p->size; i++)
         if(in == p->q[(p->head + i) % Q_SIZE])
            return 1;
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

void ReclaimPages(int pid)
{
   // Search through memory info array by pid, change 'owned' --> 'not owned'
   int i;
   
   for(i=0; i<PAGE_MAX; i++)
   {
      if(pages[i].owner == pid)
         pages[i].owner = -1;
   }
}

int Alloc(int pid, int how_many, int page_index[]) 
{
   int i , counter;
   //search thru the memory info array for 'how_many' pages currently not owned
   //set them to be owned by 'pid,' and collect their page indices into page_index[]
   //return 0 if successfully got that many asked,
   //otherwise, leave/recover owner info intact and return -1
   
   counter = 0;
   for(i=0; i<PAGE_MAX; i++)
   {
      if(pages[i].owner == -1)
      {
         pages[i].owner = pid;
         page_index[counter] = i;
         counter ++;
         if(counter == how_many)
            return 0;
      }
   }
   
   for(i=0; i<counter; i++)
      pages(page_index[counter]).owner = -1; //release
   
   return -1;
}
