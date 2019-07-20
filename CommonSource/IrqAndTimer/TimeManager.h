#ifndef __TIMEMANAGER_H
#define __TIMEMANAGER_H

#include "..\KeilMDK.h"
#include "..\AppError.h"
#include "..\CommonMarco.h"

typedef enum
{
	TM_one = 0,
	TM_two,
	TM_three,
	TM_four,
	TM_five,
	TM_six,
	TM_seven,
	TM_eight,
	TM_nine,
	TM_ten,
	TM_MAX,
}TimeManagerID;

X_Boolean TimeManagerAdd(TimeManagerID *id,X_Boolean(*isItTime)(X_Void),X_Void(*ToDo)(X_Void));
uint8_t TimeManagerRelease(TimeManagerID id);
X_Void TimeManagerInit(X_Void);
X_Void TimeManagerHandle(X_Void);

#endif
