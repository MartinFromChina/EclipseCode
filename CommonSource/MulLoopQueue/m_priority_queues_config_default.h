#ifndef __M_PRIORITY_QUEUES_CONFIG_DEFAULT_H
#define __M_PRIORITY_QUEUES_CONFIG_DEFAULT_H

//#define USE_PRIORITY_QUEUE_BASED_ON_LIST_NODE
//#define USE_PRIORITY_QUEUE_BASED_ON_ARRAY
#define USE_PRIORITY_QUEUE_BASED_ON_PRIORITY_TABLE

#define USE_MY_PRIORITY_QUEUE_DEBUG                  0
#define MAX_PRIORITY_QUEUE_DEBUG_ID_COUNT            5

#ifdef USE_PRIORITY_QUEUE_BASED_ON_LIST_NODE

#endif

#ifdef USE_PRIORITY_QUEUE_BASED_ON_ARRAY

#endif

#ifdef USE_PRIORITY_QUEUE_BASED_ON_PRIORITY_TABLE
	#define MY_PRIORITY_QUEUE_MAX_NODE_NUMBER					500
	#define LIST_NODE_TABLE_SIZE         						10

	#define BIT_COUNT_IN_UINT32									32 // necessary?
	#define TYPE_DEF_PRIORITY_TABLE_VALUE						uint32_t
	/*
	 * scope : 0 ~ max_priority
	 * table size = (scope % 32 == 0) ? scope / 32 : scope / 32 + 1
	 * but the conditional judgment (scope % 32 == 0) ? is not a good way here
	 * so I lost the space to exchange sth : table size =  scope / 32 + 1;
	 *
	 */
	#define GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(scope)    (uint16_t)((scope/BIT_COUNT_IN_UINT32) + 1)
#endif


#define MAX_PRIORITY_QUEUE_COUNT     				 12



#endif
