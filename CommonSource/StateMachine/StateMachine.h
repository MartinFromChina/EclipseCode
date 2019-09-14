#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "..\KeilMDK.h"
#include "..\AppError.h"
#include "..\CommonMarco.h"

#define MAX_STATE_NUMBER    		20
#define MAX_STATE_EVENT_NUMBER    	6
#define DEFAULT_STATE_NUMBER        0
#define CURRENT_STATE_FLAG          0xff

typedef uint8_t 			StateNumber;

typedef  struct
{
	StateNumber current_state;
}s_StateMachineParam;

/***************************************state machine***************************************************************/



typedef struct
{
	X_Boolean  (*onHop)(s_StateMachineParam *p_this);
	StateNumber const NextStateWhenTrue;
	StateNumber const NextStateWhenFalse;
}sHopHandle;

typedef struct {
	StateNumber const 	current_state_number;
	uint8_t     const   current_max_hop_times;
	sHopHandle  const    *p_hop;
}StateHandle;

typedef struct
{
	const StateNumber 	AllStateNum;
	const uint16_t     	MaxStateHopTimesInSignalProcess;
	StateHandle const 	*p_Handle;
	StateNumber 		*p_CurrentStateNum;
}StateBasicParam;

#define APP_STATE_MACHINE_DEF(id,state_number,event_number,p_handle)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static const StateBasicParam    CONCAT_2(id, _entry) = {state_number								\
														,event_number								\
														,p_handle									\
														,&CONCAT_2(id, _current_state_number)};		\
static const StateBasicParam* id = &CONCAT_2(id, _entry)

uint8_t StateMachineRun( const StateBasicParam *p_sbp
						,s_StateMachineParam *p_smp
						,X_Boolean (*DoesBreak)(const StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber current_state_going_to_leave,StateNumber next_state_going_to_enter));

X_Void StateMachineStateSet(const StateBasicParam *p_sbp,StateNumber state);

/*************************************simple state machine*****************************************************************/

typedef  struct {
	StateNumber (*Action)(s_StateMachineParam *p_this);
}StateAction;
typedef struct
{
	const StateNumber 	AllStateNum;
	const uint16_t 		MaxStateHopTimesInSignalProcess;
	StateAction const 	*p_Action;
    StateNumber         *p_CurrentStateNum;
}StateSimpleParam;

#define APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(id_entry,state_number,max_signal_loop_times,p_action)                \
static StateNumber CONCAT_2(id_entry, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static const StateSimpleParam CONCAT_2(id_entry, _entry) = {state_number,max_signal_loop_times,p_action,&CONCAT_2(id_entry, _current_state_number)}

#define APP_SIMPLE_STATE_MACHINE_DEF(id,state_number,max_signal_loop_times,p_action)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static const StateSimpleParam CONCAT_2(id, _entry) = {state_number,max_signal_loop_times,p_action,&CONCAT_2(id, _current_state_number)}; \
static const StateSimpleParam* id = &CONCAT_2(id, _entry)


uint8_t SimpleStateMachineRun( const StateSimpleParam *p_ssp
						,s_StateMachineParam *p_smp
						,X_Boolean (*DoesBreak)(const StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber current_state,StateNumber next_state));

X_Void SimpleStateMachineStateSet(const StateSimpleParam *p_ss,StateNumber state);

/*
 * static s_StateMachineParam s_SMP;
 *



APP_STATE_MACHINE_DEF(p_example_state,sizeof(ExampleStateHandle)/sizeof(ExampleStateHandle[0]),3,&ExampleStateHandle[0]);

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_come)
{
	// going to jump new state:next_state
}

StateMachineRun(p_example_state,&s_SMP,X_False,X_Null,StateJumpRecorder);

 */

/*
static const StateAction SimpleStateAction[] = {
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
};

static X_Boolean DoesBreakSimple(const StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if(loop_counter > 3) {return X_True;}
	return X_False;
}

APP_SIMPLE_STATE_MACHINE_DEF(p_simple_state,sizeof(SimpleStateAction)/sizeof(SimpleStateAction[0]),2,&SimpleStateAction[0]);


typedef struct
{
	s_StateMachineParam base;
	X_Boolean UserFlag;
}sParamExtern;

static sParamExtern sPE;

error_code = SimpleStateMachineRun(p_simple_state,&sPE.base,DoesBreakSimple,StateJumpRecorder);

*/
/*
******************************************************
example code
******************************************************
typedef struct
{
	s_StateMachineParam 		base;
	StateNumber 						StateBackupWhenSuccessed;
	StateNumber 						StateBackupWhenFailed;
	uint16_t 								wait_counter;
}sParamExtern;

static sParamExtern sPE;

typedef enum
{
	RWC_Idle = 0,
}RWC_state;

**********************************************************************************************************************************
			0
*********************************************************************************************************************************
static StateNumber RWC_IdleAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}

static const StateAction SimpleStateAction[] = {
		{RWC_IdleAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_simple_state
														,sizeof(SimpleStateAction)/sizeof(SimpleStateAction[0])
														,1
														,&SimpleStateAction[0]);


X_Void NoName(X_Void)
{
	SimpleStateMachineRun(p_simple_state,&sPE.base,X_Null,X_Null);
}
 */

#endif
