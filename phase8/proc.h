// proc.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 7

#ifndef __PROC__
#define __PROC__

void InitProc(void); 
void UserProc(void);
void CarProc(void);
void TermProc(void);
void Ouch(void);
void Wrapper(func_p_t handler_p);
void ChildCode(void);
void ChldHandler(void);
#endif
