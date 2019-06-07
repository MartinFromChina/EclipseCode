#include "lcm_gcd.h"

static X_Boolean GetLcmBetweenTwoNumber(uint32_t m,uint32_t n,uint32_t limit,uint32_t * result)
{
	uint32_t temp;
	X_Boolean isOK;

	isOK = X_False;
	if(m == 0 || n == 0) {return isOK;}
	if(result == X_Null) {return isOK;}

	if(m < n)
	{
		temp = n;
	}
	else
	{
		temp = m;
	}

	for(temp = m ; temp < limit ; temp ++)
	{

		if(temp % m == 0 && temp % n == 0)
		{
			isOK = X_True;
			* result = temp;
			break;
		}
	}
	if(temp >=  limit && isOK == X_False)
	{
		* result = 0;
	}
	return isOK;
}

X_Boolean GetLCM(uint32_t *p_buf,uint32_t length,uint32_t limit,uint32_t * result)
{
	X_Boolean isOK;
	uint32_t i,Cur_Result,Pre_Result;

	if(result == X_Null) {return X_False;}
	Pre_Result = p_buf[0];

	for(i=1;i<length;i++)
	{
		isOK = GetLcmBetweenTwoNumber(Pre_Result,p_buf[i],limit,&Cur_Result);
		if(isOK == X_False)
		{
			* result = 0;
			break;
		}
		else
		{
			Pre_Result = Cur_Result;
		}

	}
	* result = Cur_Result;
	return isOK;
}
