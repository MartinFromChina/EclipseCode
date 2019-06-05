#ifndef __APP_TIMER_H
#define __APP_TIMER_H

#include "..\KeilMDK.h"

#define MAX_TIMER_ENTRY_NUMBER    10

typedef struct
{
	uint8_t  entry_number;
	uint32_t divider;
	uint32_t current_counter;
	X_Boolean isTimerStart;
	void (*p_action)(void);
}AppTimerEntry;

#define CONCAT_TWO(p1, p2)      p1##p2

#define APP_TIMER_DEF(timer_id)            					\
		static AppTimerEntry   CONCAT_TWO(timer_id,_entry);								\
		static AppTimerEntry 	* timer_id =   &CONCAT_TWO(timer_id,_entry);

X_Void AppTimerInit(X_Void);
X_Void AppTimerHook(X_Void);
uint32_t AppTimerCreated(AppTimerEntry *p_entry,uint32_t interval,void(*callBack)(void));
uint32_t AppTimerStart(AppTimerEntry *p_entry);
uint32_t AppTimerRestart(AppTimerEntry *p_entry);
uint32_t AppTimerStop(AppTimerEntry *p_entry);
X_Void AppTimerLocked(X_Void);
X_Void AppTimerUnlocked(X_Void);

#define TIMER_HOOK  AppTimerHook

#endif
