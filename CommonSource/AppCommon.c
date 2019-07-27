#include "AppCommon.h"

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

