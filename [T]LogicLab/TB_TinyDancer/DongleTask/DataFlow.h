#ifndef __DATAFLOW_H
#define __DATAFLOW_H

#include "..\..\..\CommonSource\KeilMDK.h"

X_Boolean AirMouseNotityDataPush(const uint8_t * p_data,uint8_t length);
X_Boolean AirMouseNotityDataPop(uint8_t ** p_data,uint8_t* length);
X_Void DataFlowInit(X_Void);

#endif

