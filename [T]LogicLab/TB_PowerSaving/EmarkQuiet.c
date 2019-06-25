#include "EmarkQuiet.h"
#include "PenState.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

#define PEN_QUIET_DEBUG  0
#define PEN_QUIET_WAKEUP_DEBUG  1

STRING_DEBUG_ONCE_ENTRY_DEF(p_state_entry,100);
STRING_DEBUG_ONCE_ENTRY_DEF(p_wakeup_entry,100);

X_Boolean isSureQuiet;

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

	String_Debug_Once(PEN_QUIET_DEBUG,p_state_entry,PS_StateJumpDetected,(40,"-----PS_StateJumpDetected\r\n"));
	pbs = PenBasicStateGetUnderCertainState(PBS_Quiet);
	if( pbs == PBS_Quiet)
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
	String_Debug_Once(PEN_QUIET_DEBUG,p_state_entry,PS_Set,(40,"-----PS_Set\r\n"));
	return PS_Get;
}
static PenState GetAction(X_Void)
{
	isSureQuiet = X_True;
	String_Debug_Once(PEN_QUIET_DEBUG,p_state_entry,PS_Get,(40,"-----PS_Get\r\n"));
	return PS_Config;
}
static PenState ConfigAction(X_Void)
{
	// sleep and set motion sensor wakeup enable
	String_Debug_Once(PEN_QUIET_WAKEUP_DEBUG,p_wakeup_entry,1,(40,"-----motion wake en\r\n"));
	String_Debug_Once(PEN_QUIET_DEBUG,p_state_entry,PS_Config,(40,"-----PS_Config\r\n"));
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

X_Void PenQuietStateHandle(X_Void)
{
	uint8_t i, state_number;

	state_number = (sizeof(StateHandle) / sizeof(StateHandle[0]));
	CurrentState = PS_StateJumpDetected;
	if(isSureQuiet == X_True)
	{
		CurrentState = PS_Config;
	}
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
		String_Debug(PEN_QUIET_DEBUG,(30,"unknow state\r\n"));
		CurrentState = PS_StateJumpDetected;
	}

}
X_Void PenQuietStateInit(X_Void)
{
	OnceEntryValueInit(p_wakeup_entry,100);
	isSureQuiet = X_False;
}
/***************************************************/
X_Boolean DoesSureQuiet_TB(X_Void)
{
	return isSureQuiet;
}
X_Void ClearQuietState_TB(X_Void)
{
	isSureQuiet = X_False;
}
