#include "m_list_node.h"

static X_Boolean BorrowNode(sMySingleLinkListParam const*p_node_param,sListNodeSpace *p_node)
{
	uint16_t i;
	X_Boolean isOK;
	if(p_node_param == X_Null || p_node == X_Null) {return X_False;}// necessary?

	isOK = X_False;
	for(i=0;p_node_param ->ValidNodeNumber;i++)
	{
		if(p_node_param ->p_node_space[i].isFree == X_True)
		{
			p_node_param ->p_node_space[i].isFree = X_False;
			p_node = &(p_node_param ->p_node_space[i]);
			isOK = X_True;
		}
	}
	return isOK;
}

static X_Boolean ReturnNode(sMySingleLinkListParam const*p_node_param,uint16_t node_number)
{
	uint16_t i;
	X_Boolean isOK;
	if(p_node_param == X_Null ) {return X_False;}// necessary?
	if(node_number >= p_node_param ->ValidNodeNumber)  {return X_False;}

	p_node_param ->p_node_space[node_number].isFree = X_True;
	p_node_param ->p_node_space[node_number].NextPtr = X_Null;

	return X_True;
}

static X_Boolean NodePointerGetByNodeOrderNumber(sMySingleLinkListParam const*p_node_param,uint16_t first_node_number
												,uint16_t node_order // 0 means the first used node   ; 1 means the second  used node ...
												,sListNodeSpace *p_node)
{
	uint16_t i;
	X_Boolean isOK;

	if(p_node_param == X_Null || p_node == X_Null) {return X_False;}// necessary?
	if( first_node_number >= p_node_param ->ValidNodeNumber || node_order >= p_node_param ->ValidNodeNumber) {return X_False;}// necessary?

	p_node = &(p_node_param ->p_node_space[first_node_number]);

	isOK = X_True;
	for(i=0;i<node_order;i++)
	{
		if(p_node ->NextPtr != X_Null)
		{
			p_node = p_node ->NextPtr;
		}
		else
		{
			isOK = X_False;
			break;
		}

	}
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
	s_sll ->current_head_node_number	= 0;
	s_sll ->used_node_num  				= 0;
	s_sll ->isInitOK       				= X_True;
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_init,0,s_sll);}
	return APP_SUCCESSED;
}
m_app_result mSingleListTailAdd(const sMySingleLinkList * s_sll,uint16_t infor_number)
{
	sListNodeSpace * p_current_node;
	sListNodeSpace * p_next_node;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(s_sll ->isInitOK == X_False){return APP_ERROR;}

	if(BorrowNode(s_sll ->p_param,p_next_node) == X_False) {return APP_ERROR;}

	p_next_node ->extern_information = infor_number;
	p_next_node ->NextPtr			   = X_Null;

	if(s_sll ->used_node_num == 0)
	{
		s_sll ->current_head_node_number                                   = p_next_node ->node_ID_in_list_array;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailAdd,0,s_sll);}
	}
	else
	{
		if(NodePointerGetByNodeOrderNumber(s_sll ->p_param,s_sll ->current_head_node_number
									,(s_sll ->used_node_num - 1),p_current_node) == X_False)
		{
			ReturnNode(s_sll ->p_param,p_next_node ->node_ID_in_list_array);
			return APP_ERROR;
		}

		p_current_node ->NextPtr = p_next_node;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailAdd,1,s_sll);}
	}
	s_sll ->used_node_num ++;
	return APP_SUCCESSED;
}
m_app_result mSingleListTailRemove(const sMySingleLinkList * s_sll)
{
	uint16_t  current_node_address,previous_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->isInitOK == X_False){return APP_ERROR;}

	if(s_sll ->p_param ->used_node_num > 0) //
	{
		current_node_address = CurrentNodeAddressGet(s_sll ->p_param ->used_node_num,s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		if(s_sll ->p_param ->used_node_num == 1) //
		{
			previous_node_address = current_node_address;
		}
		else
		{
			previous_node_address = NodeAddressStepBack(s_sll->ValidNodeNumber,current_node_address);
		}
		s_sll ->p_inf_buf[previous_node_address].next_node_address = MY_INVALID_NODE_CONTEXT;
		s_sll ->p_param ->used_node_num --;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailRemove,0,s_sll);}

		return APP_SUCCESSED;
	}
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_TailRemove,0,s_sll);}
	return APP_ALREADY_DONE_BEFORE;

}
m_app_result mSingleListHeadAdd(const sMySingleLinkList * s_sll,uint16_t infor_number)
{
	uint16_t  front_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(s_sll ->p_param ->isInitOK == X_False){return APP_ERROR;}

	if(s_sll ->p_param ->used_node_num > 0)
	{
		front_node_address  = NodeAddressStepBack(s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		s_sll ->p_inf_buf[front_node_address].next_node_address = s_sll ->p_param ->first_node_address;
	}
	else
	{
		front_node_address    = s_sll ->p_param ->first_node_address;
	}
	s_sll ->p_inf_buf[front_node_address].information_number = infor_number;
	s_sll ->p_param ->used_node_num ++;
	s_sll ->p_param ->first_node_address = front_node_address;
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadAdd,0,s_sll);}
	return APP_SUCCESSED;
}
m_app_result mSingleListHeadRemove(const sMySingleLinkList * s_sll)
{
	uint16_t  next_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->isInitOK == X_False){return APP_ERROR;}

	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,0,s_sll);}

	if(s_sll ->p_param ->used_node_num > 0) //
	{
		if(s_sll ->p_param ->used_node_num == 1) //
		{
			next_node_address = s_sll ->p_param ->first_node_address;
		}
		else
		{
			next_node_address = NodeAddressMoveForward(s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		}
		s_sll ->p_inf_buf[s_sll ->p_param ->first_node_address].next_node_address = MY_INVALID_NODE_CONTEXT;
		s_sll ->p_param ->used_node_num --;
		s_sll ->p_param ->first_node_address = next_node_address;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,1,s_sll);}
		return APP_SUCCESSED;
	}
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_HeadRemove,2,s_sll);}
	return APP_ALREADY_DONE_BEFORE;
}
m_app_result mSingleListInsert(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t infor_number)
{
	uint16_t next_empty_node_addr,latest_used_node_addr,right_addr;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->isInitOK == X_False){return APP_ERROR;}
	if(s_sll ->p_param ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}
	if(node_number > (s_sll ->p_param ->used_node_num + 1)) {return APP_BEYOND_SCOPE;}

	if(node_number == 0)// insert head
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,0,s_sll);}
		return mSingleListHeadAdd(s_sll,infor_number);
	}
	else if(node_number == (s_sll ->p_param ->used_node_num )) // inset tail
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,1,s_sll);}
		return mSingleListTailAdd(s_sll,infor_number);
	}
	else
	{
		// get new node address  (get m)
		latest_used_node_addr = CurrentNodeAddressGet(s_sll ->p_param ->used_node_num,s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		next_empty_node_addr = NodeAddressMoveForward(s_sll->ValidNodeNumber,latest_used_node_addr);
		// origin right node  addr backup from left node (back up r from l)
		if(GetNextNodeAddrByNodeNumber(s_sll->p_inf_buf,node_number,s_sll ->p_param ->first_node_address,&right_addr) == X_False) {return APP_ERROR;}
		if(right_addr >= s_sll ->ValidNodeNumber)
		{
			if(s_sll ->onDebugParamCollect != X_Null) {s_sll ->onDebugParamCollect(SQO_Push,right_addr);}
			if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,2,s_sll);}
			if(s_sll ->onDebugParamCollect != X_Null) {s_sll ->onDebugParamCollect(SQO_Clear,0);}
			return APP_ERROR;
		}
		// left node relink to new node  addr (l = m)
		if(SetNextNodeAddrByNodeNumber(s_sll->p_inf_buf,node_number - 1,s_sll ->p_param ->first_node_address,next_empty_node_addr) == X_False) {return APP_ERROR;}
		// new node  link to origin right node  addr (m = r)
		s_sll ->p_inf_buf[next_empty_node_addr].information_number = infor_number;
		s_sll ->p_inf_buf[next_empty_node_addr].next_node_address  = right_addr;

		s_sll ->p_param ->used_node_num ++;
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_Insert,3,s_sll);}
		return APP_SUCCESSED;
	}

}
m_app_result mSingleListPullAway(const sMySingleLinkList * s_sll,uint16_t node_number)
{
	uint16_t current_addr;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->isInitOK == X_False){return APP_ERROR;}

	if(node_number >= s_sll ->p_param ->used_node_num) {return APP_PARAM_ERROR;}
	if(s_sll ->p_param ->used_node_num == 0){return APP_ALREADY_DONE_BEFORE;}

	if(s_sll ->p_param ->used_node_num ==  (node_number + 1)) //
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,0,s_sll);}
		return mSingleListTailRemove(s_sll);
	}

	if(node_number == 0)
	{
		if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,1,s_sll);}
		return mSingleListHeadRemove(s_sll);
	}

	if(GetNextNodeAddrByNodeNumber(s_sll->p_inf_buf,node_number - 1,s_sll ->p_param ->first_node_address,&current_addr) == X_False) {return APP_ERROR;}
	if( current_addr >= s_sll ->ValidNodeNumber){return APP_ERROR;}
	if(SetNextNodeAddrByNodeNumber(s_sll->p_inf_buf,node_number - 1,s_sll ->p_param ->first_node_address
			,s_sll->p_inf_buf[current_addr].next_node_address) == X_False) {return APP_ERROR;}
	s_sll->p_inf_buf[current_addr].next_node_address = MY_INVALID_NODE_CONTEXT; // not necessary ?
	s_sll ->p_param ->used_node_num --;
	if(s_sll ->onDebug != X_Null) {s_sll ->onDebug(LO_PullAway,2,s_sll);}
	return APP_SUCCESSED;

}
X_Boolean mSingleListSizeGet(const sMySingleLinkList * s_sll,uint16_t *p_size)
{
	if(s_sll == X_Null || p_size == X_Null) {return X_False;}
	*p_size = s_sll ->p_param ->used_node_num ;
	return X_True;
}
X_Boolean DoesMySingleListEmpty(const sMySingleLinkList * s_sll)
{
	if(s_sll == X_Null) {return X_False;}
	return (s_sll ->p_param ->used_node_num == 0);
}
X_Void mSingleListClear(const sMySingleLinkList * s_sll)
{
	mSingleListInit(s_sll);
}

