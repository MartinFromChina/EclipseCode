#include "StateMachine.h"

uint8_t StateMachineRun( const StateBasicParam *p_sbp
						,s_StateMachineParam *p_smp
						,X_Boolean (*DoesBreak)(const StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber current_state_going_to_leave,StateNumber next_state_going_to_enter))
{
	uint16_t Counter;
	StateNumber i,j,current_state,previous_state,current_hop_times,current_hop_counter;
	X_Boolean isStateJumpWrong,isReachNullEvent;

	if(p_sbp == X_Null || p_smp == X_Null) {return APP_POINTER_NULL;}
	if(p_sbp->p_Handle == X_Null){return APP_POINTER_NULL;}
	if(p_sbp->AllStateNum > MAX_STATE_NUMBER || p_sbp->AllStateNum == 0) {return APP_BEYOND_SCOPE;}
	if(p_sbp->MaxStateHopTimesInSignalProcess > MAX_STATE_EVENT_NUMBER || p_sbp->MaxStateHopTimesInSignalProcess == 0) {return APP_BEYOND_SCOPE;}
	if((*p_sbp->p_CurrentStateNum) > MAX_STATE_NUMBER)
	{
		(*p_sbp->p_CurrentStateNum) = DEFAULT_STATE_NUMBER;
		return APP_BEYOND_SCOPE;
	}

	Counter = 0;
	current_state = (*p_sbp->p_CurrentStateNum);
	isStateJumpWrong = X_False;
	isReachNullEvent = X_False;

	for(j=0;j<p_sbp->MaxStateHopTimesInSignalProcess;j++)
	{
		Counter ++;
		if((current_state+1) > p_sbp->AllStateNum)
		{
			current_state = DEFAULT_STATE_NUMBER;
			isStateJumpWrong = X_True;
			break;
		}
		current_hop_times = p_sbp ->p_Handle[current_state].current_max_hop_times;
		current_hop_counter = 0;
		for(i=0;i<current_hop_times;i++)
		{
			previous_state = current_state;
			if(p_sbp ->p_Handle[current_state].p_hop[current_hop_counter].onHop != X_Null)
			{
				if(p_sbp ->p_Handle[current_state].p_hop[current_hop_counter].onHop(p_smp) == X_True)
				{
					current_state = p_sbp ->p_Handle[current_state].p_hop[current_hop_counter].NextStateWhenTrue;
				}
				else
				{
					current_state = p_sbp ->p_Handle[current_state].p_hop[current_hop_counter].NextStateWhenFalse;
				}

				if(current_state == CURRENT_STATE_FLAG || current_state == previous_state)
				{
					// no state change ,go on
					current_state = previous_state;
				}
				else
				{
					if(StateRecorder != X_Null) {StateRecorder(previous_state,current_state);}
					break;
				}
			}
			current_hop_counter ++;

		}
		if(DoesBreak == X_Null)
		{
			break;
		}
		else
		{
			if(DoesBreak(p_sbp,current_state,Counter) == X_True) {break;}
		}

	}

	(*p_sbp->p_CurrentStateNum) = current_state;

	if(isStateJumpWrong == X_True || isReachNullEvent == X_True) {return APP_ERROR;}
	return APP_SUCCESSED;

}

uint8_t SimpleStateMachineRun(const StateSimpleParam *p_ssp
						,s_StateMachineParam *p_smp
						,X_Boolean (*DoesBreak)(const StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
						,X_Void(*StateRecorder)(StateNumber current_state,StateNumber next_state))
{
	uint16_t Counter;
	StateNumber i,current_state,previous_state;
	X_Boolean isStateJumpWrong;

	if(p_ssp == X_Null || p_smp == X_Null) {return APP_POINTER_NULL;}
	if(p_ssp->p_Action == X_Null){return APP_POINTER_NULL;}
	if(p_ssp->AllStateNum > MAX_STATE_NUMBER || p_ssp->AllStateNum == 0) {return APP_BEYOND_SCOPE;}
	if((*p_ssp->p_CurrentStateNum) > MAX_STATE_NUMBER)
	{
		(*p_ssp->p_CurrentStateNum) = DEFAULT_STATE_NUMBER;
		return APP_BEYOND_SCOPE;
	}

	Counter = 0;
	current_state = (*p_ssp->p_CurrentStateNum);
	isStateJumpWrong = X_False;

	for(i = 0; i<p_ssp->MaxStateHopTimesInSignalProcess ; i++)
	{
		if(p_ssp ->p_Action[current_state].Action != X_Null)
		{
			previous_state = current_state;
			(*p_ssp->p_CurrentStateNum) = current_state;
			p_smp->current_state = current_state;
			current_state = p_ssp ->p_Action[current_state].Action(p_smp);
			Counter ++;
			if((current_state+1) > p_ssp->AllStateNum)
			{
				current_state = DEFAULT_STATE_NUMBER;
				isStateJumpWrong = X_True;
				break;
			}
			if(previous_state != current_state)
			{
				if(StateRecorder != X_Null) {StateRecorder(previous_state,current_state);}
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
	(*p_ssp->p_CurrentStateNum) = current_state;

	if(isStateJumpWrong == X_True ) {return APP_ERROR;}
	return APP_SUCCESSED;

}

X_Void SimpleStateMachineStateSet(const StateSimpleParam *p_ss,StateNumber state)
{
	if(p_ss == X_Null) {return;}
	*p_ss ->p_CurrentStateNum = state;
}
/*
uint8_t StateMachineSetState(X_Boolean isSimpleMachine,StateNumber state,const X_Void *p_This)
{
	if(p_This == X_Null) {return APP_POINTER_NULL;}
	if(isSimpleMachine == X_True)
	{
		const StateSimpleParam *p_entry = (StateSimpleParam *)p_This;
		if( (state+1) >=  p_entry->AllStateNum ) {return APP_BEYOND_SCOPE;}
		(*p_entry->p_CurrentStateNum) = state;
	}
	else
	{
		const StateBasicParam *p_entry = (StateBasicParam *)p_This;
		if( (state+1) >=  p_entry->AllStateNum ) {return APP_BEYOND_SCOPE;}
		(*p_entry->p_CurrentStateNum) = state;
	}
	return APP_SUCCESSED;

}
*/
