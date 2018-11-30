// lib.h, 159
// Team Name: LIGMAOS
// Members: Andrew Encinas, Chandler Ocapan, Alex Paraiso
// Phase 8

#ifndef __LIB__
#define __LIB__

void Bzero(char *p, int bytes);
int QisEmpty(q_t *p);
int QisFUll(q_t *p);
int DeQ(q_t *p);
void EnQ(int to_add, q_t *p);
int StrCmp(char *s1, char *s2);
void MemCpy(char *dst, char *src, int size);
int InQ(int in, q_t *p);
void DelQ(int delete, q_t *p);

#endif
