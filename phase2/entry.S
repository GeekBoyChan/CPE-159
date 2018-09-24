// entry.S, entries to kernel, build process TF

#include "constants.h"
#include <spede/machine/asmacros.h> // do not use/include "spede.h"

#define K_STACK_SIZE (16*1024)      // kernel has 16KB stack

.comm k_stack, K_STACK_SIZE, 1      // declare kernel stack space (16K x 1B)
.text

// loads proc TF to run it
ENTRY(Loader)
   movl 4(%esp), %ecx
   movl %ecx, %esp
   popa
   ... to skip the 'entry' of TF, increment stack pointer by 4 bytes
   iret

// build/push TF then go to kernel
ENTRY(TimerEntry)
   ... push the timer entry number ($TIMER)
   ... jump to the common routine

ENTRY(SyscallEntry)
   ... push eax register which has the entry number (via syscall)
   ... jump to the common routine

CommonRoutine:
   pusha   // push all: eax ecx edx ebx esp ebp esi edi
   movl %esp, %ecx          // copy esp
   movw $0x10, %ax          // put 16 to ax
   mov %ax, %ds             // set kernel data segment registers ds
   mov %ax, %es             // set kernel data segment registers es
   leal k_stack + K_STACK_SIZE, %esp  // set esp to top of kernel stack
   pushl %ecx               // push (proc TF addr) there
   cld                      // clear direction flag bit
   call CNAME(TheKernel)    // call C kernel code

