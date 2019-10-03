#include "m_priority_queues.h"
/*
 ***********************************************************************************************
 *1,push node into queue with priority
 *if same priority is pushed ,the previous one will be replace by the new one
 *2,pop node by priority (from small to large or from large to small could be config)
 *3,user can check the queue state include : (is empty ? min priority ,max priority , cunrrent used node number and so on)
 ***********************************************************************************************
 */
#ifdef USE_PRIORITY_QUEUE_BASED_ON_LIST_NODE
typedef enum
{
	QP_Replace,
	QP_Insert,
}ePriorityQueuePushAction;

static X_Boolean GetActionAndNodeNumber(const sMySingleLinkList * s_sll,X_Boolean isFromSmall,uint16_t priority,uint16_t current_node_count
									,ePriorityQueuePushAction *p_action,uint16_t *p_number)
{
	uint16_t i;
	sListNodeSpace * p_node;
	if(s_sll == X_Null ||p_action == X_Null || p_number == X_Null){return X_False;}
	if(s_sll ->p_manager ->isInitOK == X_False){return X_False;}
	if(s_sll ->p_manager ->current_head_node_number >= s_sll ->ValidNodeNumber ){return X_False;}
	if(current_node_count > s_sll ->ValidNodeNumber){return X_False;}
	if(current_node_count == 0)
	{
		*p_action =	QP_Insert;
		*p_number = 0;
		return X_True;
	}

	p_node = &(s_sll->p_param ->p_node_space[s_sll ->p_manager ->current_head_node_number]);
	if(isFromSmall == X_True)
	{
		for(i=0;i<current_node_count;i++)
		{
			if(priority == p_node ->extern_information)
			{
				*p_action = QP_Replace;
				*p_number = i;
				return X_True;
			}
			else if(priority < p_node ->extern_information )
			{
				*p_action = QP_Insert;
				*p_number = i;
				return X_True;
			}

			if(p_node ->NextPtr != X_Null){p_node = p_node ->NextPtr;}
			else
			{
				if((i + 1) == current_node_count)
				{
					i = current_node_count;
					break;
				}
				else {return X_False;}
			}
		}
		*p_action = QP_Insert;
		*p_number = i;
		return X_True;
	}
	else
	{
		for(i=0;i<current_node_count;i++)
		{

			if(priority == p_node ->extern_information)
			{
				*p_action = QP_Replace;
				*p_number = i;
				return X_True;
			}
			else if(priority > p_node ->extern_information )
			{
				*p_action = QP_Insert;
				*p_number = i;
				return X_True;
			}

			if(p_node ->NextPtr != X_Null){p_node = p_node ->NextPtr;}
			else
			{
				if((i + 1) == current_node_count)
				{
					i = current_node_count;
					break;
				}
				else {return X_False;}
			}
		}
		*p_action = QP_Insert;
		*p_number = i;
		return X_True;
	}
}
X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager)
{
	uint16_t i,priority_default;
	if(p_manager == X_Null) {return;}
	mSingleListInit(p_manager ->p_list);
	if(p_manager ->MaxNodeNumber >= 500) {return;}

	priority_default = (p_manager ->isHighPriorityFromSmall == X_True) ? MY_MAX_PRIORITY_VALUE : 0;

	for(i=0;i<p_manager ->MaxNodeNumber;i++)
	{
		p_manager ->p_param[i].is_OccupyPermit = X_True;
		p_manager ->p_param[i].priority		   = priority_default;
		p_manager ->p_param[i].p_buf           = X_Null;
	}
	*p_manager ->isInit = X_True;
	#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
	if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_init,0,p_manager);}
	#endif
}
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam const *p_data)
{
	uint8_t error_code;
	uint16_t node_count,node_number;
	ePriorityQueuePushAction e_pqpa;
	if(p_manager == X_Null || p_data == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	if(p_data ->p_buf == X_Null)	 {return X_False;}
	if(p_data ->priority >= p_manager ->MaxNodeNumber) {return X_False;}
	if( mSingleListSizeGet(p_manager->p_list,&node_count) == X_False){return X_False;}

	p_manager ->p_param[p_data ->priority].is_OccupyPermit = p_data ->is_OccupyPermit;
	p_manager ->p_param[p_data ->priority].priority = p_data ->priority;
	p_manager ->p_param[p_data ->priority].p_buf = p_data ->p_buf;
	if(node_count == 0)// insert first element
	{
		#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
		if(p_manager ->onDebug != X_Null)
		{
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,0);}
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,p_data ->priority);}
			p_manager ->onDebug(PQO_Push,0,p_manager);
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
		}
		#endif
		if(mSingleListHeadAdd(p_manager->p_list,p_data ->priority) == APP_SUCCESSED){return X_True;}
		return X_False;
	}
	else if(node_count >= p_manager ->MaxNodeNumber) // only replace can be done
	{
		if(GetActionAndNodeNumber(p_manager->p_list,p_manager ->isHighPriorityFromSmall,p_data ->priority,node_count
									,&e_pqpa,&node_number) == X_True)
		{
			#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
			if(p_manager ->onDebug != X_Null)
			{
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,e_pqpa);}
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,node_number);}
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,p_data ->priority);}
				p_manager ->onDebug(PQO_Push,1,p_manager);
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
			}
			#endif
			if(e_pqpa == QP_Insert) {return X_False;}
			return X_True;
				// the updata value == previous value in current situation, no need updata 
