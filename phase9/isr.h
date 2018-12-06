// isr.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 8

#ifndef __ISR__
#define __ISR__

void NewProcISR(func_p_t p);  //Create process
void TimerISR(void);          //Timer ISR
void GetPidISR(void);          //
void SleepISR(void);
void SetVideoISR(void);
void WriteISR(void);
void ReadISR(void);
void SemInitISR(void);
void SemWaitISR(void);
void SemPostISR(void);
void TermISR(int index);
void TermTxISR(int index);
void TermRxISR(int interface_num);
void SignalISR(void);
void WrapperISR(int pid, func_p_t handler_p);
void GetPpidISR(void);
void ForkISR(void);
void ExitISR(void);
void WaitISR(void);

#endif
