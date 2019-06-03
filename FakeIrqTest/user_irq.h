#ifndef __USER_IRQ_H
#define __USER_IRQ_H

#include <stdbool.h>

#ifdef WIN32
    #include <windows.h>
    #include <io.h>
    #include <process.h>
    #define  MYVOID             void
#else
    #include <unistd.h>
    #include <sys/time.h>
    #include <pthread.h>
    #define  CRITICAL_SECTION   pthread_mutex_t
    #define  _vsnprintf         vsnprintf
    #define  MYVOID             void *
#endif

void sleep_ms(int ms);
void UserIrqCreated(void);
void UserIrqDelete(void);
void StartTimer(void);
void LockIrq(void);
void UnlockIrq(void);

#endif
