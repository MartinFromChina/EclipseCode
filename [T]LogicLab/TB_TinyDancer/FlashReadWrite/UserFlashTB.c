#include "UserFlashTB.h"
#include "..\..\ScriptsCommandModule\HexCommand.h"
#include "..\..\..\CommonSource\Math\random_number.h"
#include "..\..\..\CommonSource\MulLoopQueue\loop_queues.h"

RTT_DEBUG_ONCE_ENTRY_DEF(p_script_func_num,100);

#define QUEUE_SIZE   10
#define GENERATOR_SPEED  2

SIMPLE_LOOPQUEUE_DEF(p_queue,QUEUE_SIZE);
static uint8_t SectorNumberBuf[QUEUE_SIZE];

static uint8_t user_sector_number = 0,wait_counter = 0,generator_counter = 0;
static X_Boolean isFlashNotReady = X_False;

/***********
 * 0755xxccaabbpp	   :	call number xx function
 * 					cc : 00   pointer to next line
 * 					cc : 01   if xx function return true , pointer to aa line ; else pointer to bb line
 *					pp : parameter to number xx function
 */
/*
 *	1	:FunctionInitial					(1)  00010100
 *	2	:EventGenerator						(2)  01020100
 *
1:command:07550100010100
2:command:07550201020100
3:command:07550301030200
4:end
 */

static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	SEGGER_RTT_Debug_Once(FLASH_DEBUG,p_script_func_num,1,(30,"--ScriptsFunctionInitial \r\n"));
	user_sector_number = 0;
	wait_counter	 = 0;
	isFlashNotReady = X_False;
	generator_counter = 0;
	SimpleQueueInitialize(p_queue);
	return X_True;
}
static X_Boolean EventGenerator(X_Void * p_param)
{
	X_Boolean isOK;
	uint8_t node_number;
	user_sector_number = GetRandomNumber(0,MAX_SECTOR_NUMBER);
	SEGGER_RTT_Debug_Once(FLASH_DEBUG,p_script_func_num,2,(30,"EventGenerator %d\r\n",user_sector_number));

	if(generator_counter < 10) {generator_counter ++;}
	else
	{
		generator_counter = 0;
//		isFlashNotReady = (isFlashNotReady == X_True)? X_False : X_True;
		isFlashNotReady = !isFlashNotReady;
	}

	node_number = SimpleQueueFirstIn(p_queue,&isOK,X_False);
	if(isOK == X_True)
	{
		SectorNumberBuf[node_number] = user_sector_number;
	}
	return X_True;

}

static X_Boolean EventWait(X_Void * p_param)
{
	SEGGER_RTT_Debug_Once(FLASH_DEBUG,p_script_func_num,3,(30,"EventWait \r\n"));
	if(wait_counter > 0)
	{
		wait_counter --;
		return X_True;
	}
	return X_False;
}

static struct _ScriptsFunctionArray
{
	uint8_t function_number;
	X_Boolean(*action)(X_Void * p_param);
}
const ScriptsFunctionArray[] = {
	  {1,ScriptsFunctionInitial},
	  {2,EventGenerator},
	  {3,EventWait},
};

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param)
{
	uint8_t i;
	X_Boolean isOK;

	isOK = X_False;
	for(i=0;i<sizeof(ScriptsFunctionArray)/sizeof(ScriptsFunctionArray[0]);i++)
	{
		if(ScriptsFunctionArray[i].function_number == func_num && ScriptsFunctionArray[i].action != X_Null)
		{
			isOK = ScriptsFunctionArray[i].action(p_param);
			break;
		}
	}
	return isOK;
}

X_Boolean  GetSectorNumber(uint8_t *p_sector)
{
	X_Boolean isOK;
	uint8_t node_number;

	if(p_sector == X_Null) {return X_False;}
	node_number = SimpleQueueFirstOut(p_queue,&isOK);
	if(isOK == X_True)
	{
		*p_sector = SectorNumberBuf[node_number];
		RealseSimpleQueueBuf(p_queue,node_number);
		return X_True;
	}
	*p_sector = SectorNumberBuf[node_number];
	return X_False;
}

X_Boolean DoesFlashReady(X_Void)
{
	return isFlashNotReady;
}
