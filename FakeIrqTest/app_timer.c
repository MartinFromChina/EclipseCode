#include "app_timer.h"
#include "..\CommonSource\AppError.h"

static uint8_t current_entry_number = 0;
static AppTimerEntry * pEntryBuf[MAX_TIMER_ENTRY_NUMBER];

X_Void AppTimerHook(X_Void)
{
	uint8_t i;
	for(i = 0 ; i<current_entry_number ; i++)
	{
		if(pEntryBuf[i] == X_Null) {break;}
		if(pEntryBuf[i]->isTimerStart == X_True)
		{
			if(pEntryBuf[i]->current_counter == pEntryBuf[i]->divider)
			{
				pEntryBuf[i]->current_counter = 0;
				if(pEntryBuf[i]->p_action != X_Null)
				{
					(*pEntryBuf[i]->p_action)();
				}
			}
			else
			{
				pEntryBuf[i]->current_counter ++;
			}

		}

	}
}

uint32_t AppTimerCreated(AppTimerEntry *p_entry,uint32_t interval,void(*callBack)(void))
{
	if(interval == 0xffffffff) {return APP_BEYOND_SCOPE;}
	if(current_entry_number > MAX_TIMER_ENTRY_NUMBER) {return APP_BEYOND_SCOPE;}
	p_entry->entry_number   = current_entry_number;
	pEntryBuf[current_entry_number] = p_entry;
	current_entry_number ++;
	p_entry->current_counter = 0;
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
uint32_t AppTimerRestart(AppTimerEntry *p_entry)
{
	p_entry->current_counter = 0;
	p_entry->isTimerStart  = X_True;
	return APP_SUCCESSED;
}
uint32_t AppTimerStop(AppTimerEntry *p_entry)
{
	p_entry->isTimerStart  = X_False;
	return APP_SUCCESSED;
}

