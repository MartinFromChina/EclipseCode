#include "UserFlash.h"
#include "UserFlashTB.h"
#include "..\..\..\CustomizedModule\FlashReadWrite\FlashEventHandle.h"

static X_Void FlashTestDebugInit(X_Void);
static X_Void onFlashDebug(eFlashDebugOperation op,uint8_t operation_ID,const sMyFlashEventHandler * p_handler);
static uint32_t DebugParamCollect(eSimpleQueueOperation op,uint32_t param);

//APP_FLASH_EVENT_HANDLER_DEFINE(p_flash_handler
//								,0,0
//								,0,0
//								,0,0
//								,,X_Null,X_Null,X_Null,X_Null,X_Null
//								,onFlashDebug,DebugParamCollect
//								);

//APP_PRIORITY_QUEUE_DEF(p_flash_handler
//						,20
//						,2000
//						,X_True
//						,X_Null,X_Null);

APP_PRIORITY_QUEUE_DEF(p_priority_queue,5,450,X_False,X_Null,X_Null);

FILE * FlashTestOpenFile(void)
{
//	mFlashEventInit(p_flash_handler);
	FlashTestDebugInit();
	mPriorityQueueInitialize(p_priority_queue);
	return fopen(".//TB_TinyDancer//FlashReadWrite//command.txt", "r");
}
static uint16_t debug_counter = 0;
static X_Boolean isJustReady = X_True;
X_Void onTick(X_Void)
{
	X_Boolean isOK;
	uint16_t node_num,priority,high,low,node_count;
	uint8_t sector_number;
	if(GetSectorNumber(&sector_number) == X_True)
	{
		SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60,"**%d**flash ready ? %d ; current sector number %d\r\n"
													,debug_counter,DoesFlashReady(),sector_number));
		debug_counter ++;
		isJustReady = X_True;
//		Test_PriorityQueue(sector_number);
		isOK = mPriorityQueuePush(p_priority_queue,sector_number,&node_num);
		if(isOK == X_False)
		{
			SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60," push failed\r\n"));
		}
	}
//	mFlashEventHandlerRun(p_flash_handler);
	if(DoesFlashReady() == X_True)
	{
		if(isJustReady == X_True)
		{
			isJustReady = X_False;
			if(GetCurrentUsedPriorityScope(p_priority_queue,&high,&low) == X_False)
			{
				SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60,"PriorityScope get false \r\n"));
			}
			node_count = GetMyPriorityQueueUsedNodeCount(p_priority_queue);
			SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60,"node_count %d;high %d low %d \r\n",node_count,high,low));
		}
		isOK = mPriorityQueuePop(p_priority_queue,&priority,&node_num);
		SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60," pop %s , priority %d \r\n",(isOK == X_True)?"OK":"Failed",priority));
		isOK = mPriorityQueuePop(p_priority_queue,&priority,&node_num);
		SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60," pop %s , priority %d \r\n",(isOK == X_True)?"OK":"Failed",priority));
	}

}


#include "../../UserDebug.h"
SIMPLE_LOOPQUEUE_DEF(p_queue,10);
static uint32_t param_buf[10];
static X_Void FlashTestDebugInit(X_Void)
{
	SimpleQueueInitialize(p_queue);
}
static uint32_t DebugParamCollect(eSimpleQueueOperation op,uint32_t param)
{
	uint8_t node_num;
	X_Boolean isOK;
	if(op == SQO_Push)
	{
		node_num = SimpleQueueFirstIn(p_queue,&isOK,X_True);
		if(isOK == X_True)
		{
			param_buf[node_num] = param;
			return APP_SUCCESSED;
		}
		return APP_ERROR;
	}
	else if(op == SQO_Pop)
	{
		node_num = SimpleQueueFirstOut(p_queue,&isOK);
		if(isOK == X_True)
		{
			RealseSimpleQueueBuf(p_queue,node_num);
			return param_buf[node_num];
		}
		return APP_ERROR;
	}
	else if(op == SQO_Clear)
	{
		ClearSimpleQueue(p_queue);
		return APP_SUCCESSED;
	}
	return APP_ERROR;
}

/*
 *******************************************************************
	0
 *******************************************************************
 */
static  X_Void FlashInitOK(const sMyFlashEventHandler *p_handler)
{
	uint32_t base_addr,size,erase_unit,program_unit,sector_size,sector_count;
	base_addr 	= DebugParamCollect(SQO_Pop,0);
	size 		= DebugParamCollect(SQO_Pop,0);
	erase_unit  = DebugParamCollect(SQO_Pop,0);
	program_unit = DebugParamCollect(SQO_Pop,0);
	sector_size	 = DebugParamCollect(SQO_Pop,0);
	sector_count = DebugParamCollect(SQO_Pop,0);

	SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(90,"flash event handler init OK\r\n"));
	SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(90,"base_addr:%d; size:%d bytes; min erase unit:%d bytes; min program unit:%d bytes;\r\n"
										,base_addr
										,size
										,erase_unit
										,program_unit
										,sector_size
										,sector_count
										));
	SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(90," sector_size:%d; max sector_count:%d;\r\n"
										,sector_size
										,sector_count
										));
}
/*
 *******************************************************************

 *******************************************************************
 */
static const sMyFlashEventMessageDebugTable sTable[] = {
		{
			FO_Init,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			FO_UnInit,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			FO_Read,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			FO_Write,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			FO_Erase,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			FO_SectorRead,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			FO_SectorWrite,
			{FlashInitOK,X_Null,X_Null,X_Null,X_Null,},
		},
};

static X_Void onFlashDebug(eFlashDebugOperation op,uint8_t operation_ID,const sMyFlashEventHandler * p_handler)
{
	if(op >= (sizeof(sTable)/sizeof(sTable[0]))) {return;}
	if(operation_ID >= MAX_MY_FLASH_EVENT_DEBUG_ID_COUNT) {return;}
	if(sTable[op].debug_handler[operation_ID] != X_Null) {sTable[op].debug_handler[operation_ID](p_handler);}
}
