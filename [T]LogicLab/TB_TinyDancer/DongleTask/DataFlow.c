#include "DataFlow.h"
#include "..\..\UserDebug.h"
#include "..\..\..\CommonSource\MulLoopQueue\mul_loop_queues.h"

#define MAX_DATA_LENGTH   30
#define BUF_NUM_DEFAULT   10

static uint8_t airmouse_buf[BUF_NUM_DEFAULT][MAX_DATA_LENGTH+1];

APP_LOOPQUEUE_DEF(p_airmouse,p_airmouse_manager,airmouse_buf_length,BUF_NUM_DEFAULT);




X_Boolean AirMouseNotityDataPush(const uint8_t * p_data,uint8_t length)
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

X_Boolean AirMouseNotityDataPop(uint8_t ** p_data,uint8_t* length)
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
	queueInitialize(p_airmouse,p_airmouse_manager,airmouse_buf_length);
}
