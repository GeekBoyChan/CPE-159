// entry.S, entries to kernel services, build process trapframe

#include "constants.h"
#include <spede/machine/asmacros.h> // do not use/include "spede.h"
#define K_STACK_SIZE (16*2^10)      // kernel has 16KB stack

.comm k_stack, K_STACK_SIZE, 1      // declare kernel stack space (16K x 1B)
.text

// loads proc TF to run it
ENTRY(Loader)
   movl 4(%esp)%ecx //copy 4 bytes from above register esp to register ecx
   mov %ecx, %esp   //copy ecx to esp (point esp to the process trapframe)
   popa             //pop all (8) registers
   iret             //return from interrupt

// build proc frame, and go into kernel
ENTRY(TimerEntry)                  // will have efl, cs, eip pushed by hardware
   pusha            //push all (8) registers
   mov %ecx, %esp   //copy esp to ecx
   movw $0x10, %ax                 // set kernel data segment registers
   mov %ax, %ds                    // set kernel data segment registers
   mov %ax, %es                    // set kernel data segment registers
   leal k_stack + K_STACK_SIZE, %esp  // set esp to top of kernel stack
   push ecx (onto the kernel stack)
   cld                              // clear the direction flag of efl
   call TheKernel C function

