#ifndef __CHARSTRINGDEBUGMODULE_H
#define __CHARSTRINGDEBUGMODULE_H

#include "..\KeilMDK.h"

//#if (USE_LOCAL_CONFIG == 0)
	#include "CharStringModuleConfig.h"
//#else
//	#include "CharStringModuleConfigExtern.h"
//#endif



#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2

#if (USE_STRING_DEBUG == 1)
	#define STRING_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)            			\
					static uint16_t    CONCAT_2(p_entry_name, _whole) = init_value;				\
					static uint16_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)

	#define STRING_DEBUG_COUNTER_ENTRY_DEF(p_entry_name,init_value)            							\
					static uint32_t    CONCAT_2(p_entry_name,_whole) = init_value;									\
					static uint32_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)
#else
	#define STRING_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)            			\
					static uint8_t*   p_entry_name = X_Null

	#define STRING_DEBUG_COUNTER_ENTRY_DEF(p_entry_name)            							\
					static uint8_t*   p_entry_name = X_Null
#endif


#if (USE_STRING_DEBUG == 1)

	X_Boolean PushStringIntoCharBuf(uint8_t length,char *string, ...);
	X_Boolean OnceEntryCheck(uint16_t *backup_entry,uint16_t new_value) ;
	X_Boolean CounterEntryCheck(uint32_t *backup_entry,uint32_t counter_limit);
	#define String_Debug(flag,message)   do{ 																\
														if(flag != 0)	\
														{	DEBUG_PUSH_METHOD	message ;	   	}			\
													}while(0)
		/*******************************************/

		#define String_Debug_Once(flag,entry,once_ref,message)   do{ 																\
															if(flag != 0 && OnceEntryCheck(entry,once_ref) == X_True)	\
															{	DEBUG_PUSH_METHOD	message ;	   	}			\
														}while(0)
		/*******************************************/
		#define String_Debug_Counter(flag,entry,counter_ref,message)   do{ 																\
															if(flag != 0 && CounterEntryCheck(entry,counter_ref) == X_True)	\
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


void OnceEntryValueInit(uint16_t * p_entry,uint16_t value);
void CounterEntryValueInit(uint32_t * p_entry,uint32_t value);
char *StringErrorTranslate(uint32_t error_code,char*(*error_get)(uint32_t error,char* p_cannot));
void StringDebugProcess(void);
void StringDebugFlush(void);
void StringDebugInit(void(*p_init)(void));

#endif