//			if(mSingleListUpdataValueByNodeNumber(p_manager->p_list,node_number,p_data ->priority) == X_True) {return X_True;}
//			return X_False;
		}
		return X_False;
	}
	else
	{
		if(GetActionAndNodeNumber(p_manager->p_list,p_manager ->isHighPriorityFromSmall,p_data ->priority,node_count
									,&e_pqpa,&node_number) == X_True)
		{
			#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
			if(p_manager ->onDebug != X_Null)
			{
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,e_pqpa);}
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,node_number);}
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,p_data ->priority);}
				p_manager ->onDebug(PQO_Push,2,p_manager);
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
			}
			#endif
			if(e_pqpa == QP_Insert)
			{
				error_code = mSingleListInsert(p_manager->p_list,node_number,p_data ->priority);
				if( error_code == APP_SUCCESSED){return X_True;}
				#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
				if(p_manager ->onDebug != X_Null)
				{
					if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,error_code);}
					p_manager ->onDebug(PQO_Push,3,p_manager);
					if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
				}
				#endif
				return X_False;
			}
			return X_True;
			// the updata value == previous value in current situation, no need updata 
//			if(mSingleListUpdataValueByNodeNumber(p_manager->p_list,node_number,p_data ->priority) == X_True) {return X_True;}
//			#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
//			if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_Push,4,p_manager);}
//			#endif
//			return X_False;
		}
		return X_False;
	}
}
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam *p_data)
{
	uint16_t infor_number;
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	if(DoesMySingleListEmpty(p_manager ->p_list) == X_True)  {return X_False;}

	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,0,&infor_number) == X_True)
	{
		if(infor_number >= p_manager ->MaxNodeNumber) {return X_False;}
		mSingleListHeadRemove(p_manager ->p_list);
		p_data ->is_OccupyPermit = p_manager ->p_param[infor_number].is_OccupyPermit;
		p_data ->priority		 = p_manager ->p_param[infor_number].priority;
		p_data ->p_buf			 = p_manager ->p_param[infor_number].p_buf;
		#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
		if(p_manager ->onDebug != X_Null)
		{
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,infor_number);}
			p_manager ->onDebug(PQO_Pop,0,p_manager);
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
		}
		#endif
		return X_True;
	}
	#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
	if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_Pop,1,p_manager);}
	#endif
	return X_False;
}
X_Void    ClearMyPriorityQueue(const sMyPriorityListManager *p_manager)
{
	mPriorityQueueInitialize(p_manager);
}
m_app_result    RealseMyPriorityQueueNode(const sMyPriorityListManager *p_manager,uint16_t node_priority)
{
	uint16_t node_number;
	if(p_manager == X_Null) {return APP_POINTER_NULL;}
	if(*p_manager ->isInit == X_False) {return APP_ERROR;}

	if(mSingleListFindByValue(p_manager ->p_list,node_priority,&node_number) == X_True)
	{
		return mSingleListPullAway(p_manager ->p_list,node_number);
	}
	return APP_ERROR;
}
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager)
{
	uint16_t node_count;
	if(p_manager == X_Null) {return 0;}
	if(*p_manager ->isInit == X_False) {return 0;}
	if( mSingleListSizeGet(p_manager->p_list,&node_count) == X_True){return node_count;}
	return 0;
}
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_high,uint16_t *p_low)
{
	uint16_t node_count;
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}

	if(DoesMySingleListEmpty(p_manager ->p_list) == X_True){return X_False;}

	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,0,p_high) == X_False) {return X_False;}
	if(mSingleListSizeGet(p_manager->p_list,&node_count) == X_False) {return X_False;}
	if(node_count == 0){return X_False;}
	if(node_count == 1){p_low = p_high;return X_True;}
	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,node_count - 1,p_low) == X_False) {return X_False;}
	return X_True;
}
uint16_t   GetPriorityByNodeNumber(const sMyPriorityListManager *p_manager,uint16_t node_number)
{
	uint16_t priority;
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,node_number,&priority) == X_True)
	{
		return priority;
	}
	return 0xffff;
}
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	return DoesMySingleListEmpty(p_manager ->p_list);
}
#endif

