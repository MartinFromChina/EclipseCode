#include "AppCommon.h"
#include <stdlib.h>
#include "Math\bit_operation.h"

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

#define MAX_STRING_LENGTH   32
static char string[MAX_STRING_LENGTH];
static char error_buf[] = "length error";

char *ValueToBinaryString(X_Boolean isAlign,uint8_t string_length,uint32_t value)
{
	uint8_t bitnumber,i;
	char string_backup[MAX_STRING_LENGTH];
	// clear buf ?
	if(string_length > MAX_STRING_LENGTH) {return error_buf;}//
	itoa(value, string, 2);

	if(isAlign == X_True)
	{
		bitnumber = GetBinaryBitNumber(value);
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
