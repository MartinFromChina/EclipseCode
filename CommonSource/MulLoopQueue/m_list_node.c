#include "m_list_node.h"

static X_Boolean BorrowNode(sMySingleLinkListParam const*p_node_param,sListNodeSpace **p_node)
{
	uint16_t i;
	X_Boolean isOK;
	if(p_node_param == X_Null || *p_node == X_Null) {return X_False;}// necessary?

	isOK = X_False;
	for(i=0;p_node_param ->ValidNodeNumber;i++)
	{
		if(p_node_param ->p_node_space[i].isFree == X_True)
		{
			p_node_param ->p_node_space[i].isFree = X_False;
			*p_node = &(p_node_param ->p_node_space[i]);
			isOK = X_True;
		}
	}
	return isOK;
}

static X_Boolean ReturnNode(sMySingleLinkListParam const*p_node_param,uint16_t node_number)
{
	if(p_node_param == X_Null ) {return X_False;}// necessary?
	if(node_number >= p_node_param ->ValidNodeNumber)  {return X_False;}

	p_node_param ->p_node_space[node_number].isFree = X_True;
	p_node_param ->p_node_space[node_number].NextPtr = X_Null;

	return X_True;
}

static X_Boolean NodePointerGetByNodeOrderNumber(sMySingleLinkListParam const*p_node_param,uint16_t first_node_number
												,uint16_t node_order // 0 means the first used node   ; 1 means the second  used node ...
												,sListNodeSpace **p_node)
{
	uint16_t i;
	X_Boolean isOK;
	sListNodeSpace *p_temp;

	if(p_node_param == X_Null || *p_node == X_Null) {return X_False;}// necessary?
	if( first_node_number >= p_node_param ->ValidNodeNumber || node_order >= p_node_param ->ValidNodeNumber) {return X_False;}// necessary?

	p_temp = &(p_node_param ->p_node_space[first_node_number]);

	isOK = X_True;
	for(i=0;i<node_order;i++)
	{
		if(p_temp ->NextPtr != X_Null)
		{
			p_temp = p_temp ->NextPtr;
		}
		else
		{
			isOK = X_False;
			break;
		}

	}
	*p_node = p_temp;
	return isOK;
}

