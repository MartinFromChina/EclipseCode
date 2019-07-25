#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "..\KeilMDK.h"
#include "..\AppError.h"
#include "..\CommonMarco.h"

#define MAX_STATE_NUMBER    		8
#define MAX_STATE_EVENT_NUMBER    	6
#define DEFAULT_STATE_NUMBER        0

typedef uint8_t 			StateNumber;

typedef  struct {
	StateNumber (*Action)(StateNumber current_state);
}StateAction;
typedef struct {
	StateAction SAction[MAX_STATE_EVENT_NUMBER];
}StateHandle;

typedef struct
{
	const StateNumber 	AllStateNum;
	const uint8_t     	MaxEventNum;
	StateHandle const 	*p_Handle;
	StateNumber 		CurrentStateNum;
}StateBasicParam;

#define APP_STATE_MACHINE_DEF(id,state_number,event_number,p_handle)                \
static StateBasicParam    CONCAT_2(id, _entry) = {state_number,event_number,p_handle,DEFAULT_STATE_NUMBER}; \
static StateBasicParam* id = &CONCAT_2(id, _entry)

uint8_t StateMachineRun( StateBasicParam *p_sbp
						,X_Boolean isNullEventForbid
						,X_Boolean (*DoesBreak)(StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber state));

typedef struct
{
	const StateNumber 	AllStateNum;
	const uint16_t 		MaxStateHopTimesInSignalProcess;
	StateAction const 	*p_Action;
	StateNumber 		CurrentStateNum;
}StateSimpleParam;

#define APP_SIMPLE_STATE_MACHINE_DEF(id,state_number,max_signal_loop_times,p_action)                \
static StateSimpleParam CONCAT_2(id, _entry) = {state_number,max_signal_loop_times,p_action,DEFAULT_STATE_NUMBER}; \
static StateSimpleParam* id = &CONCAT_2(id, _entry)


uint8_t SimpleStateMachineRun( StateSimpleParam *p_ssp
						,X_Boolean (*DoesBreak)(StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber state));
/*
 static const StateHandle ExampleStateHandle[2] = {

		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},  // if the position is empty , the pointer's init value is X_Null?
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
};


APP_STATE_MACHINE_DEF(p_example_state,2,3,&ExampleStateHandle[0]);

X_Void StateJumpRecorder(StateNumber state_num)
{
	// going to jump new state:state_num
}

StateMachineRun(p_example_state,X_False,X_Null,StateJumpRecorder);

 */

/*
static const StateAction SimpleStateAction[5] = {
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_simple_state,5,2,&SimpleStateAction[0]);

error_code = SimpleStateMachineRun(p_simple_state,DoesBreakSimple,StateJumpRecorder);
 */

#endif
