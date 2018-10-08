// constants.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 4

#ifndef __CONSTANTS__
#define __CONSTANTS__

#define VGA_MASK 0x0f00     // foreground bright white, background black
#define TIMER 32            // TIMER constant identifier
#define PIC_MASK 0x21       // Programmable Interrupt Controller I/O
#define MASK ~0x01          // mask for Programmable Interrupt Controller
#define PIC_CONTROL 0x20    // Programmable Interrupt Controller I/O
#define DONE 0x60

#define LOOP 166666          // fast: 166666 times calling asm("inb $0x80");
#define TIME_MAX 350         // max timer count, then rotate process
#define PROC_MAX 20          // max number of processes
#define Q_SIZE 20            // queuing capacity
#define STACK_SIZE 4096      // process stack in bytes

//Added for Phase 2
#define STDOUT 1
#define WRITE 4
#define GETPID 20
#define SETVIDEO 100
#define SYSCALL 128
#define SLEEP 162
#define HOME_POS ((unsigned short *)0xb8000)
#define END_POS ((unsigned short *)0xb8000+24*80)

// Added for Phase 3
#define SEM_MAX 20 	         // kernel has 20 semaphores to dispatch
#define SEMINIT 101 	       // serivce # to req for semaphore
#define SEMWAIT 102	         // service # for sem-wait
#define SEMPOST 103	         // service # for sem-post

#endif
