#ifndef __USER_IRQ_H
#define __USER_IRQ_H

#include <stdbool.h>
#include "..\KeilMDK.h"

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

#define RELATIVE_TIMER_DIVIDER        10

void sleep_ms(int ms);
void UserIrqCreated(void);
void UserIrqDelete(void);
void StartTimer(void);
void LockIrq(void);
void UnlockIrq(void);

#define MAX_SYS_TIMER_ENTRY_NUMBER    10

typedef struct
{
	uint8_t  entry_number;
	uint32_t divider;
	uint32_t current_counter;
	X_Boolean isTimerStart;
	void (*p_action)(void);
}SystemTimerEntry;

#define CONCAT_TWO(p1, p2)      p1##p2

#define SYS_TIMER_DEF(timer_id)            					\
		static SystemTimerEntry   CONCAT_TWO(timer_id,_entry);								\
		static SystemTimerEntry 	* timer_id =   &CONCAT_TWO(timer_id,_entry);

uint32_t SystemTimerStart(SystemTimerEntry *p_entry);
uint32_t SystemTimerStop(SystemTimerEntry *p_entry);
uint32_t SystemTimerCreated(SystemTimerEntry *p_entry,uint32_t interval,void(*callBack)(void));

#endif
