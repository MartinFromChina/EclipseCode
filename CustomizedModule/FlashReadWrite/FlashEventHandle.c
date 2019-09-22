#include "FlashEventHandle.h"
#include "..\..\CommonSource\MulLoopQueue\m_priority_queues.h"
/*
 ******************************************************************************************
 handle flash read write event,descripe in detail:
 1, handle basic  read or write request in queue(the queue only hold the buf pointer)
 2, handle user define sector base on MCU sector system
 ******************************************************************************************
 */
APP_PRIORITY_QUEUE_DEF(p_prio_queue,10,X_True);

X_Void FlashEventInit(X_Void)
{
	mPriorityQueueInitialize(p_prio_queue);
}
