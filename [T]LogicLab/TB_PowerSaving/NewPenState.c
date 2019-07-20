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
		{{IdleAction},
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},},
		}, // Idle

		{
		{{WakeUpRoutineAction},{test1},{test1},{X_Null},{X_Null},{X_Null}},
		},//WakeUp

		{
		{{test2},{test2},{test2},{test2},{test2},{test2}}
		},

		{
		{{test2},{test2},{test2},{test2},{X_Null},{X_Null}},
		},

		{
		{{test3},{test3},{test3},{test3},{test3},{test3}},
		},

};


APP_STATE_MACHINE_DEF(pen_state,5,4,&PenStateHandle[0]);



typedef enum
{
	SimpleIdle 				= 0,
	SimpleWakeUp 				= 1,
	SimpleNormalProcess 		= 2,
	SimpleSleepy 				= 3,
	SimpleShutDown 			= 4,
}PenSimpleState;



static StateNumber SimpleIdleAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,SimpleIdle,(30,"--SimpleIdle\r\n"));
	return SimpleWakeUp;
}
static StateNumber SimpleWakeUpAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,SimpleWakeUp,(30,"--SimpleWakeUp\r\n"));
	return SimpleNormalProcess;
}
static StateNumber SimpleNormalProcessAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,SimpleNormalProcess,(30,"--SimpleNormalProcess\r\n"));
	return SimpleSleepy;
}
static StateNumber SimpleSleepyAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,SimpleSleepy,(30,"--SimpleSleepy\r\n"));
	return SimpleShutDown;
}
static StateNumber SimpleShutDownAction(StateNumber current_state)
{
	String_Debug_Once(BASIC_STATE_DEBUG,state_entry,SimpleShutDown,(30,"--SimpleShutDown\r\n"));
	return SimpleIdle;
}



static const StateAction PenStateAction[5] = {
		{SimpleIdleAction},
		{SimpleWakeUpAction},
		{SimpleNormalProcessAction},
		{SimpleSleepyAction},
		{SimpleShutDownAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(pen_simple_state,5,2,&PenStateAction[0]);

X_Void StateJumpRecorder(StateNumber state)
{
	// current state number is state and latest state number != state
//	String_Debug(STATE_ERROR_DEBUG,(30,"new state number %d\r\n",state));
}

static X_Boolean DoesBreakSimple(StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if(loop_counter > 2) {return X_True;}
	return X_False;
}

X_Void AllStateAndEventAction(X_Void)
{
	uint8_t error_code;

//	error_code = StateMachineRun(&pen_state,X_True,X_Null,StateJumpRecorder);
	error_code = SimpleStateMachineRun(&pen_simple_state,DoesBreakSimple,StateJumpRecorder);
	String_Debug_Once(STATE_ERROR_DEBUG,error_entry,error_code,(30,"%s\r\n",StringErrorTranslate(error_code,AppErrorGet)));
}