#ifdef USE_PRIORITY_QUEUE_BASED_ON_PRIORITY_TABLE
static X_Boolean PriorityTableInit(uint32_t *p_table,uint16_t max_priority)
{
	uint16_t i;
	if(max_priority == 0 || p_table == X_Null) {return X_False;}
	uint16_t size = GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority);

	for(i=0;i<size;i++)
	{
		p_table[i] = 0;
	}
	return X_True;
}
static X_Boolean PriorityTableInsert(uint32_t *p_table,X_Boolean isHighPriorityFromSmall,uint16_t max_priority_value,uint16_t priority)
{
	uint16_t priority_convert;
	if(p_table == X_Null) {return X_False;}
	if(priority > max_priority_value) {return X_False;}

	if(isHighPriorityFromSmall == X_True) {priority_convert = priority;}
	else {priority_convert = max_priority_value - priority;}

	TYPE_DEF_PRIORITY_TABLE_VALUE bit,bit_number;
	uint16_t index;

	index = priority_convert/BIT_COUNT_IN_UINT32;
	bit_number = (TYPE_DEF_PRIORITY_TABLE_VALUE)priority_convert & (BIT_COUNT_IN_UINT32 - 1u);
	bit = 1u;
	bit <<= (BIT_COUNT_IN_UINT32 - 1u) - bit_number;
	p_table[index] |= bit;
	return X_True;
}
static X_Boolean PriorityTableRemove(uint32_t *p_table,X_Boolean isHighPriorityFromSmall,uint16_t max_priority_value,uint16_t priority)
{
	uint16_t priority_convert;
	if(p_table == X_Null) {return X_False;}
	if(priority > max_priority_value) {return X_False;}

	if(isHighPriorityFromSmall == X_True) {priority_convert = priority;}
	else {priority_convert = max_priority_value - priority;}

	TYPE_DEF_PRIORITY_TABLE_VALUE bit,bit_number;
	uint16_t index;

	index = priority_convert/BIT_COUNT_IN_UINT32;
	bit_number = (TYPE_DEF_PRIORITY_TABLE_VALUE)priority_convert & (BIT_COUNT_IN_UINT32 - 1u);
	bit = 1u;
	bit <<= (BIT_COUNT_IN_UINT32 - 1u) - bit_number;
	p_table[index] &= ~bit;
	return X_True;


}
static X_Boolean PriorityTableGetHighest(uint32_t *p_table,X_Boolean isHighPriorityFromSmall,uint16_t max_priority_value,uint16_t *p_priority)
{
	uint16_t i,prio,priority_convert;
	X_Boolean isAllZero;
	if(p_table == X_Null || p_priority == X_Null) {return X_False;}

	uint16_t size = GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority_value);
	prio = 0;
	isAllZero = X_True;
	for(i=0;i<size;i++)
	{
		if(p_table[i] != 0) {isAllZero = X_False;break;}
		else {prio += BIT_COUNT_IN_UINT32;}
	}

	if( isAllZero == X_True )  {return X_False;}
	prio +=(uint16_t)GetLeadZeroCount(p_table[i]);
	if(prio > max_priority_value) {return X_False;}

	if(isHighPriorityFromSmall == X_True) {priority_convert = prio;}
	else{priority_convert = max_priority_value - prio;}
	*p_priority = priority_convert;

	return X_True;
}
static X_Boolean PriorityTableGetLowest(uint32_t *p_table,X_Boolean isHighPriorityFromSmall,uint16_t max_priority_value,uint16_t *p_priority)
{
	uint16_t i,prio,priority_convert;
	uint8_t  rear_zero_count;
	X_Boolean isAllZero;
	if(p_table == X_Null || p_priority == X_Null) {return X_False;}

	uint16_t size = GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority_value);
	prio = (size * BIT_COUNT_IN_UINT32) - 1;
	isAllZero = X_True;
	for(i=size;i>0;i--)
	{
		if(p_table[i-1] != 0) {isAllZero = X_False;break;}
		else {prio -= BIT_COUNT_IN_UINT32;}
	}
	if( isAllZero == X_True )  {return X_False;}
	rear_zero_count = GetRearZeroCount(p_table[i-1]);
	if(prio >= BIT_COUNT_IN_UINT32) {prio = prio - BIT_COUNT_IN_UINT32;}
