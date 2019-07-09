#include "NewPenState.h"
#include "..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\..\CommonSource\AppError.h"

#define STATE_ERROR_DEBUG  1
#define BASIC_STATE_DEBUG  1
STRING_DEBUG_ONCE_ENTRY_DEF(error_entry,100);
STRING_DEBUG_ONCE_ENTRY_DEF(state_entry,100);

typedef enum
{
	Idle 				= 0,
	WakeUp 				= 1,
	NormalProcess 		= 2,
	Sleepy 				= 3,
	ShutDown 			= 4,
}PenState;

static StateNumber IdleAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,Idle,(30,"--Idle\r\n"));
	return WakeUp;
}

static StateNumber WakeUpRoutineAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,WakeUp,(30,"--WakeUpRoutine\r\n"));
	return WakeUp;
}

static StateNumber test1(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,WakeUp+1,(30,"--test1\r\n"));
	return ShutDown;
}
static StateNumber test2(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,Sleepy,(30,"--test2\r\n"));
	return Idle;
}
static StateNumber test3(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,ShutDown,(30,"--test3\r\n"));
	return Sleepy;
}
static const StateHandle PenStateHandle[5] = {

		{
		1,
		{{IdleAction}},
		}, // Idle

		{
		3,
		{{WakeUpRoutineAction},{test1},{test1}},
		},//WakeUp

		{
		2,
		{{test2},{test2},{test2},{test2},{test2},{test2}}
		},

		{
		4,
		{{test2},{test2},{test2},{test2}},
		},

		{
		3,
		{{test3},{test3},{test3},{test3},{test3},{test3}},
		},

};


APP_STATE_MACHINE_DEF(pen_state,5,4,&PenStateHandle[0]);

static X_Boolean DoesStop(StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if(loop_counter > MAX_STATE_EVENT_NUMBER) {return X_True;}
	if(p_sbp->CurrentStateNum != nextstate){return X_True;}
	return X_False;
}
X_Void StateJumpRecorder(StateNumber state)
{
	// current state number is state and latest state number != state
	String_Debug(STATE_ERROR_DEBUG,(30,"new state number %d\r\n",state));
}
X_Void AllStateAndEventAction(X_Void)
{
	uint8_t error_code;

	error_code = StateMachineRun(&pen_state,X_True,DoesStop,StateJumpRecorder);
	String_Debug_Once(STATE_ERROR_DEBUG,error_entry,error_code,(30,"%s\r\n",StringErrorTranslate(error_code,AppErrorGet)));
}

