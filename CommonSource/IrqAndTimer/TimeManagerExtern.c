#include "TimeManagerExtern.h"

static sTimeManagerBasic *p_sTMB[TM_MAX];
static uint8_t EntryCounter = 0;

uint8_t TimeManagerExternAdd(TimeManagerID *id,sTimeManagerBasic *p_This,X_Void(*ToDo)(sTimeManagerBasic *p_This))
{
	uint8_t i;
	X_Boolean isOK,isAlreadyInList;

	*id  = TM_MAX;
	if(EntryCounter >= TM_MAX) {return APP_BEYOND_SCOPE;}

	isOK = X_False;
	isAlreadyInList = X_False;

	for(i=0;i<TM_MAX;i++)
	{
		if(p_sTMB[i] != X_Null)
		{
			if(p_sTMB[i] == p_This)
			{
				isAlreadyInList = X_True;
				break;
			}
		}
	}

	if(isAlreadyInList == X_True ) {return APP_ALREADY_DONE_BEFORE;}

	for(i=0;i<TM_MAX;i++)
	{
		if(p_sTMB[i] == X_Null)
		{
			*id = (TimeManagerID)i;
			p_sTMB[i] = p_This;
			p_sTMB[i]->counter = 0;
			p_sTMB[i]->ToDo = ToDo;
			isOK = X_True;
			EntryCounter ++;
			break;
		}
	}
	if(isOK == X_True){return APP_SUCCESSED;}
	else {return APP_ERROR;}
}
uint8_t TimeManagerExternRelease(TimeManagerID id)
{
	if(id >= TM_MAX) {return APP_BEYOND_SCOPE;}
	if(p_sTMB[id] != X_Null)
	{
		p_sTMB[id] = X_Null;
		if(EntryCounter > 0) {EntryCounter--;}
		return APP_SUCCESSED;
	}
	return APP_ALREADY_DONE_BEFORE;


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
	EntryCounter = 0;
}
X_Void TimeManagerExternHandle(X_Void)
{
	uint8_t i;

	if(EntryCounter == 0) {return;}

	for(i=0;i<TM_MAX;i++)
	{
		if(p_sTMB[i] != X_Null)
		{
			if(p_sTMB[i]->ToDo != X_Null)
			{
				p_sTMB[i]->ToDo(p_sTMB[i]);
			}
			else
			{
				if(p_sTMB[i]->counter > 0) {p_sTMB[i]->counter --;}
			}
		}
	}
}
uint8_t GetTimeManagerNumber(X_Void)
{
	return EntryCounter;
}