X_Boolean mSingleListFindByValue(const sMySingleLinkList * s_sll,uint16_t infor_number,uint16_t *p_node_number)
{
	uint16_t i,addr;
	X_Boolean isFind;
	if(s_sll == X_Null || p_node_number == X_Null) {return X_False;}
	if(s_sll ->p_param ->isInitOK == X_False){return X_False;}
	if(s_sll->p_param ->used_node_num == 0)	{return X_False;}

	isFind = X_False;
	addr = s_sll ->p_param ->first_node_address;
	for(i=0;i<s_sll->p_param ->used_node_num;i++)
	{
		if(addr >= s_sll ->ValidNodeNumber) {break;}
		if(s_sll ->p_inf_buf[addr].information_number == infor_number)
		{
			*p_node_number = i;
			isFind = X_True;
			break;
		}
		addr = s_sll -> p_inf_buf[addr].next_node_address;
	}
	return isFind;
}
X_Boolean mSingleListInformationGetByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t *p_infor_num)
{
	uint16_t i,addr;
	X_Boolean isOK;
	if(s_sll == X_Null || p_infor_num == X_Null) {return X_False;}
	if(s_sll ->p_param ->isInitOK == X_False){return X_False;}
	if(s_sll->p_param ->used_node_num == 0)	{return X_False;}
	if(node_number > s_sll->p_param ->used_node_num) {return X_False;}

	isOK = X_True;
	addr = s_sll ->p_param ->first_node_address;

	for(i=0;i<node_number;i++)
	{
		if(addr >= s_sll ->ValidNodeNumber) {isOK = X_False;break;}
		addr = s_sll -> p_inf_buf[addr].next_node_address;
	}
	*p_infor_num = s_sll ->p_inf_buf[addr].information_number;
	return isOK;
}
X_Boolean mSingleListUpdataValueByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t new_infor_number)
{
	uint16_t i,addr;
	X_Boolean isOK;
	if(s_sll == X_Null ) {return X_False;}
	if(s_sll ->p_param ->isInitOK == X_False){return X_False;}
	if(s_sll->p_param ->used_node_num == 0)	{return X_False;}
	if(node_number > s_sll->p_param ->used_node_num) {return X_False;}

	isOK = X_True;
	addr = s_sll ->p_param ->first_node_address;
	for(i=0;i<node_number;i++)
	{
		if(addr >= s_sll ->ValidNodeNumber) {isOK = X_False;break;}
		addr = s_sll -> p_inf_buf[addr].next_node_address;
	}
    s_sll ->p_inf_buf[addr].information_number = new_infor_number;
	return isOK;
}
