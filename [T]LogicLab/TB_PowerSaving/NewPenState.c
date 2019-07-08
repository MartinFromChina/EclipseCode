#include "NewPenState.h"
#include "..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\..\CommonSource\AppError.h"

#define STATE_ERROR_DEBUG  1
STRING_DEBUG_ONCE_ENTRY_DEF(error_entry,100);

static const StateHandle PenStateHandle[5][3] = {
		{
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		},
		{
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		},
		{
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		},
		{
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		},
		{
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
		},
};


APP_STATE_MACHINE_DEF(pen_state,5,3,&PenStateHandle[0][0]);

static X_Boolean DoesStop(StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if(loop_counter > MAX_STATE_EVENT_NUMBER) {return X_True;}
	if(p_sbp->CurrentStateNum != nextstate){return X_True;}
	return X_False;
}

X_Void AllStateAndEventAction(X_Void)
{
	uint8_t error_code;
	error_code = StateMachineRun(&pen_state,X_True,DoesStop,X_Null);
	String_Debug_Once(STATE_ERROR_DEBUG,error_entry,error_code,(30,"%s\r\n",StringErrorTranslate(error_code,AppErrorGet)));
}

