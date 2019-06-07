#ifndef __LCM_GCD_H
#define __LCM_GCD_H

#include "..\KeilMDK.h"

X_Boolean GetLCM(uint32_t *p_buf,uint32_t length,uint32_t limit,uint32_t * result);

/*
 * test the module use code below :
 *
 *  uint32_t result;
	uint32_t buf[7] = {49,1,3,100,28,76,5};
	if(GetLCM(buf,sizeof(buf)/sizeof(buf[0]),5000000,&result) == X_True )
	{
		printf(" LCM IS  %d\r\n",result);
	}
	else
	{
		printf("get LCM failed\r\n");
	}
 */

#endif
