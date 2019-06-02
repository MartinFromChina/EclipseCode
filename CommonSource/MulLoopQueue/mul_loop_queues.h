#ifndef __MUL_LOOP_QUEUES_H
#define __MUL_LOOP_QUEUES_H

/****************************************************
 * the loop queue can't be interrupt by an irq !!!
 */

#include "stdbool.h"
#include "..\KeilMDK.h"


typedef struct DataList data_list;
struct DataList
{
	X_Boolean 	isBufFree;
	X_Boolean 	isOccupyPermit;
	uint16_t 	list_number;
	data_list   *NextPtr;
};

typedef enum
{
	empty	=	0,
	normal,
	full,
}list_state;

typedef struct
{
	list_state  l_state;
	uint16_t    FilledBufNum;
	uint16_t    MaxBufNumber;
	uint16_t  	ValidNodeNumber;
	data_list 	*FirstIn;
	data_list 	*FirstOut;
}List_Manager;

#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2

#define APP_LOOPQUEUE_DEF(id,id_manager,id_length,buf_num)            					\
		static const  uint16_t    id_length = buf_num;										\
		static data_list   CONCAT_2(id,_list)[buf_num];								\
		static data_list 	* id =   &CONCAT_2(id,_list)[0];								\
		static List_Manager CONCAT_2(id,_listmanager) ;      		\
		static List_Manager * id_manager = &CONCAT_2(id,_listmanager)

X_Void queueInitialize( data_list * p_list,List_Manager *p_manager,uint8_t length);
uint16_t QueueFirstIn( List_Manager *p_manager,X_Boolean *isOK,X_Boolean is_OccupyPermit);
uint16_t QueueFirstOut( List_Manager *p_manager,X_Boolean *isOK);
X_Boolean DoesQueueEmpty(X_Void);

#endif
