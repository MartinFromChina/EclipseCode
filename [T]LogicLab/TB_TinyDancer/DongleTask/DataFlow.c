#include "DataFlow.h"
#include "..\..\UserDebug.h"
#include "..\..\..\CommonSource\MulLoopQueue\loop_queues.h"
#include "..\..\..\CommonSource\AppCommon.h"

#define ENTRY_NUMBER      8
#define BUF_NUM_DEFAULT   5

static X_Boolean isInit = X_False;

static uint8_t get_buf[BUF_NUM_DEFAULT][MAX_DATA_LENGTH+1];
static uint8_t set_buf[BUF_NUM_DEFAULT][MAX_DATA_LENGTH+1];

static uint8_t key_buf[BUF_NUM_DEFAULT][CHAR_KEY_LENGTH+1];
static uint8_t pentip_buf[BUF_NUM_DEFAULT][CHAR_PENTIP_LENGTH+1];
static uint8_t airmouse_buf[BUF_NUM_DEFAULT][CHAR_AIRMOUSE_LENGTH+1];
static uint8_t color_buf[BUF_NUM_DEFAULT][CHAR_COLOR_LENGTH+1];
static uint8_t pentype_buf[BUF_NUM_DEFAULT][CHAR_PENTYPE_LENGTH+1];
static uint8_t battery_buf[BUF_NUM_DEFAULT][CHAR_BATTERY_LENGTH+1];

SIMPLE_LOOPQUEUE_DEF(p_get_manager			,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_set_manager			,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_key_manager			,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_airmouse_manager		,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_pentip_manager		,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_color_manager		,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_pentype_manager		,BUF_NUM_DEFAULT);
SIMPLE_LOOPQUEUE_DEF(p_battery_manager		,BUF_NUM_DEFAULT);

static  const sListManager  *p_manager_buf[ENTRY_NUMBER];


static X_Void ManagerInit(X_Void)
{
	p_manager_buf[0] = p_get_manager;
	p_manager_buf[1] = p_set_manager;
	p_manager_buf[2] = p_key_manager;
	p_manager_buf[3] = p_airmouse_manager;
	p_manager_buf[4] = p_pentip_manager;
	p_manager_buf[5] = p_color_manager;
	p_manager_buf[6] = p_pentype_manager;
	p_manager_buf[7] = p_battery_manager;
}
static uint8_t GetBufPointerAndLength(DataFlowEntry entry,uint8_t buf_num,uint8_t **p_buf)
{
	X_Boolean isOK;
	uint8_t length;

	isOK = X_False;
	switch(entry)
	{
		case CommandGetEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = MAX_DATA_LENGTH+1;
				*p_buf = &get_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		case CommandSetEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = MAX_DATA_LENGTH+1;
				*p_buf = &set_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		case CharKeyEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = CHAR_KEY_LENGTH+1;
				*p_buf = &key_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		case CharAirMouseEntry:
//			SEGGER_RTT_Debug(DATA_FLOW_DEBUG,(40,"CharAirMouseEntry buf_num %d\r\n",buf_num));
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = CHAR_AIRMOUSE_LENGTH+1;
				*p_buf = &airmouse_buf[buf_num][0];
				isOK = X_True;
//				SEGGER_RTT_Debug(DATA_FLOW_DEBUG,(40,"length %d ; %2x\r\n",length,*p_buf));
			}
			break;
		case CharPenTipEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = CHAR_PENTIP_LENGTH+1;
				*p_buf = &pentip_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		case CharColorEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = CHAR_COLOR_LENGTH+1;
				*p_buf = &color_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		case CharPenTypeEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = CHAR_PENTYPE_LENGTH+1;
				*p_buf = &pentype_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		case CharBatteryEntry:
			if((buf_num+1) <= BUF_NUM_DEFAULT)
			{
				length = CHAR_BATTERY_LENGTH+1;
				*p_buf = &battery_buf[buf_num][0];
				isOK = X_True;
			}
			break;
		default:
			break;
	}
	if(isOK == X_False)
	{
		*p_buf = X_Null;
		length = 0;
	}
	return length;
}

static uint8_t *buf_pointer;
X_Boolean DataFlowPush(DataFlowEntry entry,const uint8_t * p_data,uint8_t length)
{
	X_Boolean isOK,isDataCanBeLose;
	uint8_t buf_number,buf_length;

	if(isInit == X_False) {return X_False;}
	if(entry > (sizeof(p_manager_buf)/sizeof(p_manager_buf[0]))) {return X_False;}

//	if(entry == CharKeyEntry || entry == CharAirMouseEntry || entry == CharPenTipEntry){isDataCanBeLose = X_True;}
//	else{isDataCanBeLose = X_False;}
	isDataCanBeLose = X_False;

	buf_number = SimpleQueueFirstIn(p_manager_buf[entry],&isOK,isDataCanBeLose);
	if(isOK == X_False) {return X_False;}

	buf_length = GetBufPointerAndLength(entry,buf_number,&buf_pointer);
	if(buf_length == 0 || buf_pointer == X_Null) {return X_False;}
	if(length > buf_length) {return X_False;}

	CopyBuffer(p_data,buf_pointer,length);
	buf_pointer[buf_length]= length;
	return X_True;
}

X_Boolean DataFlowPop(DataFlowEntry entry,uint8_t ** p_data,uint8_t* length)
{
	X_Boolean isOK;
	uint8_t buf_number,buf_length;

	if(isInit == X_False) {return X_False;}
	if(entry > (sizeof(p_manager_buf)/sizeof(p_manager_buf[0]))) {return X_False;}
	if(DoesSimpleQueueEmpty(p_manager_buf[entry]) == X_True) {return X_False;}
	
	buf_number = SimpleQueueFirstOut(p_manager_buf[entry],&isOK);
	if(isOK == X_False) {return X_False;}

	buf_length = GetBufPointerAndLength(entry,buf_number,&buf_pointer);
	if(buf_length == 0 || buf_pointer == X_Null) {return X_False;}
	*length 	= buf_pointer[buf_length-1];
	*p_data 	= buf_pointer;

	//	SEGGER_RTT_Debug(DATA_FLOW_DEBUG,(43,"DataFlowPop %d buf_number %d %2x\r\n",entry,buf_number,buf_pointer));
	return X_True;
}

X_Void DataFlowClear(DataFlowEntry entry)
{
	if(entry > (sizeof(p_manager_buf)/sizeof(p_manager_buf[0]))) {return;}
	ClearSimpleQueue(p_manager_buf[entry]);
}

X_Void DataFlowInit(X_Void)
{
	isInit = X_False;
	SimpleQueueInitialize(p_get_manager		);
	SimpleQueueInitialize(p_set_manager		);
	SimpleQueueInitialize(p_key_manager		);
	SimpleQueueInitialize(p_airmouse_manager);
	SimpleQueueInitialize(p_pentip_manager	);
	SimpleQueueInitialize(p_color_manager	);
	SimpleQueueInitialize(p_pentype_manager	);
	SimpleQueueInitialize(p_battery_manager	);

	ManagerInit();
	isInit = X_True;
}
