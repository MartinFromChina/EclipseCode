#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "user_timer.h"
#include "..\CommonSource\IrqAndTimer\user_irq.h"
#include "..\CommonSource\IrqAndTimer\app_timer.h"


static uint8_t timer1_counter,timer2_counter;

APP_TIMER_DEF(timer1);
APP_TIMER_DEF(timer2);

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
void AllTimerConfig(void)
{
	AppTimerInit();
	timer1_counter = 0;
	timer2_counter = 0;
	AppTimerCreated(timer1,200,timer1_callback);
//	AppTimerCreated(timer2,100,timer2_callback);
	AppTimerStart(timer1);
//	AppTimerStart(timer2);
}
