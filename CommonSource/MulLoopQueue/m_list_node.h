#ifndef __M_LIST_NODE_H
#define __M_LIST_NODE_H

/****************************************************
 *
 */
#include "..\KeilMDK.h"
#include "..\CommonMarco.h"
#include "..\AppError.h"

#define MY_MAX_NODE_COUNT    (0xfffe)
#define MY_INVALID_NODE_CONTEXT    (0xffff)

typedef struct
{
	uint16_t next_node_address;
	uint16_t information_number;
}sNodeInformation;

typedef struct
{
	X_Boolean isInitOK;
	uint16_t first_node_address;
	uint16_t used_node_num;
}sMySingleLinkListParam;

typedef struct
{
	sNodeInformation 			*p_inf_buf;
	uint16_t  					ValidNodeNumber;
	sMySingleLinkListParam      *p_param;
}sMySingleLinkList;

#define APP_SINGLE_LIST_DEF_WITHOUT_POINTER(p_list_manager,max_node_count)            					\
		static sNodeInformation   				CONCAT_2(p_list_manager,_inf_buf)[max_node_count];		\
		static sMySingleLinkListParam 			CONCAT_2(p_list_manager,_param) = {X_False,0,0};    	\
		static const sMySingleLinkList 			CONCAT_2(p_list_manager,_list_entry) = {				\
		CONCAT_2(p_list_manager,_inf_buf)																\
		,max_node_count																					\
		,&CONCAT_2(p_list_manager,_param)																\
		};

#define APP_SINGLE_LIST_DEF(p_list_manager,max_node_count)            									\
		static sNodeInformation   				CONCAT_2(p_list_manager,_inf_buf)[max_node_count];		\
		static sMySingleLinkListParam 			CONCAT_2(p_list_manager,_param) = {X_False,0,0};    	\
		static const sMySingleLinkList 			CONCAT_2(p_list_manager,_list_entry) = {				\
		CONCAT_2(p_list_manager,_inf_buf)																\
		,max_node_count																					\
		,CONCAT_2(p_list_manager,_param)																\
		};																								\
		static const sMySingleLinkList * p_list_manager = &CONCAT_2(p_list_manager,_list_entry)

// node number : 0~ MY_MAX_NODE_COUNT
m_app_result mSingleListInit(const sMySingleLinkList * s_sll);
m_app_result mSingleListTailAdd(const sMySingleLinkList * s_sll,uint16_t infor_number);
m_app_result mSingleListTailRemove(const sMySingleLinkList * s_sll);
m_app_result mSingleListHeadAdd(const sMySingleLinkList * s_sll,uint16_t infor_number);
m_app_result mSingleListHeadRemove(const sMySingleLinkList * s_sll);
m_app_result mSingleListInsert(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t infor_number);
m_app_result mSingleListPullAway(const sMySingleLinkList * s_sll,uint16_t node_number);
X_Boolean mSingleListSizeGet(const sMySingleLinkList * s_sll,uint16_t *p_size);
X_Boolean DoesMySingleListEmpty(const sMySingleLinkList * s_sll);
X_Void mSingleListClear(const sMySingleLinkList * s_sll);

X_Boolean mSingleListFindByValue(const sMySingleLinkList * s_sll,uint16_t infor_number,uint16_t *p_node_number);
X_Boolean mSingleListInformationGetByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t *p_infor_num);
X_Boolean mSingleListUpdataValueByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t new_infor_number);


#endif
