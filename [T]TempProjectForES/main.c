#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\CommonSource\IrqAndTimer\user_irq.h"
#include "..\CommonSource\IrqAndTimer\app_timer.h"

#if (ENTRY_DEBUG == 0)
	#error "entry error"
#endif

#define COUNTER_LIMIT  3
STRING_DEBUG_COUNTER_ENTRY_DEF(p_counter);
STRING_DEBUG_ONCE_ENTRY_DEF(p_entry,0);

static uint8_t sys_timer1_counter,sys_timer2_counter;
SYS_TIMER_DEF(systimer1);
SYS_TIMER_DEF(systimer2);

static uint8_t timer1_counter,timer2_counter;

APP_TIMER_DEF(timer1);
APP_TIMER_DEF(timer2);

static void PrintfInit(void)
{
	setbuf(stdout,NULL);
}
static void sys_timer1_callback(void)
{
	String_Debug(FOW_NOW_DEBUG,(25,"sys timer1 come %d\r\n",sys_timer1_counter++));
}
static void sys_timer2_callback(void)
{
	String_Debug(FOW_NOW_DEBUG,(25,"sys timer2 come %d\r\n",sys_timer2_counter++));
}
static void timer1_callback(void)
{
//	float tem = 0.3;
//	String_Debug(FOW_NOW_DEBUG,(25,"float : %f\r\n",tem));
	String_Debug(FOW_NOW_DEBUG,(25,"timer1 come %d\r\n",timer1_counter++));
//	String_Debug_Once(ENTRY_DEBUG,p_entry,2,(30,"entry timer 1\r\n"));
}
static void timer2_callback(void)
{
	String_Debug(FOW_NOW_DEBUG,(25,"timer2 come %d\r\n",timer2_counter++));
//	String_Debug_Counter(COUNTR_DEBUG,p_counter,COUNTER_LIMIT,(30,"timer2 come 3 times\r\n"));
}
static void AllTimerConfig(void)
{
	SystemTimerInit();
	sys_timer1_counter = 0;
	sys_timer2_counter = 0;
	SystemTimerCreated(systimer1,1,sys_timer1_callback);
	SystemTimerCreated(systimer2,10,sys_timer2_callback);
	SystemTimerStart(systimer1);
//	SystemTimerStart(systimer2);

	AppTimerInit();
	timer1_counter = 0;
	timer2_counter = 0;
	AppTimerCreated(timer1,500,timer1_callback);
	AppTimerCreated(timer2,1000,timer2_callback);
//	AppTimerStart(timer1);
//	AppTimerStart(timer2);
}
int main(void)
{
	StringDebugInit(PrintfInit);
	UserIrqCreated();
	AllTimerConfig();
	StartTimer();// important !!!

	String_Debug_Once(ENTRY_DEBUG,p_entry,1,(30,"entry 1\r\n"));
	while(1)
	{
		sleep_ms(1);// important !!!
		StringDebugProcess();
	}
	 UserIrqDelete();
	 return 0;
}
