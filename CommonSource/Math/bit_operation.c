#include "bit_operation.h"



/*********************************************
change one bit keep others at the same time:
							1           		 0
set:		 | (000_1)        & (111_0) 

keep one bit change others at the same time:
										1           		 0
set others:		 | (111_0)        & (000_1) 
*********************************************/

// byte operation
#define ByteMask							((uint8_t)(1u << 0))

uint8_t byte_rotate_bit(uint8_t source)
{
		source = (source & 0xaa) >> 1 | (source & 0x55) << 1;
		source = (source & 0xcc) >> 2 | (source & 0x33) << 2;
		source = (source & 0xf0) >> 4 | (source & 0x0f) << 4;
////////////		SEGGER_RTT_Debug(ROTATE_BIT_DEBUG,(" source: %d \r\n",source));
		return source;
}

uint8_t byte_setbit(uint8_t source,uint8_t bitnumber)
{
	source = source | (ByteMask << bitnumber);
	return source;
}
uint8_t byte_clearbit(uint8_t source,uint8_t bitnumber)
{
	source = source & (~(ByteMask << bitnumber));
	return source;
}
uint8_t byte_getbit(uint8_t source,uint8_t bitnumber)
{
	if((source & (ByteMask << bitnumber)) != 0)
	{
		return 1;
	}
	return 0;
}
uint8_t byte_bitoverturn(uint8_t source,uint8_t bitnumber)
{
	uint8_t bitmask = ByteMask;
	bitmask = bitmask << bitnumber;
	source = source ^ bitmask;
	return source;
	
}

uint8_t ByteFactorSet(uint8_t factor,uint8_t bit_number,X_Boolean SetOrClear)
{
	uint8_t  value;
	if(SetOrClear == Set)
	{
		value = byte_setbit(factor,bit_number);
	}
	else
	{
		value = byte_clearbit(factor,bit_number);
}

	return value;
}


//uint16_t operation 
#define TwoByteMask							((uint16_t)(1u << 0))

uint16_t twobyte_setbit(uint16_t source,uint8_t bitnumber)
{
	source = source | (TwoByteMask << bitnumber);
	return source;
}
uint16_t twobyte_clearbit(uint16_t source,uint8_t bitnumber)
{
	source = source & (~(TwoByteMask << bitnumber));
	return source;
}
uint16_t twobyte_getbit(uint16_t source,uint8_t bitnumber)
{
	if((source & (TwoByteMask << bitnumber)) != 0)
	{
		return 1;
	}
	return 0;
}

uint16_t twobyte_bitoverturn(uint16_t source,uint8_t bitnumber)
{
	uint16_t bitmask = TwoByteMask;
	bitmask = bitmask << bitnumber;
	source = source ^ bitmask;
	return source;
	
}

uint16_t TwoByteFactorSet(uint16_t factor,uint8_t bit_number,X_Boolean SetOrClear)
{
	uint16_t  value;
	if(SetOrClear == Set)
	{
		value = twobyte_setbit(factor,bit_number);
	}
	else
	{
		value = twobyte_clearbit(factor,bit_number);
}

	return value;
}

