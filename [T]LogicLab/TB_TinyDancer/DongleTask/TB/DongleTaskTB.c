#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\..\ScriptsCommandModule\HexCommand.h"
#include "..\..\..\..\CommonSource\Math\random_number.h"

RTT_DEBUG_ONCE_ENTRY_DEF(p_script_func_num,100);

static uint32_t current_random_num = 0;

FILE * DongleTaskOpenFile(void)
{
	return fopen(".//TB_TinyDancer//DongleTask//TB//command.txt", "r");
}
/***********
 * 0755xxccaabbpp	   :	call number xx function
 * 					cc : 00   pointer to next line
 * 					cc : 01   if xx function return true , pointer to aa line ; else pointer to bb line
 *					pp : parameter to number xx function
 */
/*
 *	1	:FunctionInitial					(1)  00010100
 *	2	:EventGenerator						(2)  01020100
 *
1:command:07550100010100
2:command:07550201020100
3:end
 */

static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	SEGGER_RTT_Debug_Once(DONGLE_SCRIPT_DEBUG,p_script_func_num,1,(30,"ScriptsFunctionInitial \r\n"));
	current_random_num = 0;
	return X_True;
}

typedef enum
{
	GetRandomNum = 0,
	GetRandomEvent,
	RandomEventReport,
	RandomEventAction,
}EventGeneratorState;

static StateNumber GetRandomNumAction(StateNumber current_state)
{
	current_random_num = GetRandomNumber(1,100);
	return GetRandomEvent;
}
static StateNumber GetRandomEventAction(StateNumber current_state)
{
	return RandomEventReport;
}
static StateNumber RandomEventReportAction(StateNumber current_state)
{
	return RandomEventAction;
}
static StateNumber RandomEventActionAction(StateNumber current_state)
{
	return GetRandomEvent;
}

static const StateAction SimpleStateAction[4] = {
		{GetRandomNumAction},
		{GetRandomEventAction},
		{RandomEventReportAction},
		{RandomEventActionAction},
};


APP_SIMPLE_STATE_MACHINE_DEF(p_simple_state,4,4,&SimpleStateAction[0]);

static X_Void StateJumpRecorder(StateNumber current_state,StateNumber next_state)
{
	// going to jump new state:state_num
	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(USER_MAX_STRING_LENGTH,"current:%d \r\n",current_state));
}
static X_Boolean DoesBreakSimple(const StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
//	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(USER_MAX_STRING_LENGTH,"DoesBreak:current:%d;next:%d\r\n",(*p_sbp->p_CurrentStateNum),nextstate));
	if((*p_sbp->p_CurrentStateNum) == RandomEventAction) // reach the last state
	{
		return X_True;
	}
	return X_False;
}

static X_Boolean EventGenerator(X_Void * p_param)
{
	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(30,"EventGenerator \r\n"));
//	SEGGER_RTT_Debug_Once(DONGLE_SCRIPT_DEBUG,p_script_func_num,2,(30,"EventGenerator \r\n"));
	SimpleStateMachineRun(p_simple_state,DoesBreakSimple,StateJumpRecorder);
	return X_True;
}

static struct _ScriptsFunctionArray
{
	uint8_t function_number;
	X_Boolean(*action)(X_Void * p_param);
}
const ScriptsFunctionArray[] = {
	  {1,ScriptsFunctionInitial},
	  {2,EventGenerator},
};

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param)
{
	uint8_t i;
	X_Boolean isOK;

	isOK = X_False;
	for(i=0;i<sizeof(ScriptsFunctionArray)/sizeof(ScriptsFunctionArray[0]);i++)
	{
		if(ScriptsFunctionArray[i].function_number == func_num && ScriptsFunctionArray[i].action != X_Null)
		{
			isOK = ScriptsFunctionArray[i].action(p_param);
			break;
		}
	}
	return isOK;
}
