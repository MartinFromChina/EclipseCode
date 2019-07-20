#include "TimeManager.h"

typedef struct
{
	X_Boolean isFree;
	X_Boolean(*isItTime)(X_Void);
	X_Void(*ToDo)(X_Void);
}sTimeManager;

static sTimeManager sTM[TM_MAX];

X_Boolean TimeManagerAdd(TimeManagerID *id,X_Boolean(*isItTime)(X_Void),X_Void(*ToDo)(X_Void))
{
	uint8_t i;
	X_Boolean isOK;

	isOK = X_False;
	*id  = TM_MAX;
	for(i=0;i<TM_MAX;i++)
	{
		if(sTM[i].isFree == X_True)
		{
			*id = (TimeManagerID)i;
			sTM[i].isFree = X_False;
			sTM[i].isItTime = isItTime;
			sTM[i].ToDo = ToDo;
			isOK = X_True;
			break;
		}
	}
	return isOK;
}
uint8_t TimeManagerRelease(TimeManagerID id)
{
	if(id >= TM_MAX) {return APP_BEYOND_SCOPE;}
	sTM[id].isFree = X_True;
	return APP_SUCCESSED;
}
X_Void TimeManagerInit(X_Void)
{
	uint8_t i;
	for(i=0;i<TM_MAX;i++)
	{
		sTM[i].isFree = X_True;
	}
}
X_Void TimeManagerHandle(X_Void)
{
	uint8_t i;
	X_Boolean isTimeCome;
	for(i=0;i<TM_MAX;i++)
	{
		if(sTM[i].isFree == X_False)
		{
			isTimeCome = X_False;
			if(sTM[i].isItTime != X_Null)
			{
				if(sTM[i].isItTime() == X_True){isTimeCome = X_True;}
				else {isTimeCome = X_False;}
			}
			else
			{
				isTimeCome = X_True;
			}
			if(isTimeCome == X_True)
			{
				if(sTM[i].ToDo != X_Null)
				{
					sTM[i].ToDo();
				}
			}
		}
	}
}

