#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "..\KeilMDK.h"
#include "..\AppError.h"

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
	StateNumber CurrentStateNum;
	StateNumber AllStateNum;
	uint8_t     MaxEventNum;
	StateHandle const *p_Handle;
}StateBasicParam;

#define APP_STATE_MACHINE_DEF(id,state_number,event_number,p_handle)                \
static StateBasicParam id = {DEFAULT_STATE_NUMBER,state_number,event_number,p_handle}

uint8_t StateMachineRun(StateBasicParam *p_sbp
						,X_Boolean isNullEventForbid
						,X_Boolean (*DoesBreak)(StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber state));

typedef struct
{
	StateNumber CurrentStateNum;
	StateNumber AllStateNum;
	uint16_t MaxStateHopTimesInSignalProcess;
	StateAction const *p_Action;
}StateSimpleParam;

#define APP_SIMPLE_STATE_MACHINE_DEF(id,state_number,max_signal_loop_times,p_action)                \
static StateSimpleParam id = {DEFAULT_STATE_NUMBER,state_number,max_signal_loop_times,p_action}


uint8_t SimpleStateMachineRun(StateSimpleParam *p_ssp
						,X_Boolean (*DoesBreak)(StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber state));
/*
 static const StateHandle ExampleStateHandle[2] = {

		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},  // if the position is empty , the pointer's init value is X_Null?
		{{{X_Null},{X_Null},{X_Null},{X_Null},{X_Null},{X_Null}}},
};


APP_STATE_MACHINE_DEF(example_state,2,3,&ExampleStateHandle[0]);

X_Void StateJumpRecorder(StateNumber state_num)
{
	// going to jump new state:state_num
}

StateMachineRun(&example_state,X_False,X_Null,StateJumpRecorder);

 */

/*
static const StateAction SimpleStateAction[5] = {
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
		{X_Null},
};

APP_SIMPLE_STATE_MACHINE_DEF(simple_state,5,2,&SimpleStateAction[0]);

error_code = SimpleStateMachineRun(&simple_state,DoesBreakSimple,StateJumpRecorder);
 */

#endif
