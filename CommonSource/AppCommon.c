#include "AppCommon.h"
#include <stdlib.h>
#include "Math\bit_operation.h"
//#include "CharStringDebug\CharStringDebugModule.h"

X_Void byteBufInit(uint8_t *p_buf,uint16_t length,uint8_t init_value)
{
	uint8_t i;
	if(length >= 0xffff) {return;}
	for(i = 0;i<length;i++)
	{
		p_buf[i] = init_value;
	}
}

X_Void twobyteBufInit(uint16_t *p_buf,uint16_t length,uint16_t init_value)
{
	uint8_t i;
	if(length >= 0xffff) {return;}
	for(i = 0;i<length;i++)
	{
		p_buf[i] = init_value;
	}
}

#define MAX_BINARY_STRING_LENGTH   32
static char string[MAX_BINARY_STRING_LENGTH];
static char error_buf[] = "length error";

char *ValueToBinaryString(X_Boolean isAlign,uint8_t string_length,uint32_t value)
{
	uint8_t bitnumber,i;
	char string_backup[MAX_BINARY_STRING_LENGTH];
	// clear buf ?
	if(string_length > MAX_BINARY_STRING_LENGTH) {return error_buf;}//
	itoa(value, string, 2);

	if(isAlign == X_True)
	{
		bitnumber = GetBinaryBitNumber(value);
//		String_Debug(1,(40,"value %d;bitnumber %d\r\n",value,bitnumber));
		if(bitnumber == string_length) {return string;}
		if(bitnumber > string_length)
		{
			return &string[bitnumber - string_length];
		}
		if(bitnumber < string_length)
		{
			CopyBuffer(string,string_backup,bitnumber+1);// length unit is char ?
			for(i=0;i<(string_length - bitnumber);i++)
			{
				string[i] = '0';
			}
			CopyBuffer(string_backup,&string[string_length - bitnumber],bitnumber+1);// length unit is char ?

		}
	}
	return string;
}



#ifndef CPU_CFG_LEAD_ZEROS_ASM_PRESENT
static const uint8_t LeadZeroTable_8[256] = {
		8u,7u,6u,6u,5u,5u,5u,5u,4u,4u,4u,4u,4u,4u,4u,4u, /* 0x00 to 0x0F */
		3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u, /* 0x10 to 0x1F */
		2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u, /* 0x20 to 0x2F */
		2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u, /* 0x30 to 0x3F */
		1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u, /* 0x40 to 0x4F */
		1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u, /* 0x50 to 0x5F */
		1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u, /* 0x60 to 0x6F */
		1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u, /* 0x70 to 0x7F */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0x80 to 0x8F */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0x90 to 0x9F */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0xA0 to 0xAF */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0xB0 to 0xBF */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0xC0 to 0xCF */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0xD0 to 0xDF */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, /* 0xE0 to 0xEF */
		0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u /* 0xF0 to 0xFF */
};

uint8_t GetLeadZeroCount(uint32_t source)
{
	uint8_t nbr_lead_zeros;
	uint8_t ix;
	if (source > 0x0000FFFFu)
	{
		if (source > 0x00FFFFFFu)
		{
			ix = (uint8_t)(source >> 24u);
			nbr_lead_zeros=(LeadZeroTable_8[ix]+0u);
		}
		else
		{
			ix = (uint8_t)(source >> 16u);
			nbr_lead_zeros = (LeadZeroTable_8[ix] + 8u);
		}
	}
	else
	{
		if (source > 0x000000FFu)
		{
			ix = (uint8_t)(source >> 8u);
			nbr_lead_zeros = (LeadZeroTable_8[ix] + 16u);
		}
		else {
			ix = (uint8_t)(source >> 0u);
			nbr_lead_zeros = (LeadZeroTable_8[ix] + 24u);
		}
	}
	return (nbr_lead_zeros);
}

uint8_t GetRearZeroCount(uint32_t source)
{
	source = uint32_rotate_bit(source);
	return GetLeadZeroCount(source);
}
#endif


