#include "TimeManagerExtern.h"

static sTimeManagerBasic *p_sTMB[TM_MAX];

X_Boolean TimeManagerExternAdd(TimeManagerID *id,sTimeManagerBasic *p_This,X_Void(*ToDo)(sTimeManagerBasic *p_This))
{
	uint8_t i;
	X_Boolean isOK;

	isOK = X_False;
	*id  = TM_MAX;
	for(i=0;i<TM_MAX;i++)
	{
		if(p_sTMB[i] != X_Null)
		{
			*id = (TimeManagerID)i;
			p_sTMB[i]->counter = 0;
			p_sTMB[i]->ToDo = ToDo;
			isOK = X_True;
			break;
		}
	}
	return isOK;
}
uint8_t TimeManagerExternRelease(TimeManagerID id)
{
	if(id >= TM_MAX) {return APP_BEYOND_SCOPE;}
	p_sTMB[id] = X_Null;
	return APP_SUCCESSED;
}

X_Void TimeManagerSetBasicValue(TimeManagerID id,uint32_t value)
{
	if(id >= TM_MAX) {return;}
	if(p_sTMB[id] == X_Null) {return;}

	p_sTMB[id]->counter = value;
}

uint32_t TimeManagerGetBasicValue(TimeManagerID id)
{
	if(id >= TM_MAX) {return 0;}
	if(p_sTMB[id] == X_Null) {return 0;}
	return (p_sTMB[id]->counter);
}

X_Void TimeManagerExternInit(X_Void)
{
	uint8_t i;
	for(i=0;i<TM_MAX;i++)
	{
		p_sTMB[i] = X_Null;
	}
}
X_Void TimeManagerExternHandle(X_Void)
{
	uint8_t i;

	for(i=0;i<TM_MAX;i++)
	{
		if(p_sTMB[i] != X_Null)
		{
			if(p_sTMB[i]->counter > 0) {p_sTMB[i]->counter --;}
			if(p_sTMB[i]->ToDo != X_Null)
			{
				p_sTMB[i]->ToDo(p_sTMB[i]);
			}
		}
	}
}

