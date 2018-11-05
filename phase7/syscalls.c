// syscalls.c
// calls to OS services

// Team name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 7

#include "constants.h"
#include "types.h"

void Sleep(int sec) {         // # of seconds
   asm("movl %0, %%eax;       // service #162 (SLEEP)
        movl %1, %%ebx;       // sleep period in ticks
        int $128"
       :
       : "g" (SLEEP), "g" (sec)
       : "eax", "ebx"         // used registers
       );
}

int GetPid(void) {
   int pid;

   asm("movl %1, %%eax;       // service #20 (GETPID)
        int $128;             // interrupt!
        movl %%ebx, %0"       // after, copy eax to variable 'pid'
       : "=g" (pid)           // output
       : "g" (GETPID)         // input
       : "eax", "ebx"         // used registers
    );

   return pid;
}

void SetVideo(int row, int col) {
   asm("movl %0, %%eax;
        movl %1, %%ebx;
        movl %2, %%ecx;
        int $128"
        :
        : "g"(SETVIDEO), "g"(row), "g"(col)
        : "eax", "ebx", "ecx");
}

void Write(int device, char *str) {
   asm("movl %0, %%eax;
        movl %1, %%ebx;
        movl %2, %%ecx;
        int $128"
        :
        : "g"(WRITE), "g"(device), "g"((int)str)
        : "eax", "ebx", "ecx");
}

void Read(int device, char *buff) {
   asm("movl %0, %%eax;
        movl %1, %%ebx;
        movl %2, %%ecx;
        int $128"
        :
        : "g"(READ), "g"(device), "g"((int)buff)
        : "eax", "ebx", "ecx");
}

int SemInit(int passes) {
   int sem_id;
   asm("movl %1, %%eax;             //serive#
        movl %2, %%ebx;             // passes
        int $128;
        movl %%ecx, %0"             //sem_od
        : "=g" (sem_id)             //output
        : "g"(SEMINIT), "g"(passes)
        : "eax", "ebx", "ecx");     //used registers
   return sem_id;
}

void SemWait(int sem_id) {
   asm("movl %0, %%eax;
        movl %1, %%ebx;
        int $128"
        :
        : "g"(SEMWAIT), "g"(sem_id)
        : "eax", "ebx");
}

void SemPost(int sem_id) {
   asm("movl %0, %%eax;
        movl %1, %%ebx;
        int $128"
        :
        : "g"(SEMPOST), "g"(sem_id)
        : "eax", "ebx");
}
   
void Signal(int sig_num, func_p_t p) {
   asm("movl %0, %%eax;
        movl %1, %%ebx;
        movl %2, %%ecx;
        int $128"
        :
        : "g"(SIGNAL), "g"(sig_num), "g"((int)p)
        : "eax", "ebx", "ecx");
   
}

int GetPpid(void) 
{
   int pid;

   asm("movl %1, %%eax;       // service #20 (GETPID)
        int $128;             // interrupt!
        movl %%ebx, %0"       // after, copy eax to variable 'pid'
       : "=g" (pid)           // output
       : "g" (GETPPID)         // input
       : "eax", "ebx"         // used registers
    );

   return pid;
}
   
int Fork(void)
{
   int pid
   asm("movl %1, %%eax;
       int $128;             // interrupt!
        movl %%ebx, %0"       // after, copy eax to variable 'pid'
       : "=g" (pid)           // output
       : "g" (FORK)         // input
       : "eax", "ebx"         // used registers
   );
   return pid;
   //Returns the new child PID for the parent process, and 0 for the child; or -1 if running out of PID
}
