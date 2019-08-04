#include "ButtonModule.h"
#include "..\..\CommonSource\Math\bit_operation.h"
/*************************************************************************/
typedef enum
{
	MouseMode = 0, // click ,double click , long push
	KeyBoardMode ,  // click , continous click , long push
}ButtonOperationMode;
typedef struct
{
	uint32_t isClick;
	uint32_t isContinusClick;
	uint32_t isDoubleClick;
	uint32_t isLongpush;
	uint32_t isLongpushRelease;
}sButtonParam;

static X_Boolean isCurrentButtonPushed = X_False;
static CombineButtonValue CurrentButtonValue = 0;
static uint8_t CurrentButtonNumber = 0;
static uint16_t CurrentButtonConfigMode = 0;
static ButtonOperationMode CurrentOM[MAX_BUTTON_NUMBER];


StateNumber CustomizedBM_InitAction(StateNumber current_state)
{
	CurrentOM[CurrentButtonNumber] = (twobyte_getbit(CurrentButtonConfigMode,CurrentButtonNumber) == 0 ) ? MouseMode : KeyBoardMode;
	return current_state;
}

X_Void ButtonStateMonitor(const sButtonModuleExtern *p_sbm,CombineButtonValue *value)
{
	uint8_t i;

	CurrentButtonValue = p_sbm->base->get_value();
	*value = CurrentButtonValue;
	for(i=0;i<p_sbm->button_number;i++)
	{
		CurrentButtonNumber = i;
		isCurrentButtonPushed = (twobyte_getbit(CurrentButtonValue,CurrentButtonNumber) == 1) ? X_True : X_False;
		SimpleStateMachineRun(p_sbm->p_monitor[i],X_Null,X_Null);
	}
}
/*
#define FourByteMask							((uint32_t)(1u << 0))
static uint8_t byte4_getbit(uint32_t source,uint8_t bitnumber)
{
	if((source & (FourByteMask << bitnumber)) != 0)
	{
		return 1;
	}
	return 0;
}*/
