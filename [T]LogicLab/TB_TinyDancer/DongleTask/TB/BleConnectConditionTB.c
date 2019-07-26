#include "BleConnectConditionTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"

static X_Boolean isBleConnected = X_False;
static uint32_t  ConditionDetectedTimes = 0;
static uint32_t current_num = 0;

RTT_DEBUG_ONCE_ENTRY_DEF(p_once,100);

typedef enum
{
	Idle = 0,
	BleConditionDetected,
}UserEventState;

static X_Void BleConditonToogle(X_Void)
{
	if(isBleConnected == X_True){isBleConnected = X_False;}
	else {isBleConnected = X_True;}
}


static StateNumber IdleAction(StateNumber current_state)
{
	SEGGER_RTT_Debug(USER_EVENT_DEBUG,(40,"----BleConditionInit\r\n"));
	isBleConnected = X_False;
	ConditionDetectedTimes = 0;
	current_num = 0;
	return BleConditionDetected;
}
static StateNumber ConditionDetectedAction(StateNumber current_state)
{

	current_num = GetCurrentRandomNumber();

	if(isBleConnected == X_False)
	{
		if(current_num <= 3)
		{
			BleConditonToogle();
		}
	}
	else
	{
		if(current_num > 17)
		{
			ConditionDetectedTimes ++;
		}
		if(ConditionDetectedTimes >= 5)
		{
			ConditionDetectedTimes = 0;
			BleConditonToogle();
		}
	}


	SEGGER_RTT_Debug_Once(BLE_CONDITION_DEBUG,p_once,isBleConnected,(43,"!!! isBleConnected: %d\r\n",isBleConnected));
	return current_state;
}

static const StateAction SimpleStateAction[2] = {
		{IdleAction},
		{ConditionDetectedAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_state,2,1,&SimpleStateAction[0]);

X_Boolean BleConditionHandle(X_Void)
{
	SimpleStateMachineRun(p_state,X_Null,X_Null);
	return isBleConnected;
}
X_Void ResetBleConditionState(X_Void)
{
	(*p_state->p_CurrentStateNum) = Idle;
}
