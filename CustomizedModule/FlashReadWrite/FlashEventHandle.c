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
	return APP_SUCCESSED;
}
m_app_result mFlashEventUnInit(const sMyFlashEventHandler *p_handler)
{
	return APP_SUCCESSED;
}
m_app_result mFlashReadRequest(const sMyFlashEventHandler *p_handler,uint32_t read_start_addr,uint32_t read_length,void const * p_dest,onReadFinished read_cb)
{
	return APP_SUCCESSED;
}
m_app_result mFlashWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t write_start_addr,uint32_t write_length,void const * p_src,onWriteFinished write_cb)
{
	return APP_SUCCESSED;
}
m_app_result mFlashEraseRequest(const sMyFlashEventHandler *p_handler,uint32_t Erase_start_addr,uint32_t erase_length,void const * p_src,onEraseFinished erase_cb)
{
	return APP_SUCCESSED;
}

m_app_result mFlashSectorReadRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,void const * p_dest,onReadFinished read_cb)
{
	return APP_SUCCESSED;
}
m_app_result mFlashSectorWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,void const * p_src,onWriteFinished write_cb)
{
	return APP_SUCCESSED;
}

m_app_result mFlashEventHandlerHold(const sMyFlashEventHandler *p_handler)
{
	return APP_SUCCESSED;
}
m_app_result mFlashEventHandlerBeginProcessing(const sMyFlashEventHandler *p_handler)
{
	return APP_SUCCESSED;
}

X_Void mFlashEventHandlerRun(const sMyFlashEventHandler *p_handler)
{

}