//	printf("!!!!!prio %d ,source %2x ; rear_zero_count %d \r\n",prio,p_table[i-1],rear_zero_count);
	prio +=(uint16_t)(BIT_COUNT_IN_UINT32 - rear_zero_count);
	if(prio > max_priority_value) {return X_False;}

	if(isHighPriorityFromSmall == X_True) {priority_convert = prio;}
	else{priority_convert = max_priority_value - prio;}
	*p_priority = priority_convert;

	return X_True;
}
static X_Boolean PrioroityQueueBorrowNode(sPriorityQueueListNodeSpace const*p_space,sListNodeWithPriority **p_node)
{
	uint16_t i;
	X_Boolean isOK;
	if(p_space == X_Null || p_node == X_Null) {return X_False;}// necessary?

	isOK = X_False;
	for(i=0;i< p_space ->max_node_count;i++)
	{
		if(p_space ->p_space[i].isFree == X_True)
		{
			p_space ->p_space[i].isFree = X_False;
			*p_node = &(p_space ->p_space[i]);
			isOK = X_True;
			break;
		}
	}
	return isOK;
}

static X_Boolean PrioroityQueueReturnNode(sPriorityQueueListNodeSpace const*p_space,uint16_t node_ID)
{
	if(p_space == X_Null ) {return X_False;}// necessary?
	if(node_ID >= p_space ->max_node_count)  {return X_False;}

	p_space ->p_space[node_ID].isFree = X_True;
	p_space ->p_space[node_ID].NextPtr = X_Null;

	return X_True;
}

static m_app_result NodeTailAdd(sPriorityQueueListNodeSpace const*p_space,uint16_t priority,uint16_t *p_node_ID)
{
//	sListNodeSpace * p_current_node;
//	sListNodeSpace * p_next_node;
	sListNodeWithPriority *p_node_head;
//	if(BorrowNode(s_sll ->p_param,&p_next_node) == X_False) {return APP_NO_ENOUGH_SPACE;}
//	if(p_head == X_Null) {return APP_POINTER_NULL;}
//
//
	p_node_head = p_space->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE];
//
//	p_next_node ->extern_information = infor_number;
//	p_next_node ->NextPtr			   = X_Null;
//
//	if(s_sll ->p_manager ->used_node_num == 0)
//	{
//		s_sll ->p_manager ->current_head_node_number = p_next_node ->node_ID_in_list_array;
//		p_current_node = p_next_node;//just for debug
//	}
//	else
//	{
//		if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
//									,(s_sll ->p_manager ->used_node_num - 1),&p_current_node) == X_False)
//		{
//			ReturnNode(s_sll ->p_param,p_next_node ->node_ID_in_list_array);
//			return APP_ERROR;
//		}
//
//		p_current_node ->NextPtr = p_next_node;
//	}
//	#if (USB_MY_LIST_NODE_DEBUG == 1)
//	if(s_sll ->onDebug != X_Null)
//	{
//		if(s_sll ->onDebugParamCollect != X_Null){s_sll ->onDebugParamCollect(SQO_Push,p_current_node ->node_ID_in_list_array);}
//		if(s_sll ->onDebugParamCollect != X_Null){s_sll ->onDebugParamCollect(SQO_Push,p_next_node ->node_ID_in_list_array);}
//		s_sll ->onDebug(LO_TailAdd,0,s_sll);
//		if(s_sll ->onDebugParamCollect != X_Null){s_sll ->onDebugParamCollect(SQO_Clear,0);}
//	}
//	#endif
//	s_sll ->p_manager ->used_node_num ++;
	(*p_space ->p_current_used_node_count) ++;
	return APP_SUCCESSED;
}

