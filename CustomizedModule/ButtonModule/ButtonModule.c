#include "ButtonModule.h"
/*************************************************************************/


StateNumber CustomizedBM_InitAction(StateNumber current_state)
{
	return current_state;
}

X_Void ButtonStateMonitor(const sButtonModuleExtern *p_sbm,CombineButtonValue value)
{
//	SimpleStateMachineRun(p_button_state,X_Null,X_Null);
	uint8_t i;
	for(i=0;i<p_sbm->button_number;i++)
	{
		SimpleStateMachineRun(p_sbm->p_monitor[i],X_Null,X_Null);
	}
}

