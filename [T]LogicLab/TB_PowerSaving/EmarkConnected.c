#include "EmarkConnected.h"
#include "PenState.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

#define PEN_CONNECTED_DEBUG  1

typedef enum
{
	PS_StateJumpDetected,
	PS_Set,
	PS_Get,
	PS_Config,
	PS_TheEnd,

}PenState;

static PenState CurrentState;

static X_Void TestModeConfigClear(X_Void)
{
//	SetMode(ModeProduct);
//	SetCurrentAirMouse(X_True);
}

static PenState StateJumpDetectedAction(X_Void)
{
	PenBasicState pbs;
	pbs = PenBasicStateGetUnderCertainState(PBS_Connected);
	if( pbs == PBS_Connected)
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
	// stop advertising
	return PS_Get;
}
static PenState GetAction(X_Void)
{
	return PS_Config;
}
static PenState ConfigAction(X_Void)
{
	// all emark service and charactor
	// AdcStateCommanderWhenConnect();adc ,adc sum, battery strength monitor enable
	// TimeStampRefresh(X_True);

	/*
	 *  if test mode
	 *	RGB_StateCommanderInTestMode
	 *	DirectSetSwitchFlagsOnce((uint8_t)ModeTest,&CharacSwitchInTestMode);
	 *	// power config :
	 *		SetNormalPowerState(X_False);
			ExternalPowerOnUsed(PowerModuleEmark,X_False);
	 *  else
	 *  DirectSetSwitchFlagsOnce((uint8_t)ModeProduct,&CharacSwitchInProductMode);
		RGB_StateCommanderWhenConnected();

		SetNormalPowerState(X_True);

		FastAdjustPressureInit(true);
	 *
	 */
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

X_Void PenConnectedStateHandle(X_Void)
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
		String_Debug(PEN_CONNECTED_DEBUG,(30,"unknow state\r\n"));
		CurrentState = PS_StateJumpDetected;
	}

}

X_Void PenConnectedStateInit(X_Void)
{
	//FastAdjustPressureInit(true);
	//TestModeConfigClear();
}
