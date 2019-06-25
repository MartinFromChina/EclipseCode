#include "EmarkConnected.h"
#include "PenState.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

#define PEN_CONNECTED_DEBUG  1

typedef enum
{
	StateJumpDetected,
	Init,
	TheEnd,

}PenState;

static PenState CurrentState;

static PenState StateJumpDetectedAction(X_Void)
{
	PenBasicState pbs;
	pbs = PenBasicStateGetUnderCertainState(PBS_Connected);
	if( pbs == PBS_Connected)
	{
		return Init;
	}
	else
	{
		SetCurrentBasicState(pbs);
		return TheEnd;
	}
}
static PenState InitAction(X_Void)
{
	return TheEnd;
}
static struct _StateHandle{
	PenState state;
	PenState (*Action)(X_Void);
}
const StateHandle[]={
	{StateJumpDetected,StateJumpDetectedAction},
	{Init,InitAction},
//	{ChargeWhenShutDown,ChargeWhenShutDownAction},
//	{DisConnected,DisConnectedAction},
//	{Connected,ConnectedAction},
	{TheEnd,X_Null},
};

X_Void PenConnectedStateHandle(X_Void)
{
	uint8_t i, state_number;

	state_number = (sizeof(StateHandle) / sizeof(StateHandle[0]));
	CurrentState = StateJumpDetected;
	for(i=0;i< state_number ;i++)
	{
		if(StateHandle[i].state == CurrentState)
		{
			if(StateHandle[i].Action != X_Null)
			{
				CurrentState = StateHandle[i].Action();
			}
			if(CurrentState == TheEnd){break;}
		}
	}

	if(i >= state_number)
	{
		String_Debug(PEN_CONNECTED_DEBUG,(30,"unknow state\r\n"));
		CurrentState = StateJumpDetected;
	}

}

X_Void PenConnectedStateInit(X_Void)
{

}
