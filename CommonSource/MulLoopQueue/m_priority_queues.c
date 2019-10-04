#include "m_priority_queues.h"
/*
 ***********************************************************************************************
 *1,push node into queue with priority
 *if same priority is pushed ,the previous one will be replace by the new one
 *2,pop node by priority (from small to large or from large to small could be config)
 *3,user can check the queue state include : (is empty ? min priority ,max priority , cunrrent used node number and so on)
 ***********************************************************************************************
 */
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
	prio = (size-1) * BIT_COUNT_IN_UINT32;
	isAllZero = X_True;
	for(i=size;i>0;i--)
	{
		if(p_table[i-1] != 0) {isAllZero = X_False;break;}
		else {prio -= BIT_COUNT_IN_UINT32;}
	}
	if( isAllZero == X_True )  {return X_False;}
	rear_zero_count = GetRearZeroCount(p_table[i-1]);
//printf("!!!!!prio %d ,source %2x ; rear_zero_count %d \r\n",prio,p_table[i-1],rear_zero_count);
	prio +=(uint16_t)(BIT_COUNT_IN_UINT32 - rear_zero_count);
	if(prio >= 1) {prio = prio - 1;}
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
	uint16_t i;
	X_Boolean isOK,isInsertAgain;
	sListNodeWithPriority * p_current_node;
	sListNodeWithPriority * p_next_node;
	sListNodeWithPriority * p_head_node;

	if(p_space == X_Null || p_node_ID == X_Null) {return APP_POINTER_NULL;}

	if(PrioroityQueueBorrowNode(p_space,&p_next_node) == X_True)
	{
		p_next_node ->priority = priority;
		p_next_node ->NextPtr  = X_Null;
		*p_node_ID = p_next_node ->node_ID_in_list_array;
	}
	else {return APP_NO_ENOUGH_SPACE;}


	p_head_node = p_space->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE];
	isInsertAgain = X_False;
	if(p_head_node == X_Null) {p_space->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE] = p_next_node;}
	else
	{
		p_current_node = p_head_node;
		isOK = X_False;
		for(i=0;i<p_space ->max_node_count;i++)
		{
			if(p_current_node ->NextPtr != X_Null)
			{
				if(p_current_node ->priority == priority)  // same priority insert again , no need new node ,reconfig the old one
				{
					isOK 		  = X_True;
					isInsertAgain = X_True;
//			printf("!!!!! 1,same priority insert again %d \r\n",priority);
					PrioroityQueueReturnNode(p_space,p_next_node ->node_ID_in_list_array);
					*p_node_ID = p_current_node ->node_ID_in_list_array;
					break;
				}
				p_current_node = p_current_node ->NextPtr;
			}
			else
			{
				isOK = X_True;
				if(p_current_node ->priority == priority)  // same priority insert again , no need new node ,reconfig the old one
				{
					isInsertAgain = X_True;
//			printf("!!!!! 2,same priority insert again %d \r\n",priority);
					PrioroityQueueReturnNode(p_space,p_next_node ->node_ID_in_list_array);
					*p_node_ID = p_current_node ->node_ID_in_list_array;
					break;
				}
				p_current_node ->NextPtr = p_next_node;
				break;
			}
		}
		if(isOK == X_False)
		{
			PrioroityQueueReturnNode(p_space,p_next_node ->node_ID_in_list_array);
			return APP_ERROR;
		}
	}
	if(isInsertAgain == X_False){(*p_space ->p_current_used_node_count) ++;}
	return APP_SUCCESSED;

}
static m_app_result NodePullAway(sPriorityQueueListNodeSpace const*p_space,uint16_t priority,uint16_t *p_node_ID)
{
	uint16_t i;
	X_Boolean isOK;
	sListNodeWithPriority * p_current_node;
	sListNodeWithPriority * p_node_going_to_drop;
	sListNodeWithPriority * p_left_node;
	sListNodeWithPriority * p_right_node;
	sListNodeWithPriority *p_node_head;

	if(p_space == X_Null || p_node_ID == X_Null) {return APP_POINTER_NULL;}
	if((*p_space ->p_current_used_node_count) == 0){return APP_ALREADY_DONE_BEFORE;}

	p_node_head = p_space ->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE];
	if(p_node_head == X_Null) {return APP_POINTER_NULL;}

	if(p_node_head ->NextPtr == X_Null)
	{
		if(p_node_head ->priority != priority) {return APP_UNEXPECT_STATE;}
		if(PrioroityQueueReturnNode(p_space,p_node_head ->node_ID_in_list_array) == X_False) {return APP_ERROR;}
		*p_node_ID = p_node_head ->node_ID_in_list_array;
		p_space->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE] = X_Null;
		(*p_space ->p_current_used_node_count) --;

		return APP_SUCCESSED;
	}

	p_current_node = p_node_head;
	p_left_node    = X_Null;
	p_right_node   = X_Null;
	isOK = X_False;
	for(i=0;i<p_space ->max_node_count;i++)
	{
		if(p_current_node ->priority == priority)
		{
			isOK = X_True;
			p_node_going_to_drop = p_current_node;
			p_right_node         = p_node_going_to_drop ->NextPtr; // maybe null !!!
			*p_node_ID = p_node_going_to_drop ->node_ID_in_list_array;
			break;
		}
		if(p_current_node ->NextPtr != X_Null)
		{
			p_left_node = p_current_node;
			p_current_node = p_current_node ->NextPtr;
		}
		else
		{
			if(p_current_node ->priority == priority)
			{
				isOK = X_True;
				p_node_going_to_drop = p_current_node;
				p_right_node         = p_node_going_to_drop ->NextPtr; // maybe null !!!
				*p_node_ID = p_node_going_to_drop ->node_ID_in_list_array;
			}
			break;
		}
	}

	if(isOK == X_False) {return APP_UNEXPECT_STATE;}
//	if(p_left_node == X_Null && p_right_node == X_Null) {return APP_UNEXPECT_STATE;} // not necessary

	if(p_left_node == X_Null) {p_space->p_list_head_nodes[priority % LIST_NODE_TABLE_SIZE] = p_right_node;}
	else {p_left_node->NextPtr = p_right_node;}

	if(PrioroityQueueReturnNode(p_space,p_node_going_to_drop ->node_ID_in_list_array) == X_False) {return APP_ERROR;}
	(*p_space ->p_current_used_node_count) --;
	return APP_SUCCESSED;
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
	}
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
	check_scope = (uint16_t)(( p_manager ->MaxPriorityValue/ LIST_NODE_TABLE_SIZE) + 1);
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