m_app_result mSingleListInit(const sMySingleLinkList * s_sll)
{
	uint16_t i;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll->ValidNodeNumber > MY_MAX_NODE_COUNT) {return APP_BEYOND_SCOPE;}
	if(s_sll ->p_param ->ValidNodeNumber > MY_MAX_NODE_COUNT) {return APP_BEYOND_SCOPE;}



	for(i=0;i<s_sll ->p_param ->ValidNodeNumber;i++)
	{
		s_sll ->p_param ->p_node_space[i].isFree	  = X_True;
		s_sll ->p_param ->p_node_space[i].extern_information = 0;
		s_sll ->p_param ->p_node_space[i].NextPtr     = X_Null;
		s_sll ->p_param ->p_node_space[i].node_ID_in_list_array = i;
	}
	s_sll ->p_manager ->current_head_node_number	= 0;
	s_sll ->p_manager ->used_node_num  				= 0;
	s_sll ->p_manager ->isInitOK       				= X_True;
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_init,0,s_sll);}
	return APP_SUCCESSED;
}
m_app_result mSingleListTailAdd(const sMySingleLinkList * s_sll,uint16_t infor_number)
{
	sListNodeSpace * p_current_node;
	sListNodeSpace * p_next_node;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_manager ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}

	if(BorrowNode(s_sll ->p_param,&p_next_node) == X_False) {return APP_NO_ENOUGH_SPACE;}

	p_next_node ->extern_information = infor_number;
	p_next_node ->NextPtr			   = X_Null;

	if(s_sll ->p_manager ->used_node_num == 0)
	{
		s_sll ->p_manager ->current_head_node_number = p_next_node ->node_ID_in_list_array;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailAdd,0,s_sll);}
	}
	else
	{
		if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
									,(s_sll ->p_manager ->used_node_num - 1),&p_current_node) == X_False)
		{
			ReturnNode(s_sll ->p_param,p_next_node ->node_ID_in_list_array);
			return APP_ERROR;
		}

		p_current_node ->NextPtr = p_next_node;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailAdd,1,s_sll);}
	}
	s_sll ->p_manager ->used_node_num ++;
	return APP_SUCCESSED;
}
m_app_result mSingleListTailRemove(const sMySingleLinkList * s_sll)
{
	sListNodeSpace * p_current_node;
	sListNodeSpace * p_previous_node;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}

	if(s_sll  ->p_manager ->used_node_num > 0) //
	{
		if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
											,(s_sll ->p_manager ->used_node_num - 1),&p_current_node) == X_False) {return APP_ERROR;}

		if(s_sll ->p_manager ->used_node_num == 1) //
		{
			p_previous_node = p_current_node;
		}
		else
		{
			if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
											,(s_sll ->p_manager ->used_node_num - 2),&p_previous_node) == X_False) {return APP_ERROR;}
		}

		p_previous_node ->NextPtr = X_Null;
		if(ReturnNode(s_sll ->p_param,p_current_node ->node_ID_in_list_array) == X_True)
		{
			s_sll ->p_manager ->used_node_num --;
			if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailRemove,0,s_sll);}
			return APP_SUCCESSED;
		}
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailRemove,1,s_sll);}
		return APP_ERROR;
	}
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailRemove,2,s_sll);}
	return APP_ALREADY_DONE_BEFORE;

}
m_app_result mSingleListHeadAdd(const sMySingleLinkList * s_sll,uint16_t infor_number)
{
	sListNodeSpace * p_old_head_node;
	sListNodeSpace * p_new_head_node;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_manager ->used_node_num >= s_sll->ValidNodeNumber || s_sll ->p_manager ->current_head_node_number >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}

	if(BorrowNode(s_sll ->p_param,&p_new_head_node) == X_False) {return APP_NO_ENOUGH_SPACE;}

	if(s_sll ->p_manager ->used_node_num == 0)
	{
		p_new_head_node ->NextPtr = X_Null;
	}
	else
	{
		p_old_head_node = &(s_sll ->p_param ->p_node_space[s_sll ->p_manager ->current_head_node_number]);
		p_new_head_node ->NextPtr = p_old_head_node;
	}

	p_new_head_node ->extern_information = infor_number;
	s_sll ->p_manager ->current_head_node_number 	 = p_new_head_node ->node_ID_in_list_array;
	s_sll ->p_manager ->used_node_num ++;
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadAdd,0,s_sll);}
	return APP_SUCCESSED;
}
m_app_result mSingleListHeadRemove(const sMySingleLinkList * s_sll)
{
	sListNodeSpace * p_head_node;
	sListNodeSpace * p_second_node;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_manager ->current_head_node_number >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}

	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,0,s_sll);}

	if(s_sll ->p_manager ->used_node_num > 0) //
	{
		p_head_node = &(s_sll ->p_param ->p_node_space[s_sll ->p_manager ->current_head_node_number]);
		if(s_sll ->p_manager ->used_node_num == 1) //
		{
			p_second_node = p_head_node;
		}
		else
		{
			if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
												,1,&p_second_node) == X_False) {return APP_ERROR;}
		}

		if(ReturnNode(s_sll ->p_param,p_head_node ->node_ID_in_list_array) == X_True)
		{
			s_sll ->p_manager ->current_head_node_number = p_second_node ->node_ID_in_list_array;
			s_sll ->p_manager ->used_node_num --;
			if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,1,s_sll);}
			return APP_SUCCESSED;
		}
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,2,s_sll);}
		return APP_ERROR;
	}
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,3,s_sll);}
	return APP_ALREADY_DONE_BEFORE;
}
m_app_result mSingleListInsert(const sMySingleLinkList * s_sll,uint16_t node_order_number,uint16_t infor_number)
{
	sListNodeSpace * p_new_node;
	sListNodeSpace * p_left_node;
	sListNodeSpace * p_right_node;

	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}
	if(s_sll ->p_manager ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(node_order_number > (s_sll ->p_manager ->used_node_num )) {return APP_PARAM_ERROR;}

	if(node_order_number == 0)// insert head
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,0,s_sll);}
		return mSingleListHeadAdd(s_sll,infor_number);
	}
	else if(node_order_number == (s_sll ->p_manager ->used_node_num )) // inset tail
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,1,s_sll);}
		return mSingleListTailAdd(s_sll,infor_number);
	}
	else
	{
		// get new node address  (get m)
		if(BorrowNode(s_sll ->p_param,&p_new_node) == X_False) {return APP_NO_ENOUGH_SPACE;}
		if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
											,(node_order_number - 1),&p_left_node) == X_False)
		{
			ReturnNode(s_sll ->p_param,p_new_node ->node_ID_in_list_array);
			return APP_ERROR;
		}
		// origin right node  addr backup from left node (back up r from l)
		p_right_node = p_left_node ->NextPtr;
		// left node relink to new node  addr (l = m)
		p_left_node ->NextPtr = p_new_node;
		// new node  link to origin right node  addr (m = r)
		p_new_node ->NextPtr = p_right_node;

		p_new_node ->extern_information = infor_number;
		s_sll ->p_manager ->used_node_num ++;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,2,s_sll);}
		return APP_SUCCESSED;
	}

}
m_app_result mSingleListPullAway(const sMySingleLinkList * s_sll,uint16_t node_order_number)
{
	sListNodeSpace * p_node_going_to_drop;
	sListNodeSpace * p_left_node;
	sListNodeSpace * p_right_node;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_manager ->isInitOK == X_False){return APP_UNEXPECT_STATE;}

	if(node_order_number >= s_sll ->p_manager ->used_node_num) {return APP_PARAM_ERROR;}
	if(s_sll ->p_manager ->used_node_num == 0){return APP_ALREADY_DONE_BEFORE;}

	if(s_sll ->p_manager ->used_node_num ==  (node_order_number + 1)) //
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,0,s_sll);}
		return mSingleListTailRemove(s_sll);
	}

	if(node_order_number == 0)
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,1,s_sll);}
		return mSingleListHeadRemove(s_sll);
	}

	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
												,(node_order_number),&p_node_going_to_drop) == X_False){return APP_ERROR;}
	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
													,(node_order_number - 1),&p_left_node) == X_False) {return APP_ERROR;}
	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
														,(node_order_number + 1),&p_right_node) == X_False) {return APP_ERROR;}

	p_left_node->NextPtr = p_right_node;
	s_sll ->p_manager ->used_node_num --;
	if(ReturnNode(s_sll ->p_param,p_node_going_to_drop ->node_ID_in_list_array) == X_True)
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,2,s_sll);}
		return APP_SUCCESSED;
	}
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,3,s_sll);}
	return APP_ERROR;

}
X_Boolean mSingleListSizeGet(const sMySingleLinkList * s_sll,uint16_t *p_size)
{
	if(s_sll == X_Null || p_size == X_Null) {return X_False;}
	*p_size = s_sll ->p_manager ->used_node_num ;
	return X_True;
}
X_Boolean DoesMySingleListEmpty(const sMySingleLinkList * s_sll)
{
	if(s_sll == X_Null) {return X_False;}
	return (s_sll ->p_manager ->used_node_num == 0);
}
X_Void mSingleListClear(const sMySingleLinkList * s_sll)
{
	mSingleListInit(s_sll);
}

