#include "UserEventTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"

static X_Boolean isReset = X_False;
static uint32_t  ResetConditionDetectedTimes = 0;
static uint32_t current_num = 0;

static _sCommandMap const CommandMap[]= {
		{0,SuddenlyPowerOff		,0,"SuddenlyPowerOff"	},
		{1,ResetByWatchDog		,0,"ResetByWatchDog"  },
		{2,ResetByUser			,0,"ResetByUser"		},
		{3,HardFault			,0,"HardFault"		},
};

typedef enum
{
	Idle = 0,
	ResetConditionDetected,
}UserEventState;


static StateNumber IdleAction(StateNumber current_state)
{
	SEGGER_RTT_Debug(USER_EVENT_DEBUG,(30,"----UserEventInit\r\n"));
	isReset = X_False;
	ResetConditionDetectedTimes = 0;
	current_num = 0;
	return ResetConditionDetected;
}
static StateNumber ResetConditionDetectedAction(StateNumber current_state)
{

	current_num = GetCurrentRandomNumber();

	if(current_num > 16)
	{
		ResetConditionDetectedTimes ++;
		SEGGER_RTT_Debug(USER_EVENT_DEBUG,(43,"ResetConditionDetected:%d\r\n",ResetConditionDetectedTimes));
	}

	if(ResetConditionDetectedTimes >= 20)
	{
		ResetConditionDetectedTimes = 0;
		SEGGER_RTT_Debug(USER_EVENT_DEBUG,(43,"Reset: %s\r\n",CommandMap[20-current_num].p_char));
		isReset = X_True;
	}
	else
	{
		isReset = X_False;
	}
	return current_state;
}

static const StateAction SimpleStateAction[2] = {
		{IdleAction},
		{ResetConditionDetectedAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_state,2,1,&SimpleStateAction[0]);

X_Boolean DoesResetHappen(X_Void)
{
	SimpleStateMachineRun(p_state,X_Null,X_Null);
	return isReset;
}

X_Void ResetUserEventState(X_Void)
{
	(*p_state->p_CurrentStateNum) = Idle;
}
