#include "app_timer.h"
#include "..\AppError.h"

#include "..\Math\lcm_gcd.h"

#define USE_PRINTF_DEBUG   0
#if  (USE_PRINTF_DEBUG == 1)
	#include <stdio.h>
#endif

static uint8_t current_entry_number;
static uint32_t app_timer_counter_threshold,app_timer_counter;
static AppTimerEntry* pEntryBuf[MAX_TIMER_ENTRY_NUMBER];

static X_Boolean isTimerStart,isTimerLocked,isTimerJustBegin;

static X_Void AppTimerInit(X_Void)
{
	isTimerLocked = X_False;
	isTimerStart  = X_False;
	isTimerJustBegin = X_False;
	current_entry_number = 0;
	app_timer_counter = 0;
	app_timer_counter_threshold = 0;
}

X_Void AppTimerHook(X_Void)
{
	uint8_t i;

	if(isTimerLocked == X_True || isTimerStart == X_False) {return;}

	if(app_timer_counter >= app_timer_counter_threshold)
	{
		if(isTimerJustBegin == X_True)
		{
			isTimerJustBegin = X_False;
			app_timer_counter = 0;
		}
		else
		{
			app_timer_counter = 1;
		}

	}
	else
	{
		app_timer_counter ++ ;
	}


	for(i = 0 ; i<current_entry_number ; i++)
	{
		if(pEntryBuf[i] == X_Null) {break;}
		if(pEntryBuf[i]->isTimerStart == X_True && pEntryBuf[i]->divider > 0 && pEntryBuf[i]->p_action != X_Null)
		{
			if(app_timer_counter % pEntryBuf[i]->divider == 0)
			{
				#if  (USE_PRINTF_DEBUG == 1)
					printf("i = %d ; app_timer_counter : %d\r\n",i,app_timer_counter);
				#endif
				(*pEntryBuf[i]->p_action)();
			}

		}

	}
}

uint32_t AppTimerCreated(AppTimerEntry *p_entry,uint32_t interval,void(*callBack)(void))
{
	if(interval == 0xffffffff || interval == 0) {return APP_BEYOND_SCOPE;}
	if(current_entry_number > MAX_TIMER_ENTRY_NUMBER) {return APP_BEYOND_SCOPE;}
	p_entry->entry_number   = current_entry_number;
	pEntryBuf[current_entry_number] = p_entry;
	current_entry_number ++;
	p_entry->divider       = interval;
	p_entry->isTimerStart  = X_False;
	p_entry->p_action      = callBack;
	return APP_SUCCESSED;
}

uint32_t AppTimerStart(AppTimerEntry *p_entry)
{
	p_entry->isTimerStart  = X_True;
	return APP_SUCCESSED;
}
uint32_t AppTimerStop(AppTimerEntry *p_entry)
{
	p_entry->isTimerStart  = X_False;
	return APP_SUCCESSED;
}
uint32_t AppTimerModuleStart(X_Void(*timer_config)(X_Void))
{
	uint8_t i;
	uint32_t buf[MAX_TIMER_ENTRY_NUMBER + 1];

	AppTimerInit();
	/** */
	timer_config();
	/** */
	// get LCM
	for(i = 0;i<current_entry_number;i++)
	{
		buf[i] = pEntryBuf[i]->divider;
	}

	if(GetLCM(buf,current_entry_number,MAX_TIME_SCOPE_IN_MS,&app_timer_counter_threshold) == X_True)
	{
		isTimerStart = X_True;
		isTimerJustBegin = X_True;
		app_timer_counter = app_timer_counter_threshold;
		return APP_SUCCESSED;
	}
	else
	{
		isTimerStart = X_False;
		return APP_ERROR;
	}


}
X_Void AppTimerLocked(X_Void)
{
	isTimerLocked = X_True;
}
X_Void AppTimerUnlocked(X_Void)
{
	isTimerLocked = X_False;
}
