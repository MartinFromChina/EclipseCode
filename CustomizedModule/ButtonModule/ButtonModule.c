#include "ButtonModule.h"
/*************************************************************************/
static CombineButtonValue CurrentButtonValue = 0;

StateNumber CustomizedBM_InitAction(StateNumber current_state)
{
	return current_state;
}

X_Void ButtonStateMonitor(const sButtonModuleExtern *p_sbm,CombineButtonValue *value)
{
	uint8_t i;

	CurrentButtonValue = p_sbm->base->get_value();
	*value = CurrentButtonValue;
	for(i=0;i<p_sbm->button_number;i++)
	{
		SimpleStateMachineRun(p_sbm->p_monitor[i],X_Null,X_Null);
	}
}

