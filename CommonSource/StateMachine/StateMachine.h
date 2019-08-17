#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "..\KeilMDK.h"
#include "..\AppError.h"
#include "..\CommonMarco.h"

#define MAX_STATE_NUMBER    		8
#define MAX_STATE_EVENT_NUMBER    	6
#define DEFAULT_STATE_NUMBER        0

typedef uint8_t 			StateNumber;
typedef  struct
{
	StateNumber current_state;
}s_StateMachineParam;

typedef  struct {
	StateNumber (*Action)(s_StateMachineParam *p_this);
}StateAction;
typedef struct {
	StateAction SAction[MAX_STATE_EVENT_NUMBER];
}StateHandle;

typedef struct
{
	const StateNumber 	AllStateNum;
	const uint8_t     	MaxEventNum;
	StateHandle const 	*p_Handle;
	StateNumber 		*p_CurrentStateNum;
}StateBasicParam;

#define APP_STATE_MACHINE_DEF(id,state_number,event_number,p_handle)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static const StateBasicParam    CONCAT_2(id, _entry) = {state_number,event_number,p_handle,&CONCAT_2(id, _current_state_number)}; \
static const StateBasicParam* id = &CONCAT_2(id, _entry)

uint8_t StateMachineRun( const StateBasicParam *p_sbp
						,s_StateMachineParam *p_smp
						,X_Boolean isNullEventForbid
						,X_Boolean (*DoesBreak)(const StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber current_state,StateNumber next_state));

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

//uint8_t StateMachineSetState(X_Boolean isSimpleMachine,StateNumber state,const X_Void *p_This);

/*
 * static s_StateMachineParam s_SMP;
 *
 static const StateHandle ExampleStateHandle[2] = {

		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},  // if the position is empty , the pointer's init value is X_Null?
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
};


APP_STATE_MACHINE_DEF(p_example_state,2,3,&ExampleStateHandle[0]);

static X_Void StateJumpRecorder(StateNumber current_state,StateNumber next_state)
{
	// going to jump new state:next_state
}

StateMachineRun(p_example_state,&s_SMP,X_False,X_Null,StateJumpRecorder);

 */

/*
static const StateAction SimpleStateAction[5] = {
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

APP_SIMPLE_STATE_MACHINE_DEF(p_simple_state,5,2,&SimpleStateAction[0]);



typedef struct
{
	s_StateMachineParam base;
	X_Boolean UserFlag;
}sParamExtern;

static sParamExtern sPE;

error_code = SimpleStateMachineRun(p_simple_state,&sPE.base,DoesBreakSimple,StateJumpRecorder);
 */

#endif
