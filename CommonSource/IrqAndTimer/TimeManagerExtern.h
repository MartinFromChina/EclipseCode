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

typedef struct s_TimeManagerBasic sTimeManagerBasic;

struct s_TimeManagerBasic
{
	uint32_t counter;
	X_Void(* ToDo)(sTimeManagerBasic *p_This);
};

X_Boolean TimeManagerExternAdd(TimeManagerID *id,sTimeManagerBasic *p_This,X_Void(*ToDo)(sTimeManagerBasic *p_This));
uint8_t TimeManagerExternRelease(TimeManagerID id);
uint32_t TimeManagerGetBasicValue(TimeManagerID id);
X_Void TimeManagerSetBasicValue(TimeManagerID id,uint32_t value);
X_Void TimeManagerExternInit(X_Void);
X_Void TimeManagerExternHandle(X_Void);

/*************************************************
 * use this module two ways below:
 *
 *
 *if (just use one counter value)
  {
	 static TimeManagerID mTM_ID = TM_MAX;
	 static sTimeManagerBasic my_sTMB;

	 TimeManagerExternAdd(&mTM_ID,&my_sTMB,X_Null);
	 TimeManagerSetBasicValue(mTM_ID,1000);
	 TimeManagerGetBasicValue(mTM_ID);
	 TimeManagerExternRelease(mTM_ID);
  }

 *
 *
  else if (use extern time manager function)
  {

	typedef struct  s_TimerManagerExtern sTimerManagerExtern;
	struct s_TimerManagerExtern
	{
		sTimeManagerBasic base;
		uint32_t counterExtern;
	};

	static TimeManagerID mTM_ID = TM_MAX;
	static sTimerManagerExtern sTME;
	static uint32_t externvalue;

	static X_Void ManagerAction(sTimerManagerExtern *p_This)
	{
		sTimerManagerExtern *s_mte_copy  = (sTimerManagerExtern*)(p_this);
		if(p_this->counter > 0) {p_this->counter--;}
		s_mte_copy->counterExtern ++;
	}

	static uint32_t GetExternCounter(sTimerManagerExtern *p_This)
	{
		return p_This->counterExtern;
	}

	TimeManagerExternAdd(&mTM_ID,&sTME.base,ManagerAction);
	TimeManagerExternRelease(mTM_ID);
	TimeManagerGetBasicValue(mTM_ID);
	TimeManagerSetBasicValue(mTM_ID,1000);
	externvalue = GetExternCounter(&sTME);

  }
 */

#endif
