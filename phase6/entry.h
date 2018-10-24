// entry.h
// prototypes for what's in entry.S
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 5

#ifndef __ENTRY__
#define __ENTRY__

#ifndef ASSEMBLER                 // skip below if ASSEMBLER defined (from an assembly code)
                                  // since below is not in assembler syntax
__BEGIN_DECLS

#include "types.h"                // TF_t

void TimerEntry(void);            // coded in entry.S, assembler won't like this syntax
void Loader(TF_t *);              // coded in entry.S
void SyscallEntry(void);
void Term0Entry(void);
void Term1Entry(void);

__END_DECLS

#endif // ifndef ASSEMBLER

#endif // ifndef _ENTRY_H_

