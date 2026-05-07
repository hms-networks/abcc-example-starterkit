/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Definition of ABCC types
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Data type mnemonics:
**
** The variable names throughout this code use the following mnemonics
** to specify the data type of the variable:
**
**    a     Array
**    b     8-bit integer
**    c     Character
**    d     Double-precision float
**    e     Enumeration
**    f     Boolean (flag)
**    i     16-bit integer
**    l     32-bit integer
**    ll    64-bit integer
**    p     Pointer
**    r     Single-precision float
**    s     Structure
**    u     Union
**    n     Function
**    x     Not known / not relevant
**------------------------------------------------------------------------------
*/

#ifndef ABCC_TD_H
#define ABCC_TD_H

#include <stdint.h>

/*---------------------------------------------------------------------------
**
** BOOL8
**
** INT8
** INT16
** INT32
**
** UINT8
** UINT16
** UINT32
**
** FLOAT32
**
** The standard boolean data type, 8-bit.
** The standard signed 8, 16, and 32 bit data types (respectively).
** The standard unsigned 8, 16, and 32 bit data types (respectively).
**---------------------------------------------------------------------------
*/

typedef int        BOOL;
typedef int8_t     BOOL8;
typedef uint8_t    UINT8;
typedef int8_t     INT8;
typedef uint16_t   UINT16;
typedef int16_t    INT16;
typedef uint32_t   UINT32;
typedef int32_t    INT32;
typedef uint64_t   UINT64;
typedef int64_t    INT64;
typedef float      FLOAT32;
typedef double     FLOAT64;

/*------------------------------------------------------------------------------
** LeINT16
** LeINT32
** LeUINT16
** LeUINT32
**
** Little endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     LeINT16;
typedef INT32     LeINT32;
typedef INT64     LeINT64;
typedef UINT16    LeUINT16;
typedef UINT32    LeUINT32;
typedef UINT64    LeUINT64;

/*------------------------------------------------------------------------------
** BeINT16
** BeINT32
** BeUINT16
** BeUINT32
**
** Big endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     BeINT16;
typedef INT32     BeINT32;
typedef INT64     BeINT64;
typedef UINT16    BeUINT16;
typedef UINT32    BeUINT32;
typedef UINT64    BeUINT64;

/*---------------------------------------------------------------------------
**
** FALSE
** TRUE
**
** These are the symbolic constants for true and false used in boolean
** data type comparisons.
**
**---------------------------------------------------------------------------
*/
#ifndef FALSE
   #define FALSE     0
#endif

#ifndef TRUE
   #define TRUE      ( !FALSE )
#endif

/*---------------------------------------------------------------------------
**
** NULL
**
** Default value for invalid pointers.
**
**---------------------------------------------------------------------------
*/
#ifndef NULL
#define NULL 0
#endif

#ifdef __cplusplus
   #define CPLUSPLUS
#endif

#ifdef CPLUSPLUS
   #define EXTFUNC extern "C"
   #define EXTVAR extern "C"
#else
   #define EXTFUNC extern
   #define EXTVAR extern
#endif

/*---------------------------------------------------------------------------
** Compiler/platform-specific packing macros.
**---------------------------------------------------------------------------
*/
/*
** https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
** https://docs.microsoft.com/en-us/cpp/preprocessor/pack
*/
#define ABCC_SYS_PACK_ON   __pragma(pack(push,ABCC_BYTEALIGN,1))
#define ABCC_SYS_PACK_OFF  __pragma(pack(pop,ABCC_BYTEALIGN))
#define PACKED_STRUCT

#endif  /* inclusion lock */

