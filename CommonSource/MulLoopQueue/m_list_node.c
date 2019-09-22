#include "m_list_node.h"

static X_Boolean ListNodeInit( data_list *dlist)
{
	if(dlist == X_Null) {return X_False;}

	dlist->NextPtr = X_Null;
	dlist->isBufFree = X_True;
	dlist->isOccupyPermit = X_True;
	return X_True;
}
/*
static uint8_t GetListLength(data_list *p_list)
{
	uint8_t i;
	for(i=0;i<255;i++)
	{
		if(p_list[i] == X_Null )
		{
			break;
		}
	}
	if(i<1){i=1;}
	return (i-1);
}
*/
static X_Void GetListState( List_Manager *p_manager)
{
	if(p_manager->FilledBufNum == 0)
	{
		p_manager->l_state = empty;
		return;
	}
	if(p_manager->FilledBufNum >= p_manager->ValidNodeNumber)
	{
		p_manager->l_state = full;
		return;
	}
	p_manager->l_state = normal;
}
X_Void queueInitialize( data_list * p_list, List_Manager *p_manager,const uint8_t length)
{
	uint8_t i;
	X_Boolean isOkFlag;

	p_manager->MaxBufNumber = length;

	for(i=0;i<p_manager->MaxBufNumber;i++)
	{

		isOkFlag = ListNodeInit(&p_list[i]);
		p_list[i].list_number = i ;
		if(isOkFlag == X_False)
		{
			break;
		}
	}
	p_manager->ValidNodeNumber = i;

	for(i=0;i < p_manager->ValidNodeNumber;i++)
	{
		if(i+1 >= p_manager->ValidNodeNumber)
		{
			p_list[i].NextPtr = &p_list[0];
		}
		else
		{
			p_list[i].NextPtr = &p_list[i+1];
		}
	}
	p_manager->FirstIn = &p_list[0];
	p_manager->FirstOut = &p_list[0];
	p_manager->l_state = empty;
	p_manager->FilledBufNum = 0 ;
	p_manager->isEmpty = X_True;
}

uint16_t QueueFirstIn( List_Manager *p_manager,X_Boolean *isOK,X_Boolean is_OccupyPermit)
{
	uint16_t buf_number;
	data_list *FI;
	FI = p_manager->FirstIn;
	GetListState(p_manager);

	buf_number = 0;
	switch(p_manager->l_state)
	{
		case empty:
		case normal:

			if(p_manager->FirstIn ->NextPtr != X_Null){p_manager->FirstIn = p_manager->FirstIn->NextPtr;}

			FI->isBufFree = X_False;
			FI->isOccupyPermit = is_OccupyPermit;
			*isOK = X_True;
			p_manager->FilledBufNum ++;
			buf_number = FI->list_number;

			p_manager->isEmpty = X_False;
		break;
		case full:
			p_manager->isEmpty = X_False;

			if(p_manager->FirstOut->isOccupyPermit == X_True)
			{
				if(p_manager->FirstIn ->NextPtr != X_Null){p_manager->FirstIn = p_manager->FirstIn->NextPtr;}
				else
				{
					*isOK = X_False;
					return 0;
				}
				if(p_manager->FirstOut ->NextPtr != X_Null){p_manager->FirstOut = p_manager->FirstOut->NextPtr;}
				else
				{
					*isOK = X_False;
					return 0;
				}

				FI->isBufFree = X_False;
				FI->isOccupyPermit = is_OccupyPermit;
				*isOK = X_True;

				buf_number = FI->list_number;
			}
			else
			{
				*isOK = X_False;
				buf_number = 0;
			}
		break;
		default:
			*isOK = X_False;
			buf_number = 0;
			p_manager->isEmpty = X_True;
		break;
	}

	return buf_number;
}
uint16_t QueueFirstOut( List_Manager *p_manager,X_Boolean *isOK)
{
	uint16_t buf_number;
	data_list *FO;
	FO = p_manager->FirstOut;

	buf_number = 0;
	GetListState(p_manager);
	switch(p_manager->l_state)
	{
		case empty:
			*isOK = X_False;
			buf_number = 0;

			p_manager->isEmpty = X_True;
		break;
		case normal:
		case full:
			//
			if(p_manager->FirstOut ->NextPtr != X_Null){p_manager->FirstOut = p_manager->FirstOut ->NextPtr;}
			else
			{
				*isOK = X_False;
				return 0;
			}

			FO->isBufFree = X_True;
			FO->isOccupyPermit = X_True;

			if(p_manager->FilledBufNum > 0) {p_manager->FilledBufNum --;}

			*isOK = X_True;
			buf_number = FO->list_number;
		break;
		default:
			*isOK = X_False;
			buf_number = 0;
			p_manager->isEmpty = X_True;
		break;
	}

	return buf_number;
}
X_Boolean DoesQueueEmpty( List_Manager *p_manager)
{
	return p_manager->isEmpty ;
}
