#include "FlashEventHandle.h"
#include "..\..\CommonSource\MulLoopQueue\m_priority_queues.h"
/*
 ******************************************************************************************
 handle flash read write event,descripe in detail:
 1, handle basic  read or write request in queue(the queue only hold the buf pointer)
 2, handle user define sector base on MCU sector system
 ******************************************************************************************
 */
APP_PRIORITY_QUEUE_DEF(p_prio_queue,10,X_True,X_Null,X_Null);

X_Void FlashEventInit(X_Void)
{
	mPriorityQueueInitialize(p_prio_queue);
}


/*******************************************test********************************************/
#include "../../[T]LogicLab/UserDebug.h"
static uint8_t buf[10];
static uint8_t test_counter = 0;
X_Void Test_PriorityQueue(uint16_t priority)
{
	uint16_t node_count,priority_min,priority_max;

	sMyPriorityNodeParam sMPNP,sMPNP_pop;
	sMPNP.is_OccupyPermit 	= X_False;
	sMPNP.p_buf 			= buf;
	sMPNP.priority			= priority;
	mPriorityQueuePush(p_prio_queue,&sMPNP);

	test_counter ++;

	if(test_counter >= 5)
	{
		test_counter = 0;

		node_count = GetMyPriorityQueueUsedNodeCount(p_prio_queue);
		SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(30,"node_count %d\r\n",node_count));

		GetCurrentUsedPriorityScope(p_prio_queue,&priority_max,&priority_min);
		SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(30,"priority scope %d ~ %d\r\n",priority_max,priority_min));

		while(mPriorityQueuePop(p_prio_queue,&sMPNP_pop) == X_True)
		{
			SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(60,"~~~~ is?%d; p_buf %2x ; priority %d\r\n"
						,sMPNP_pop.is_OccupyPermit
						,sMPNP_pop.p_buf
						,sMPNP_pop.priority));
		}
	}
}
