#include "user_irq.h"
#include "app_timer.h"
#include "..\AppError.h"

static void SystemTimerHook(void);
static void SystemTimerInit(void);

#ifndef TIMER_HOOK
	static void TimerHook(void){}
	#define TIMER_HOOK TimerHook
#endif
#define TimerTicksInMs  1

CRITICAL_SECTION cs_log;


#ifdef WIN32
static void Lock(CRITICAL_SECTION *l) {
    EnterCriticalSection(l);
}
static void Unlock(CRITICAL_SECTION *l) {
    LeaveCriticalSection(l);
}
void sleep_ms(int ms) {
    Sleep(ms);
}
#else
static void Lock(CRITICAL_SECTION *l) {
    pthread_mutex_lock(l);
}
static void Unlock(CRITICAL_SECTION *l) {
    pthread_mutex_unlock(l);
}
void sleep_ms(int ms) {
    usleep(ms*1000);
}
#endif

bool isTimerStart;

static void TimerThread(void *pcn) {
    while (1) {
		sleep_ms(TimerTicksInMs);
		if(isTimerStart == true)
		{
			SystemTimerHook();
			TIMER_HOOK();
    	}
    }
}

void UserIrqCreated(void)
{
#ifdef WIN32
    InitializeCriticalSection(&cs_log);
#else
    pthread_mutex_init(&cs_log,NULL);
    pthread_t threads[1];
    int threadsN;
    int rc;
#endif
	#ifdef WIN32
    _beginthread((void(__cdecl *)(void *))TimerThread,0,(void *)1);
	#else
    threadsN=0;
    rc=pthread_create(&(threads[threadsN++]),NULL,TimerThread,(void *)1);if (rc) Log("%d=pthread_create %d error!\n",rc,threadsN-1);
	#endif
    isTimerStart = false;
    SystemTimerInit();
}
void UserIrqDelete(void)
{
	#ifdef WIN32
    DeleteCriticalSection(&cs_log);
	#else
    pthread_mutex_destroy(&cs_log);
	#endif
}
void StartTimer(void)
{
	isTimerStart = true;
}
void LockIrq(void)
{
	 Lock(&cs_log);
}
void UnlockIrq(void)
{
	Unlock(&cs_log);
}

static uint8_t current_entry_number;
static SystemTimerEntry * pEntryBuf[MAX_SYS_TIMER_ENTRY_NUMBER];

static void SystemTimerInit(void)
{
	current_entry_number = 0;
}

uint32_t SystemTimerStart(SystemTimerEntry *p_entry)
{
	p_entry->isTimerStart  = X_True;
	return APP_SUCCESSED;
}
uint32_t SystemTimerStop(SystemTimerEntry *p_entry)
{
	p_entry->isTimerStart  = X_False;
	return APP_SUCCESSED;
}
uint32_t SystemTimerCreated(SystemTimerEntry *p_entry,uint32_t interval,void(*callBack)(void))
{
	if(interval == 0xffffffff) {return APP_BEYOND_SCOPE;}
	if(current_entry_number > MAX_SYS_TIMER_ENTRY_NUMBER) {return APP_BEYOND_SCOPE;}
	p_entry->entry_number   = current_entry_number;
	pEntryBuf[current_entry_number] = p_entry;
	current_entry_number ++;
	p_entry->current_counter = 0;
	p_entry->divider       = interval;
	p_entry->isTimerStart  = X_False;
	p_entry->p_action      = callBack;
	return APP_SUCCESSED;
}

static void SystemTimerHook(void)
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
