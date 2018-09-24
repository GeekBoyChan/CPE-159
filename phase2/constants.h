// constants.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 1

#ifndef __CONSTANTS__
#define __CONSTANTS__

#define VGA_MASK 0x0f00     // foreground bright white, background black
#define TIMER 32            // TIMER constant identifier
#define PIC_MASK 0x21       // Programmable Interrupt Controller I/O
#define MASK ~0x01          // mask for Programmable Interrupt Controller
#define PIC_CONTROL 0x20    // Programmable Interrupt Controller I/O
#define DONE 0x60

#define LOOP 166666          // slow: 1666666 times calling asm("inb $0x80");
#define TIME_MAX 350         // max timer count, then rotate process
#define PROC_MAX 20          // max number of processes
#define Q_SIZE 20            // queuing capacity
#define STACK_SIZE 4096      // process stack in bytes

#endif
