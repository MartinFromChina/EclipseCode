#ifndef __APPCOMMON_H
#define __APPCOMMON_H

#include "KeilMDK.h"

#ifndef CopyBuffer
static __inline X_Void __CopyBuffer(X_Void const* src, X_Void* dst, X_UInt length)
{
    X_UInt8 const* _src;
    X_UInt8* _dst;

    _src = (X_UInt8 const*)src;
    _dst = (X_UInt8*)dst;

    while (length != 0)
    {
        *_dst = *_src;
        ++_dst;
        ++_src;
        --length;
    }
}
#define CopyBuffer  __CopyBuffer
#endif
X_Void byteBufInit(uint8_t *p_buf,uint16_t length,uint8_t init_value);
X_Void twobyteBufInit(uint16_t *p_buf,uint16_t length,uint16_t init_value);
char *ValueToBinaryString(X_Boolean isAlign,uint8_t string_length,uint32_t value);

#endif
