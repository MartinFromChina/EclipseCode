#ifndef __TIMEMANAGEREXTERN_H
#define __TIMEMANAGEREXTERN_H

#include "..\KeilMDK.h"
#include "..\AppError.h"

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

typedef struct sTimeManagerBasic s_TimeManagerBasic;

typedef struct
{
	uint32_t counter;
	X_Void(* ToDo)(s_TimeManagerBasic *p_This);
}sTimeManagerBasic;

X_Boolean TimeManagerExternAdd(TimeManagerID *id,sTimeManagerBasic *p_This,X_Void(*ToDo)(sTimeManagerBasic *p_This));
uint8_t TimeManagerExternRelease(TimeManagerID id);
uint32_t TimeManagerGetBasicValue(TimeManagerID id);
X_Void TimeManagerSetBasicValue(TimeManagerID id,uint32_t value);
X_Void TimeManagerExternInit(X_Void);
X_Void TimeManagerExternHandle(X_Void);

#endif
