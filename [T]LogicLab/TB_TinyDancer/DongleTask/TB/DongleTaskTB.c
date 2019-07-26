#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\..\ScriptsCommandModule\HexCommand.h"
#include "..\..\..\..\CommonSource\Math\random_number.h"
#include "..\..\..\..\CommonSource\Math\bit_operation.h"

#include "UsbFeatureTB.h"
#include "UserEventTB.h"
#include "BleConnectConditionTB.h"

#define FRAME_STATE_MACHINE      1
#define FRAME_SCRIPT_FUNCTION    1

RTT_DEBUG_ONCE_ENTRY_DEF(p_script_func_num,100);

static uint32_t current_random_num = 0;
static X_Boolean isResetHappen = X_False,isBleConnected = X_False;

uint32_t GetCurrentRandomNumber(X_Void)
{
	return current_random_num;
}

FILE * DongleTaskOpenFile(void)
{
	return fopen(".//TB_TinyDancer//DongleTask//TB//command.txt", "r");
}
/***********************************************************************************************************************/

typedef enum
{
	KeyNotify,
	AirMouseNotify,
	PenTipNotify,
	ColorNotify,
	PenTypeNotify,
	BatteryNotify,
}EventCharacteristic;

typedef struct
{
	uint8_t isKeyNotify 		:1;
	uint8_t isAirMouseNotify	:1;
	uint8_t	isPenTipNotify		:1;
	uint8_t	isColorNotify		:1;
	uint8_t	isPenTypeNotify		:1;
	uint8_t	isBatteryNotify		:1;
}sNotifyEvent;

/***********************************************************************************************************************/
#if (FRAME_STATE_MACHINE == 1)
typedef enum
{
	GetRandomNum = 0,
	UserEventHandle,
	UsbEventHandle,
	BleEventHandle,
	NotifyEventHandle,
}EventGeneratorState;

static StateNumber GetRandomNumAction(StateNumber current_state)
{
	current_random_num = GetRandomNumber(1,20);
	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(30,"random number : %d\r\n",current_random_num));
	return (current_state+1);
}
static StateNumber UserEventHandleAction(StateNumber current_state)
{
	isResetHappen = DoesResetHappen();
	return (current_state+1);
}
static StateNumber UsbEventHandleAction(StateNumber current_state)
{
	GetFeatureAndSetFeature();
	return (current_state+1);
}
static StateNumber BleEventHandleAction(StateNumber current_state)
{
	isBleConnected = BleConditionHandle();
	return (current_state+1);
}
static StateNumber NotifyEventHandleAction(StateNumber current_state)
{
//	CharacteristicNotifyEventHandle(isBleConnected);
	return 0;
}

static const StateAction SimpleStateAction[5] = {
		{GetRandomNumAction},
		{UserEventHandleAction},
		{UsbEventHandleAction},
		{BleEventHandleAction},
		{NotifyEventHandleAction},
};


APP_SIMPLE_STATE_MACHINE_DEF(p_simple_state,5,5,&SimpleStateAction[0]);

static X_Void StateJumpRecorder(StateNumber current_state,StateNumber next_state)
{
	// going to jump new state:state_num
//	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(USER_MAX_STRING_LENGTH,"current:%d \r\n",current_state));
}
static X_Boolean DoesBreakSimple(const StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
//	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(USER_MAX_STRING_LENGTH,"DoesBreak:current:%d;next:%d\r\n",(*p_sbp->p_CurrentStateNum),nextstate));
	if((*p_sbp->p_CurrentStateNum) == NotifyEventHandle) // reach the last state
	{
		return X_True;
	}
	return X_False;
}
#endif
/***********************************************************************************************************************/
#if (FRAME_SCRIPT_FUNCTION == 1)
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
	SEGGER_RTT_Debug_Once(DONGLE_SCRIPT_DEBUG,p_script_func_num,1,(30,"--ScriptsFunctionInitial \r\n"));
	current_random_num = 0;
	isResetHappen = X_False;
	isBleConnected = X_False;
	ResetUserEventState();
	ResetBleConditionState();
	return X_True;
}
static X_Boolean EventGenerator(X_Void * p_param)
{
	SEGGER_RTT_Debug_Once(DONGLE_SCRIPT_DEBUG,p_script_func_num,2,(30,"EventGenerator \r\n"));
	SimpleStateMachineRun(p_simple_state,DoesBreakSimple,StateJumpRecorder);
	if(isResetHappen == X_False) {return X_True;}
	else {return X_False;}

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

#endif
