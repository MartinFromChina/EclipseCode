#ifndef __M_LIST_NODE_H
#define __M_LIST_NODE_H

/****************************************************
 *
 */
#include "..\KeilMDK.h"
#include "..\CommonMarco.h"
#include "..\AppError.h"

#define MY_MAX_PRIORITY_VALUE 		 (0xffff)
#define MY_MAX_NODE_COUNT   		 (0xfffe)
#define MY_INVALID_NODE_CONTEXT    	 (0xffff)

#define MAX_LISTNODE_DEBUG_ID_COUNT     5

typedef enum
{
	LO_init = 0,
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
}eListOperation;

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

typedef struct   _sMySingleLinkList    sMySingleLinkList;
struct _sMySingleLinkList
{
	sNodeInformation 			*p_inf_buf;
	uint16_t  					ValidNodeNumber;
	sMySingleLinkListParam      *p_param;
	X_Void 						(*onDebug)(eListOperation e_lop,uint8_t operation_ID,const sMySingleLinkList * s_sll);
};

typedef   X_Void (*m_list_node_debug_handler)(const sMySingleLinkList * s_sll);

typedef struct
{
	eListOperation eLO;
	m_list_node_debug_handler debug_handler[MAX_LISTNODE_DEBUG_ID_COUNT];
}sListNodeMessageDebugTable;

#define APP_SINGLE_LIST_DEF_WITHOUT_POINTER(p_list_manager,max_node_count,debug_method)            		\
		static sNodeInformation   				CONCAT_2(p_list_manager,_inf_buf)[max_node_count];		\
		static sMySingleLinkListParam 			CONCAT_2(p_list_manager,_param) = {X_False,0,0};    	\
		static const sMySingleLinkList 			CONCAT_2(p_list_manager,_list_entry) = {				\
		CONCAT_2(p_list_manager,_inf_buf)																\
		,max_node_count																					\
		,&CONCAT_2(p_list_manager,_param)																\
		,debug_method																					\
		};

#define APP_SINGLE_LIST_DEF(p_list_manager,max_node_count,debug_method)            						\
		static sNodeInformation   				CONCAT_2(p_list_manager,_inf_buf)[max_node_count];		\
		static sMySingleLinkListParam 			CONCAT_2(p_list_manager,_param) = {X_False,0,0};    	\
		static const sMySingleLinkList 			CONCAT_2(p_list_manager,_list_entry) = {				\
		CONCAT_2(p_list_manager,_inf_buf)																\
		,max_node_count																					\
		,CONCAT_2(p_list_manager,_param)																\
		,debug_method																					\
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

/************
debug example code:
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
 */


#endif
