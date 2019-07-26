#ifndef __DATAFLOW_H
#define __DATAFLOW_H

#include "..\..\..\CommonSource\KeilMDK.h"

typedef enum
{
	CommandGetEntry,
	CommandSetEntry,
	CharKeyEntry,
	CharAirMouseEntry,
	CharPenTipEntry,
	CharColorEntry,
	CharPenTypeEntry,
	CharBatteryEntry,
}DataFlowEntry;

X_Boolean DataFlowPush(DataFlowEntry entry,const uint8_t * p_data,uint8_t length);
X_Boolean DataFlowPop(DataFlowEntry entry,uint8_t ** p_data,uint8_t* length);
X_Void DataFlowInit(X_Void);

#endif

