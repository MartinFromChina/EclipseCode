#include "FlashEventHandle.h"
/*
 ******************************************************************************************
 handle flash read write event,descripe in detail:
 1, handle basic  read or write request in queue(the queue only hold the buf pointer)
 2, handle user define sector base on MCU sector system
 ******************************************************************************************
 */
m_app_result mFlashEventInit(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	mPriorityQueueInitialize(p_handler ->p_priority_queue);
	p_handler ->p_manager ->isInitOK = X_True;

	#if (USE_MY_FLASH_EVENT_HANDLER_DEBUG == 1)
	if(p_handler ->onDebug != X_Null)
	{
		if(p_handler ->onDebugParamCollect != X_Null)
		{
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->base_addr);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->total_size_in_bytes);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->erase_uint);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->program_unit);

			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_user_param ->sector_size);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_user_param ->total_sector_count);
		}
		p_handler ->onDebug(FO_Init,0,p_handler);
		if(p_handler ->onDebugParamCollect != X_Null){p_handler ->onDebugParamCollect(SQO_Clear,0);}
	}
	#endif
	return APP_SUCCESSED;
}
m_app_result mFlashEventUnInit(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
m_app_result mFlashReadRequest(const sMyFlashEventHandler *p_handler,uint32_t read_start_addr,uint32_t read_length,FLASH_POINTER_TYPE * p_dest,onReadFinished read_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
m_app_result mFlashWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t write_start_addr,uint32_t write_length,FLASH_POINTER_TYPE const * p_src,onWriteFinished write_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
m_app_result mFlashEraseRequest(const sMyFlashEventHandler *p_handler,uint32_t erase_start_addr,uint32_t erase_length,onEraseFinished erase_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}

#if (M_FLASH_MULTI_PARTITION == 1)
m_app_result mFlashSectorReadRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,FLASH_POINTER_TYPE  * p_dest,onReadFinished read_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
m_app_result mFlashSectorWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,FLASH_POINTER_TYPE const * p_src,onWriteFinished write_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}

m_app_result mFlashEventHandlerHold(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
m_app_result mFlashEventHandlerBeginProcessing(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
#endif

X_Void mFlashEventHandlerRun(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return;}
}
