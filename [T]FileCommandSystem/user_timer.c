#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "user_timer.h"
#include "..\CommonSource\IrqAndTimer\user_irq.h"
#include "..\CommonSource\IrqAndTimer\app_timer.h"
#include "HexCommand.h"

static uint32_t timer1_counter,timer2_counter;

APP_TIMER_DEF(DebugTimer);
APP_TIMER_DEF(timer1);
APP_TIMER_DEF(timer2);

static void Debugtimer_callback(void)
{
	StringDebugProcess();
//	StringDebugFlush();////
}

static void timer1_callback(void)
{
//	ScriptCommandHandle(DoAsCommand);
	ConditionalScriptCommandHandle(ConditionalDoAsCommand);
}
static void timer2_callback(void)
{
	String_Debug(FOW_NOW_DEBUG,(30,"two come :%d\r\n",timer2_counter));
//	printf("two come :%d\r\n",timer2_counter);
	timer2_counter = timer2_counter + 2000;
}
void AllTimerConfig(void)
{
	timer1_counter = 0;
	timer2_counter = 0;
	AppTimerCreated(DebugTimer,1,Debugtimer_callback);
	AppTimerCreated(timer1,1000/RELATIVE_TIMER_DIVIDER,timer1_callback);
	AppTimerCreated(timer2,2000/RELATIVE_TIMER_DIVIDER,timer2_callback);
	AppTimerStart(DebugTimer);
	AppTimerStart(timer1);
//	AppTimerStart(timer2);
}
