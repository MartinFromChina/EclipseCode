#include "user_irq.h"
#include "app_timer.h"

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