X_Boolean mSingleListFindByValue(const sMySingleLinkList * s_sll,uint16_t infor_number,uint16_t *p_node_order_number)
{
	uint16_t i;
	sListNodeSpace * p_node;
	X_Boolean isFind;
	if(s_sll == X_Null || p_node_order_number == X_Null) {return X_False;}
	if(s_sll ->p_manager->isInitOK == X_False){return X_False;}
	if(s_sll ->p_manager->used_node_num == 0)	{return X_False;}
	if(s_sll ->p_manager ->current_head_node_number >= s_sll ->ValidNodeNumber) {return X_False;}

	isFind = X_False;
	p_node = &(s_sll ->p_param ->p_node_space[s_sll ->p_manager ->current_head_node_number]);
	for(i=0;i<s_sll->p_manager ->used_node_num;i++)
	{
		if(p_node ->extern_information == infor_number)
		{
			*p_node_order_number = p_node ->node_ID_in_list_array;
			isFind = X_True;
			break;
		}
		if(p_node ->NextPtr != X_Null)
		{
			p_node = p_node ->NextPtr;
		}
		else
		{
			break;
		}
	}
	return isFind;
}
X_Boolean mSingleListInformationGetByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_order_number,uint16_t *p_infor_num)
{
	sListNodeSpace * p_node;
	if(s_sll == X_Null || p_infor_num == X_Null) {return X_False;}
	if(s_sll ->p_manager ->isInitOK == X_False){return X_False;}
	if(s_sll->p_manager ->used_node_num == 0)	{return X_False;}
	if(node_order_number >= s_sll->p_manager ->used_node_num) {return X_False;}

	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
															,node_order_number,&p_node) == X_False)      {return X_False;}
	*p_infor_num = p_node ->extern_information;
	return X_True;
}
X_Boolean mSingleListUpdataValueByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_order_number,uint16_t new_infor_number)
{
	sListNodeSpace * p_node;
	if(s_sll == X_Null) {return X_False;}
	if(s_sll ->p_manager ->isInitOK == X_False){return X_False;}
	if(s_sll->p_manager ->used_node_num == 0)	{return X_False;}
	if(node_order_number >= s_sll->p_manager ->used_node_num) {return X_False;}

	if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->p_manager ->current_head_node_number
																,node_order_number,&p_node) == X_False)      {return X_False;}
	p_node->extern_information = new_infor_number;
	return X_True;
}
