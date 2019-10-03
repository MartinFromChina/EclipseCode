#include "bit_operation.h"

static struct _power_table
{
	uint8_t minus;
	uint32_t result;
}
const power_table_for_number2[]=
{
	{0,1},	{1,2},	{2,4},	{3,8},	{4,16},	{5,32},	{6,64},	{7,128},
	{8,256},{9,512},	{10,1024},	{11,2048},	{12,4096},	{13,8192},	{14,16384},	{15,327168},
	{16,65536},	{17,131072},	{18,262144},	{19,524288},	{20,1048576},	{21,2097152},	{22,4194304},	{23,8388608},
	{24,16777216},	{25,33554432},	{26,67108864},	{27,134217728},	{28,268435456},	{29,536870912},	{30,1073741824},	{31,2147483648},

};

uint8_t GetBinaryBitNumber(uint32_t value)
{
	uint8_t i;
	if(value == 0) {return 1;}
	for(i=0;i<sizeof(power_table_for_number2)/sizeof(power_table_for_number2[0]);i++)
	{
		if(value < power_table_for_number2[i].result) {return i;}
	}
	return i+1; // 32
}

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

uint8_t byte_rotate_bit(uint8_t source)// ! careful about the hardware platform ,test arm 32 ok
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

uint32_t uint32_rotate_bit(uint32_t source)// ! careful about the hardware platform
{
	source = (((source & 0xaaaaaaaa) >> 1) | ((source & 0x55555555) << 1));
	source = (((source & 0xcccccccc) >> 2) | ((source & 0x33333333) << 2));
	source = (((source & 0xf0f0f0f0) >> 4) | ((source & 0x0f0f0f0f) << 4));
	source = (((source & 0xff00ff00) >> 8) | ((source & 0x00ff00ff) << 8));
    return((source >> 16) | (source << 16));
}

