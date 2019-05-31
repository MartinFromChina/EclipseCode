#ifndef KEILMDK_H
#define KEILMDK_H

#include <stdint.h>
#include <wchar.h>

#define X_Void      void
typedef int8_t      X_Int8;
typedef uint8_t     X_UInt8;
typedef int16_t     X_Int16;
typedef uint16_t    X_UInt16;
typedef int32_t     X_Int32;
typedef uint32_t    X_UInt32;
typedef int64_t     X_Int64;
typedef uint64_t    X_UInt64;
typedef float       X_Float32;
typedef double      X_Float64;
typedef char        X_Char;
typedef wchar_t     X_WChar;

typedef unsigned int    X_UInt;
typedef int             X_Int;
typedef X_Int64         X_IntLongest;
typedef X_UInt64        X_UIntLongest;

#define X_Null              ((X_Void*)0)

typedef int                 X_Boolean;
#define X_True              (0 == 0)
#define X_False             (0 != 0)

#define MapperAssertion     ((sizeof(X_Int8) == 1) && (sizeof(X_UInt8) == 1) && (sizeof(X_Int16) == 2) && (sizeof(X_UInt16) == 2) && (sizeof(X_Int32) == 4) && (sizeof(X_UInt32) == 4) && (sizeof(X_Float32) == 4) && (sizeof(X_Float64) == 8))

#endif