static m_app_result NodePullAway(sPriorityQueueListNodeSpace const*p_space,uint16_t priority,uint16_t *p_node_ID)
{
//	sListNodeSpace * p_node_going_to_drop;
//	sListNodeSpace * p_left_node;
//	sListNodeSpace * p_right_node;
	sListNodeWithPriority *p_node_head;
//	if(s_sll == X_Null) {return APP_POINTER_NULL;}
//	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}

	p_node_head = p_space ->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE];
//
//	if(node_order_number >= s_sll ->p_manager ->used_node_num) {return APP_PARAM_ERROR;}
	if((*p_space ->p_current_used_node_count) == 0){return APP_ALREADY_DONE_BEFORE;}
//
//	if(s_sll ->p_manager ->used_node_num ==  (node_order_number + 1)) //
//	{
//		#if (USB_MY_LIST_NODE_DEBUG == 1)
//		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,0,s_sll);}
//		#endif
//		return mSingleListTailRemove(s_sll);
//	}
//
//	if(node_order_number == 0)
//	{
//		#if (USB_MY_LIST_NODE_DEBUG == 1)
//		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,1,s_sll);}
//		#endif
//		return mSingleListHeadRemove(s_sll);
//	}
//
//	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
//												,(node_order_number),&p_node_going_to_drop) == X_False){return APP_ERROR;}
//	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
//													,(node_order_number - 1),&p_left_node) == X_False) {return APP_ERROR;}
//	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
//														,(node_order_number + 1),&p_right_node) == X_False) {return APP_ERROR;}
//
//	p_left_node->NextPtr = p_right_node;
//	s_sll ->p_manager ->used_node_num --;
	(*p_space ->p_current_used_node_count) --;
//	if(ReturnNode(s_sll ->p_param,p_node_going_to_drop ->node_ID_in_list_array) == X_True)
//	{
//		#if (USB_MY_LIST_NODE_DEBUG == 1)
//		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,2,s_sll);}
//		#endif
		return APP_SUCCESSED;
//	}
//	#if (USB_MY_LIST_NODE_DEBUG == 1)
//	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,3,s_sll);}
//	#endif
//	return APP_ERROR;
//
}


X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager)
{
	uint16_t i,priority_default;
	if(p_manager == X_Null) {return;}
	if(p_manager ->MaxNodeCount >= MY_PRIORITY_QUEUE_MAX_NODE_NUMBER) {return;}

	priority_default = (p_manager ->isHighPriorityFromSmall == X_True) ? p_manager ->MaxPriorityValue : 0;
	for(i=0;i<p_manager ->MaxNodeCount;i++)
	{
		p_manager ->p_node_space_handle->p_space[i].node_ID_in_list_array = i;
		p_manager ->p_node_space_handle->p_space[i].priority = priority_default;
		p_manager ->p_node_space_handle->p_space[i].isFree   = X_True;
		p_manager ->p_node_space_handle->p_space[i].NextPtr  = X_Null;
	}
	for(i=0;i<LIST_NODE_TABLE_SIZE;i++)
	{
		p_manager ->p_node_space_handle->p_list_head_nodes[i] = X_Null;
	}
	(*p_manager ->p_node_space_handle->p_current_used_node_count) = 0;

	if(PriorityTableInit(p_manager ->p_priority_table,p_manager ->MaxPriorityValue) == X_True)
	{
		(*p_manager ->isInit) = X_True;
	#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
		if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_init,0,p_manager);}
	#endif
	}
	#if (USE_MY_PRIORITY_QUEUE_DEBUG == 1)
		if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_init,1,p_manager);}
	#endif
}

