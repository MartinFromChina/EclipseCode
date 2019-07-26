#include "DataFlow.h"
#include "..\..\UserDebug.h"
#include "..\..\..\CommonSource\MulLoopQueue\mul_loop_queues.h"

#define MAX_DATA_LENGTH   30
#define BUF_NUM_DEFAULT   5

#define CHAR_KEY_LENGTH				1
#define CHAR_PENTIP_LENGTH			11
#define CHAR_AIRMOUSE_LENGTH		20
#define CHAR_COLOR_LENGTH			3
#define CHAR_PENTYPE_LENGTH			1
#define CHAR_BATTERY_LENGTH			1

static uint8_t get_buf[BUF_NUM_DEFAULT][MAX_DATA_LENGTH+1];
static uint8_t set_buf[BUF_NUM_DEFAULT][MAX_DATA_LENGTH+1];

static uint8_t key_buf[BUF_NUM_DEFAULT][CHAR_KEY_LENGTH+1];
static uint8_t pentip_buf[BUF_NUM_DEFAULT][CHAR_PENTIP_LENGTH+1];
static uint8_t airmouse_buf[BUF_NUM_DEFAULT][CHAR_AIRMOUSE_LENGTH+1];
static uint8_t color_buf[BUF_NUM_DEFAULT][CHAR_COLOR_LENGTH+1];
static uint8_t pentype_buf[BUF_NUM_DEFAULT][CHAR_PENTYPE_LENGTH+1];
static uint8_t battery_buf[BUF_NUM_DEFAULT][CHAR_BATTERY_LENGTH+1];

APP_LOOPQUEUE_DEF(p_get		,p_get_manager		,get_buf_num,BUF_NUM_DEFAULT);
APP_LOOPQUEUE_DEF(p_set		,p_set_manager		,set_buf_num,BUF_NUM_DEFAULT);

APP_LOOPQUEUE_DEF(p_key		,p_key_manager		,key_buf_length			,BUF_NUM_DEFAULT);
APP_LOOPQUEUE_DEF(p_airmouse,p_airmouse_manager	,airmouse_buf_length	,BUF_NUM_DEFAULT);
APP_LOOPQUEUE_DEF(p_pentip	,p_pentip_manager	,pentip_buf_length		,BUF_NUM_DEFAULT);
APP_LOOPQUEUE_DEF(p_color	,p_color_manager	,color_buf_length		,BUF_NUM_DEFAULT);
APP_LOOPQUEUE_DEF(p_pentype	,p_pentype_manager	,pentype_buf_length		,BUF_NUM_DEFAULT);
APP_LOOPQUEUE_DEF(p_battery	,p_battery_manager	,battery_buf_length		,BUF_NUM_DEFAULT);

X_Boolean DataFlowPush(DataFlowEntry entry,const uint8_t * p_data,uint8_t length)
{
	X_Boolean isOK;
	uint8_t buf_number;
	
	if(length > MAX_DATA_LENGTH) {return X_False; }
	
	buf_number = QueueFirstIn(p_airmouse_manager,&isOK,X_False);
	if(isOK == X_False) {return X_False;}
	
//	CopyBuffer(p_data,&airmouse_buf[buf_number][0],length);
	 airmouse_buf[buf_number][MAX_DATA_LENGTH]= length;
	return X_True;
}

X_Boolean DataFlowPop(DataFlowEntry entry,uint8_t ** p_data,uint8_t* length)
{
	X_Boolean isOK;
	uint8_t buf_number;
	
	if(DoesQueueEmpty(p_airmouse_manager) == X_True) {return X_False;}
	
	buf_number = QueueFirstOut(p_airmouse_manager,&isOK);
	if(isOK == X_False) {return X_False;}
	
	*length 	= airmouse_buf[buf_number][MAX_DATA_LENGTH];
	* p_data 	= &airmouse_buf[buf_number][0];
	return X_True;
}

X_Void DataFlowInit(X_Void)
{
	queueInitialize(p_get		,p_get_manager		,get_buf_num);
	queueInitialize(p_set		,p_set_manager		,set_buf_num);

	queueInitialize(p_key		,p_key_manager		,key_buf_length);
	queueInitialize(p_airmouse	,p_airmouse_manager	,airmouse_buf_length);
	queueInitialize(p_pentip	,p_pentip_manager	,pentip_buf_length);
	queueInitialize(p_color		,p_color_manager	,color_buf_length);
	queueInitialize(p_pentype	,p_pentype_manager	,pentype_buf_length);
	queueInitialize(p_battery	,p_battery_manager	,battery_buf_length);
}
