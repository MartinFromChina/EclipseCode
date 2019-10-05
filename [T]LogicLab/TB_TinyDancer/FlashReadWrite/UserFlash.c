#include "UserFlash.h"
#include "UserFlashTB.h"
#include "..\..\..\CustomizedModule\FlashReadWrite\FlashEventHandle.h"

static X_Void FlashTestDebugInit(X_Void);
static X_Void onFlashDebug(eFlashDebugOperation op,uint8_t operation_ID,const sMyFlashEventHandler * p_handler);
static uint32_t DebugParamCollect(eSimpleQueueOperation op,uint32_t param);

APP_FLASH_EVENT_HANDLER_DEFINE(p_flash_handler
								,0,0
								,0,0
								,0,0
								,X_Null,X_Null,X_Null,X_Null,X_Null,X_Null
								,onFlashDebug,DebugParamCollect
								);

FILE * FlashTestOpenFile(void)
{
	mFlashEventInit(p_flash_handler);
	FlashTestDebugInit();
	return fopen(".//TB_TinyDancer//FlashReadWrite//command.txt", "r");
}
static uint16_t debug_counter = 0;
X_Void onTick(X_Void)
{
	uint8_t sector_number;
	if(GetSectorNumber(&sector_number) == X_True)
	{
		SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60,"**%d**flash ready ? %d ; current sector number %d\r\n"
													,debug_counter,DoesFlashReady(),sector_number));
		debug_counter ++;
	}
	mFlashEventHandlerRun(p_flash_handler);
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
