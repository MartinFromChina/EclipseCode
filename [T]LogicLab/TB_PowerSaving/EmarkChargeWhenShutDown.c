#include "EmarkChargeWhenShutDown.h"
#include "PenState.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

#define PEN_CHARGE_DEBUG  0
STRING_DEBUG_ONCE_ENTRY_DEF(p_state_entry,100);

typedef enum
{
	PS_StateJumpDetected,
	PS_Set,
	PS_Get,
	PS_Config,
	PS_TheEnd,

}PenState;

static PenState CurrentState;

static PenState StateJumpDetectedAction(X_Void)
{
	PenBasicState pbs;
	pbs = PenBasicStateGetUnderCertainState(PBS_ChargeWhenShutDown);
	if( pbs == PBS_ChargeWhenShutDown)
	{
		return PS_Set;
	}
	else
	{
		SetCurrentBasicState(pbs);
		return PS_TheEnd;
	}
}
static PenState SetAction(X_Void)
{
	String_Debug_Once(PEN_CHARGE_DEBUG,p_state_entry,PS_Set,(40,"-----PS_Set\r\n"));
	return PS_Get;
}
static PenState GetAction(X_Void)
{
	String_Debug_Once(PEN_CHARGE_DEBUG,p_state_entry,PS_Get,(40,"-----PS_Get\r\n"));
	return PS_Config;
}
static PenState ConfigAction(X_Void)
{
	String_Debug_Once(PEN_CHARGE_DEBUG,p_state_entry,PS_Config,(40,"-----PS_Config\r\n"));
	//RGB_StateCommanderWhenDisconnected();
	return PS_TheEnd;
}

static struct _StateHandle{
	PenState state;
	PenState (*Action)(X_Void);
}
const StateHandle[]={
	{PS_StateJumpDetected,StateJumpDetectedAction},
	{PS_Set,SetAction},
	{PS_Get,GetAction},
	{PS_Config,ConfigAction},
	{PS_TheEnd,X_Null},
};

X_Void PenChargeWhenShutDownStateHandle(X_Void)
{
	uint8_t i, state_number;

	state_number = (sizeof(StateHandle) / sizeof(StateHandle[0]));
	CurrentState = PS_StateJumpDetected;
	for(i=0;i< state_number ;i++)
	{
		if(StateHandle[i].state == CurrentState)
		{
			if(StateHandle[i].Action != X_Null)
			{
				CurrentState = StateHandle[i].Action();
			}
			if(CurrentState == PS_TheEnd){break;}
		}
	}

	if(i >= state_number)
	{
		String_Debug(PEN_CHARGE_DEBUG,(30,"unknow state\r\n"));
		CurrentState = PS_StateJumpDetected;
	}

}
X_Void PenChargeWhenShutDownStateInit(X_Void)
{
	// stop advertising
}
