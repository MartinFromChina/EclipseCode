#ifndef __STRINGDEBUGMODULE_H
#define __STRINGDEBUGMODULE_H

#include "..\CommonSource\KeilMDK.h"
/*
#define M_STRING_BOOL 	int
#define M_STRING_TRUE 	(0 == 0)
#define M_STRING_FALSE 	(0 != 0)
#define M_STRING_NULL 	((void*)0)
*/
#include "StringDebugModuleConf.h"

#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2


#if (USE_STRING_DEBUG == 1)								
	#define STRING_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)            			\
					static uint16_t    CONCAT_2(p_entry_name,_whole) = init_value;				\
					static uint16_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole) ;		
					
	#define STRING_DEBUG_COUNTER_ENTRY_DEF(p_entry_name)            							\
					static uint32_t    CONCAT_2(p_entry_name,_whole) = 0;									\
					static uint32_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole) ;
#else
	#define STRING_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)            			\
					static uint8_t*   p_entry_name = X_Null ;		
					
	#define STRING_DEBUG_COUNTER_ENTRY_DEF(p_entry_name)            							\
					static uint8_t*   p_entry_name = X_Null ;
#endif



#if (USE_STRING_DEBUG == 1)
		X_Boolean OnceEntryCheck(uint16_t *backup_entry,uint16_t new_value);
		X_Boolean CounterEntryCheck(uint32_t *backup_entry,uint32_t counter_limit);
		void DebugPushString(uint8_t param_num,char *string, ...);

		#define DEBUG_POP_METHOD_0(p_string)     DEBUG_POP_METHOD(p_string)
		#define DEBUG_POP_METHOD_1(p_string,arg1) 	\
												DEBUG_POP_METHOD(p_string,arg1)
		#define DEBUG_POP_METHOD_2(p_string,arg1,arg2)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2)
		#define DEBUG_POP_METHOD_3(p_string,arg1,arg2,arg3)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3)
		#define DEBUG_POP_METHOD_4(p_string,arg1,arg2,arg3,arg4)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4)
		#define DEBUG_POP_METHOD_5(p_string,arg1,arg2,arg3,arg4,arg5)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4,arg5)
		#define DEBUG_POP_METHOD_6(p_string,arg1,arg2,arg3,arg4,arg5,arg6)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4,arg5,arg6)
		#define DEBUG_POP_METHOD_7(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
		#define DEBUG_POP_METHOD_8(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
		#define DEBUG_POP_METHOD_9(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)
		#define DEBUG_POP_METHOD_10(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)     \
												DEBUG_POP_METHOD(p_string,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)

		#define String_Debug(flag,message)   do{ 																\
														if(flag == 1)	\
														{	DEBUG_PUSH_METHOD	message ;	   	}			\
													}while(0)
		/*******************************************/

		#define String_Debug_Once(flag,entry,once_ref,message)   do{ 																\
															if(flag == 1 && OnceEntryCheck(entry,once_ref) == X_True)	\
															{	DEBUG_PUSH_METHOD	message ;	   	}			\
														}while(0)
		/*******************************************/
		#define String_Debug_Counter(flag,entry,counter_ref,message)   do{ 																\
															if(flag == 1 && CounterEntryCheck(entry,counter_ref) == X_True)	\
															{	DEBUG_PUSH_METHOD	message ;	   	}			\
														}while(0)

		/*******************************************/

#else

		#define String_Debug(flag,message)   do{ }while(0)
		/*******************************************/

		#define String_Debug_Once(flag,entry,once_ref,message)   do{ }while(0)
		#define String_Debug_Counter(flag,entry,counter_ref,message)   do{ }while(0)
		/*******************************************/

#endif

char *StringErrorTranslate(uint32_t error_code,char*(*error_get)(uint32_t error,char* p_cannot));
void StringDebugProcess(void);
void StringDebugInit(void(*p_init)(void));

/**************************************************
**********************example code*****************
***************************************************
#include "StringDebugModule.h"

uint8_t i;
#define MAIN_LOOP_DEBUG       1
STRING_DEBUG_COUNTER_ENTRY_DEF(p_main_loop);
#define COUNTER_LIMIT         0xffff

String_Debug(FOW_NOW_DEBUG,(InFront,1,"hello %s\r\n","I am string debug"));

String_Debug_Counter(MAIN_LOOP_DEBUG,p_main_loop,COUNTER_LIMIT,(0,"main loop alive\r\n"));

while(1)
{
	String_Debug_Counter(MAIN_LOOP_DEBUG,p_main_loop,COUNTER_LIMIT
											,(1,"main loop alive : %d\r\n",i++));
}

*****************************************/
#endif