X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,uint16_t priority,uint16_t *p_node_number)
{
	m_app_result result;

	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False){return X_False;}
	if(*p_manager ->p_node_space_handle ->p_current_used_node_count >= p_manager ->MaxNodeCount){return X_False;}

	if (PriorityTableInsert(p_manager ->p_priority_table,p_manager ->isHighPriorityFromSmall
				,p_manager ->MaxPriorityValue,priority) == X_False) {return X_False;}

	result = NodeTailAdd(p_manager ->p_node_space_handle,priority,p_node_number);

	return (result == APP_SUCCESSED);
}
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,uint16_t *p_priority,uint16_t *p_node_number)
{
	if(p_manager == X_Null || p_priority == X_Null || p_node_number == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False){return X_False;}

	if(PriorityTableGetHighest(p_manager ->p_priority_table,p_manager ->isHighPriorityFromSmall
				,p_manager ->MaxPriorityValue,p_priority) == X_False) {return X_False;}

	return RealseMyPriorityQueueNodeByPriority(p_manager,*p_priority,p_node_number);
}
X_Void      ClearMyPriorityQueue(const sMyPriorityListManager *p_manager)
{
	mPriorityQueueInitialize(p_manager);
}
X_Boolean   RealseMyPriorityQueueNodeByPriority(const sMyPriorityListManager *p_manager,uint16_t priority,uint16_t *p_node_released)
{
	m_app_result result;

	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False){return X_False;}
	if(*p_manager ->p_node_space_handle ->p_current_used_node_count == 0){return X_False;}

	if(PriorityTableRemove(p_manager ->p_priority_table,p_manager ->isHighPriorityFromSmall
					,p_manager ->MaxPriorityValue,priority) == X_False) {return X_False;}

	result = NodePullAway(p_manager ->p_node_space_handle,priority,p_node_released);

	return (result == APP_SUCCESSED);
}
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager)
{
	if(p_manager == X_Null) {return 0;}
	return (*p_manager ->p_node_space_handle->p_current_used_node_count);
}
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_high,uint16_t *p_low)
{
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}

	if(PriorityTableGetHighest(p_manager ->p_priority_table,p_manager ->isHighPriorityFromSmall
					,p_manager ->MaxPriorityValue,p_high) == X_False) {return X_False;}

	if(PriorityTableGetLowest(p_manager ->p_priority_table,p_manager ->isHighPriorityFromSmall
					,p_manager ->MaxPriorityValue,p_low) == X_False) {return X_False;}
	return X_True;
}
X_Boolean   GetPriorityByNodeNumber(const sMyPriorityListManager *p_manager,uint16_t node_number,uint16_t *p_priority)
{
	if(p_manager == X_Null || p_priority == X_Null) {return X_False;}
	if(node_number >= p_manager ->MaxNodeCount) {return X_False;}
	if(p_manager ->p_node_space_handle->p_space[node_number].isFree == X_False) {return X_False;}

	*p_priority = p_manager ->p_node_space_handle->p_space[node_number].priority;
	return X_True;

}
X_Boolean   GetNodeNumberByPriority(const sMyPriorityListManager *p_manager,uint16_t priority,uint16_t *p_node_number)
{
	X_Boolean isOK;
	uint16_t i,check_scope;
	sListNodeWithPriority *p_head;
	if(p_manager == X_Null || p_node_number == X_Null) {return X_False;}
	if(priority > p_manager ->MaxPriorityValue) {return X_False;}

	p_head = p_manager ->p_node_space_handle->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE];
	if(p_head == X_Null) {return X_False;}
	check_scope = (uint16_t)(p_manager ->MaxPriorityValue / LIST_NODE_TABLE_SIZE) + 1;
	check_scope = (check_scope > p_manager ->MaxNodeCount) ? p_manager ->MaxNodeCount : check_scope;

	isOK = X_False;
	for(i=0;i<check_scope;i++)// use while loop is more simple ? more risky ?
	{
		if(p_head ->priority == priority)
		{
			*p_node_number = p_head ->node_ID_in_list_array;
			isOK = X_True;
			break;
		}
		if(p_head ->NextPtr != X_Null) {p_head = p_head ->NextPtr;}
		else {break;}
	}
	return isOK;
}
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	return ((*p_manager ->p_node_space_handle->p_current_used_node_count) == 0);
}

#endif
