#include "StateMachine.h"

static X_Boolean DoesBreakDefault(StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if(loop_counter > MAX_STATE_EVENT_NUMBER) {return X_True;}
	if(p_sbp->CurrentStateNum != nextstate){return X_True;}
	return X_False;
}

uint8_t StateMachineRun( StateBasicParam *p_sbp
						,X_Boolean isNullEventForbid
						,X_Boolean (*DoesBreak)(StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber state))
{
	uint16_t Counter;
	StateNumber i,current_state,previous_state;
	X_Boolean isStateJumpWrong,isReachNullEvent;

	if(p_sbp == X_Null) {return APP_POINTER_NULL;}
	if(p_sbp->p_Handle == X_Null){return APP_POINTER_NULL;}
	if(p_sbp->AllStateNum > MAX_STATE_NUMBER || p_sbp->AllStateNum == 0) {return APP_BEYOND_SCOPE;}
	if(p_sbp->MaxEventNum > MAX_STATE_EVENT_NUMBER || p_sbp->MaxEventNum == 0) {return APP_BEYOND_SCOPE;}
	if(p_sbp->CurrentStateNum > MAX_STATE_NUMBER)
	{
		p_sbp->CurrentStateNum = DEFAULT_STATE_NUMBER;
		return APP_BEYOND_SCOPE;
	}

	Counter = 0;
	current_state = p_sbp->CurrentStateNum;
	isStateJumpWrong = X_False;
	isReachNullEvent = X_False;

	for(i = 0; i<p_sbp->MaxEventNum ; i++)
	{
		if(p_sbp ->p_Handle[current_state].SAction[i].Action != X_Null)
		{
			previous_state = current_state;
			current_state = p_sbp ->p_Handle[current_state].SAction[i].Action(current_state);
			Counter ++;
			if((current_state+1) > p_sbp->AllStateNum)
			{
				current_state = DEFAULT_STATE_NUMBER;
				isStateJumpWrong = X_True;
				break;
			}
			if(previous_state != current_state)
			{
				i = 0; // !!!
				if(StateRecorder != X_Null) {StateRecorder(current_state);}
			}
		}
		else
		{
			if(isNullEventForbid == X_True)
			{
				current_state = DEFAULT_STATE_NUMBER;
				isReachNullEvent = X_True;
				break;
			}
		}

		if(DoesBreak == X_Null)
		{
			if(DoesBreakDefault(p_sbp,current_state,Counter) == X_True) {break;}
		}
		else
		{
			if(DoesBreak(p_sbp,current_state,Counter) == X_True) {break;}
		}

	}
	p_sbp->CurrentStateNum = current_state;

	if(isStateJumpWrong == X_True || isReachNullEvent == X_True) {return APP_ERROR;}
	return APP_SUCCESSED;
}

uint8_t SimpleStateMachineRun(StateSimpleParam *p_ssp
						,X_Boolean (*DoesBreak)(StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber state))
{
	uint16_t Counter;
	StateNumber i,current_state,previous_state;
	X_Boolean isStateJumpWrong;

	if(p_ssp == X_Null) {return APP_POINTER_NULL;}
	if(p_ssp->p_Action == X_Null){return APP_POINTER_NULL;}
	if(p_ssp->AllStateNum > MAX_STATE_NUMBER || p_ssp->AllStateNum == 0) {return APP_BEYOND_SCOPE;}
	if(p_ssp->CurrentStateNum > MAX_STATE_NUMBER)
	{
		p_ssp->CurrentStateNum = DEFAULT_STATE_NUMBER;
		return APP_BEYOND_SCOPE;
	}

	Counter = 0;
	current_state = p_ssp->CurrentStateNum;
	isStateJumpWrong = X_False;

	for(i = 0; i<p_ssp->MaxStateHopTimesInSignalProcess ; i++)
	{
		if(p_ssp ->p_Action[current_state].Action != X_Null)
		{
			previous_state = current_state;
			current_state = p_ssp ->p_Action[current_state].Action(current_state);
			Counter ++;
			if((current_state+1) > p_ssp->AllStateNum)
			{
				current_state = DEFAULT_STATE_NUMBER;
				isStateJumpWrong = X_True;
				break;
			}
			if(previous_state != current_state)
			{
				if(StateRecorder != X_Null) {StateRecorder(current_state);}
			}
		}

		if(DoesBreak == X_Null)
		{
			break;
		}
		else
		{
			if(DoesBreak(p_ssp,current_state,Counter) == X_True) {break;}
		}

	}
	p_ssp->CurrentStateNum = current_state;

	if(isStateJumpWrong == X_True ) {return APP_ERROR;}
	return APP_SUCCESSED;

}
