// syscalls.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 4

#ifndef __SYSCALLS__
#define __SYSCALLS__

void Sleep(int sec);
int GetPid(void);
void SetVideo(int row, int col);
void Write(int device, char *str);
int SemInit(int passes);
void SemWait(int sem_id);
void SemPost(int sem_id);

#endif
