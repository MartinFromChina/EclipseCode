#include "FlashEventHandle.h"
#include "..\..\CommonSource\MulLoopQueue\m_priority_queues.h"

/*
 ******************************************************************************************
 handle flash read write event,descripe in detail:
 1, handle basic  read or write request in queue(the queue only hold the buf pointer)
 2, handle user define sector base on MCU sector system
 ******************************************************************************************
 */
static X_Void onListNodeDebug(eListOperation e_lop,uint8_t operation_ID,const sMySingleLinkList * s_sll);
static X_Void onQueueDebug(ePriorityQueueOperation e_ppo,uint8_t operation_ID,const sMyPriorityListManager *p_lm);

APP_PRIORITY_QUEUE_DEF(p_prio_queue,20,X_True,onListNodeDebug,onQueueDebug);
X_Void FlashEventInit(X_Void)
{
	mPriorityQueueInitialize(p_prio_queue);
}


/*******************************************test********************************************/
#include "../../[T]LogicLab/UserDebug.h"

/*
	LO_TailAdd,
	LO_TailRemove,
	LO_HeadAdd,
	LO_HeadRemove,
	LO_Insert,
	LO_PullAway,
	LO_SizeGet,
	LO_DoesEmpty,
	LO_ListClear,
	LO_FindByValue,
	LO_InformationGet,
	LO_InformationUpdata,
*/
static  X_Void ListInitDebug(const sMySingleLinkList * s_sll)
{
	SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(60,"list node init OK valid node num: %d\r\n",s_sll ->ValidNodeNumber));
}
static const sListNodeMessageDebugTable sLNMDT[] = {
		{
			LO_init,
			{ListInitDebug,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			LO_TailAdd,
			{X_Null,X_Null,X_Null,X_Null,X_Null,},
		},
};
static X_Void onListNodeDebug(eListOperation e_lop,uint8_t operation_ID,const sMySingleLinkList * s_sll)
{
	if(e_lop >= (sizeof(sLNMDT)/sizeof(sLNMDT[0]))) {return;}
	if(operation_ID >= MAX_LISTNODE_DEBUG_ID_COUNT) {return;}
	if(sLNMDT[e_lop].debug_handler[operation_ID] != X_Null){sLNMDT[e_lop].debug_handler[operation_ID](s_sll);}
}

/*
	PQO_init = 0,
	PQO_Push,
	PQO_Pop,
	PQO_Clear,
	PQO_ReleaseNode,
	PQO_NodeCount,
	PQO_PriorityScope,
	PQO_DoesEmpty,
 */
static  X_Void QueueInitDebug(const sMyPriorityListManager *p_lm)
{
	SEGGER_RTT_Debug(FLASH_TEST_DEBUG,(60,"queue init OK max node num: %d\r\n",p_lm ->MaxNodeNumber));
}
static const sPriorityQueueMessageDebugTable sPQMDT[] = {
		{
			PQO_init,
			{QueueInitDebug,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			PQO_Push,
			{X_Null,X_Null,X_Null,X_Null,X_Null,},
		},
};

static X_Void onQueueDebug(ePriorityQueueOperation e_ppo,uint8_t operation_ID,const sMyPriorityListManager *p_lm)
{
	if(e_ppo >= (sizeof(sPQMDT)/sizeof(sPQMDT[0]))) {return;}
	if(operation_ID >= MAX_LISTNODE_DEBUG_ID_COUNT) {return;}
	if(sPQMDT[e_ppo].debug_handler[operation_ID] != X_Null) {sPQMDT[e_ppo].debug_handler[operation_ID](p_lm);}
}


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
		SEGGER_RTT_Debug(FLASH_DEBUG && FLASH_TEST_DEBUG,(30,"node_count %d\r\n",node_count));

		GetCurrentUsedPriorityScope(p_prio_queue,&priority_max,&priority_min);
		SEGGER_RTT_Debug(FLASH_DEBUG && FLASH_TEST_DEBUG,(30,"priority scope %d ~ %d\r\n",priority_max,priority_min));

		while(mPriorityQueuePop(p_prio_queue,&sMPNP_pop) == X_True)
		{
			SEGGER_RTT_Debug(FLASH_DEBUG && FLASH_TEST_DEBUG,(60,"~~~~ is?%d; p_buf %2x ; priority %d\r\n"
						,sMPNP_pop.is_OccupyPermit
						,sMPNP_pop.p_buf
						,sMPNP_pop.priority));
		}
	}
}
