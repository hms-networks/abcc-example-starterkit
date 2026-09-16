/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
**
** ADI example "D_alltypes"
** 
** File Description:
** Example of an ADI setup with 20 ADIs demonstrating all supported data types,
** including structured data types.
**
** Ensure the following definitions, if defined in abcc_driver_config.h,
** are set to:
**    ABCC_CFG_STRUCT_DATA_TYPE_ENABLED      1
**    ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED  0
********************************************************************************
*/

#include "abcc_api.h"

#if ( !ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
#error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to FALSE and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to TRUE in order to run this example
#endif


/*------------------------------------------------------------------------------
** Data holders for the ADI instances.
**------------------------------------------------------------------------------
*/
typedef struct APPL_AdiType
{
   UINT32  lUint32;
   INT32   lInt32;
   UINT16  iUint16;
   INT16   iInt16;
   UINT16  iBit16;
   UINT8   bUint8;
   INT8    bInt8;
   UINT8   bBit8;
   UINT8   bBitTypes[ 4 ];
   UINT16  iBitTypes[ 2 ];
}
APPL_AdiType;

UINT32   appl_lUint32;
INT32    appl_lInt32;
UINT16   appl_iUint16;
INT16    appl_iInt16;
UINT16   appl_iBit16;
UINT8    appl_bUint8;
INT8     appl_bInt8;
UINT8    appl_bBit8;
UINT8    bBitTypes[4];
UINT16   iBitTypes[2];

APPL_AdiType APPL_StructAdi;


/*------------------------------------------------------------------------------
** Structured ADI#38.
**------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_AdiReadStruct[] =
{
 /* Index: 0 */  { "ABP_UINT32", ABP_UINT32, 1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.lUint32,      NULL } } },
 /* Index: 1 */  { "ABP_SINT32", ABP_SINT32, 1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.lInt32,       NULL } } },
 /* Index: 2 */  { "ABP_UINT16", ABP_UINT16, 1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.iUint16,      NULL } } },
 /* Index: 3 */  { "ABP_SINT16", ABP_SINT16, 1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.iInt16,       NULL } } },
 /* Index: 4 */  { "ABP_BITS16", ABP_BITS16, 1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.iBit16,       NULL } } },
 /* Index: 5 */  { "ABP_UINT8",  ABP_UINT8,  1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.bUint8,       NULL } } },
 /* Index: 6 */  { "ABP_SINT8",  ABP_SINT8,  1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.bInt8,        NULL } } },
 /* Index: 7 */  { "ABP_BITS8",  ABP_BITS8,  1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.bBit8,        NULL } } },
 /* Index: 8 */  { "ABP_PAD8",   ABP_PAD8,   1, AD_ADI_DESC__R_SG,  0,  { { NULL,                         NULL } } },
 #ifdef ABCC_SYS_16_BIT_CHAR
 /* Index: 9 */  { "ABP_BIT1",   ABP_BIT1,   1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, AD_ADI_DESC__R_SG,  1,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, AD_ADI_DESC__R_SG,  3,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, AD_ADI_DESC__R_SG,  6,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, AD_ADI_DESC__R_SG,  10, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, AD_ADI_DESC__R_SG,  15, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, AD_ADI_DESC__R_SG,  5,  { { &APPL_StructAdi.iBitTypes[1], NULL } } },
 /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, AD_ADI_DESC__R_SG,  4,  { { NULL,                         NULL } } }
 #else
 /* Index: 9 */  { "ABP_BIT1",   ABP_BIT1,   1, AD_ADI_DESC__R_SG,  0,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, AD_ADI_DESC__R_SG,  1,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, AD_ADI_DESC__R_SG,  3,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, AD_ADI_DESC__R_SG,  6,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, AD_ADI_DESC__R_SG,  2,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
 /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, AD_ADI_DESC__R_SG,  7,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
 /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, AD_ADI_DESC__R_SG,  5,  { { &APPL_StructAdi.bBitTypes[2], NULL } } },
 /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, AD_ADI_DESC__R_SG,  4,  { { NULL,                         NULL } } }
 #endif
};

/*------------------------------------------------------------------------------
** Structured ADI#39.
**------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_AdiWriteStruct[] =
{
 /* Index: 0 */  { "ABP_UINT32", ABP_UINT32, 1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.lUint32,      NULL } } },
 /* Index: 1 */  { "ABP_SINT32", ABP_SINT32, 1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.lInt32,       NULL } } },
 /* Index: 2 */  { "ABP_UINT16", ABP_UINT16, 1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.iUint16,      NULL } } },
 /* Index: 3 */  { "ABP_SINT16", ABP_SINT16, 1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.iInt16,       NULL } } },
 /* Index: 4 */  { "ABP_BITS16", ABP_BITS16, 1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.iBit16,       NULL } } },
 /* Index: 5 */  { "ABP_UINT8",  ABP_UINT8,  1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.bUint8,       NULL } } },
 /* Index: 6 */  { "ABP_SINT8",  ABP_SINT8,  1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.bInt8,        NULL } } },
 /* Index: 7 */  { "ABP_BITS8",  ABP_BITS8,  1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.bBit8,        NULL } } },
 /* Index: 8 */  { "ABP_PAD8",   ABP_PAD8,   1, AD_ADI_DESC___W_G, 0,  { { NULL,                         NULL } } },
 #ifdef ABCC_SYS_16_BIT_CHAR
 /* Index: 9 */  { "ABP_BIT1",   ABP_BIT1,   1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, AD_ADI_DESC___W_G, 1,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, AD_ADI_DESC___W_G, 3,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, AD_ADI_DESC___W_G, 6,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, AD_ADI_DESC___W_G, 10, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, AD_ADI_DESC___W_G, 15, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
 /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, AD_ADI_DESC___W_G, 5,  { { &APPL_StructAdi.iBitTypes[1], NULL } } },
 /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, AD_ADI_DESC___W_G, 4,  { { NULL,                         NULL } } }
 #else
 /* Index: 9 */  { "ABP_BIT1",   ABP_BIT1,   1, AD_ADI_DESC___W_G, 0,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, AD_ADI_DESC___W_G, 1,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, AD_ADI_DESC___W_G, 3,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, AD_ADI_DESC___W_G, 6,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
 /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, AD_ADI_DESC___W_G, 2,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
 /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, AD_ADI_DESC___W_G, 7,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
 /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, AD_ADI_DESC___W_G, 5,  { { &APPL_StructAdi.bBitTypes[2], NULL } } },
 /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, AD_ADI_DESC___W_G, 4,  { { NULL,                         NULL } } }
 #endif
};


/*------------------------------------------------------------------------------
** ADI table mixing normal types and structured data types.
**------------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------------------------------
** iInstance | pabName | bDataType | bNumOfElements | bDesc | pxValuePtr | pxValuePropPtr | psStruct
**---------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   { 20, "ABP_UINT32_READ",  ABP_UINT32, 1,  AD_ADI_DESC__R_SG, { { &appl_lUint32, NULL } }, NULL },
   { 21, "ABP_UINT32_WRITE", ABP_UINT32, 1,  AD_ADI_DESC___W_G, { { &appl_lUint32, NULL } }, NULL },
   { 22, "ABP_SINT32_READ",  ABP_SINT32, 1,  AD_ADI_DESC__R_SG, { { &appl_lInt32,  NULL } }, NULL },
   { 23, "ABP_SINT32_WRITE", ABP_SINT32, 1,  AD_ADI_DESC___W_G, { { &appl_lInt32,  NULL } }, NULL },
   { 24, "ABP_UINT16_READ",  ABP_UINT16, 1,  AD_ADI_DESC__R_SG, { { &appl_iUint16, NULL } }, NULL },
   { 25, "ABP_UINT16_WRITE", ABP_UINT16, 1,  AD_ADI_DESC___W_G, { { &appl_iUint16, NULL } }, NULL },
   { 26, "ABP_SINT16_READ",  ABP_SINT16, 1,  AD_ADI_DESC__R_SG, { { &appl_iInt16,  NULL } }, NULL },
   { 27, "ABP_SINT16_WRITE", ABP_SINT16, 1,  AD_ADI_DESC___W_G, { { &appl_iInt16,  NULL } }, NULL },
   { 28, "ABP_BITS16_READ",  ABP_BITS16, 1,  AD_ADI_DESC__R_SG, { { &appl_iBit16,  NULL } }, NULL },
   { 29, "ABP_BITS16_WRITE", ABP_BITS16, 1,  AD_ADI_DESC___W_G, { { &appl_iBit16,  NULL } }, NULL },
   { 30, "ABP_UINT8_READ",   ABP_UINT8,  1,  AD_ADI_DESC__R_SG, { { &appl_bUint8,  NULL } }, NULL },
   { 31, "ABP_UINT8_WRITE",  ABP_UINT8,  1,  AD_ADI_DESC___W_G, { { &appl_bUint8,  NULL } }, NULL },
   { 32, "ABP_SINT8_READ",   ABP_SINT8,  1,  AD_ADI_DESC__R_SG, { { &appl_bInt8,   NULL } }, NULL },
   { 33, "ABP_SINT8_WRITE",  ABP_SINT8,  1,  AD_ADI_DESC___W_G, { { &appl_bInt8,   NULL } }, NULL },
   { 34, "ABP_PAD8_READ",    ABP_PAD8,   1,  AD_ADI_DESC__R_SG, { { NULL,          NULL } }, NULL },
   { 35, "ABP_PAD8_WRITE",   ABP_PAD8,   1,  AD_ADI_DESC___W_G, { { NULL,          NULL } }, NULL },
   { 36, "ABP_BIT7_READ",    ABP_BIT7,   1,  AD_ADI_DESC__R_SG, { { &appl_bBit8,   NULL } }, NULL },
   { 37, "ABP_BIT7_WRITE",   ABP_BIT7,   1,  AD_ADI_DESC___W_G, { { &appl_bBit8,   NULL } }, NULL },
   { 38, "Struct_READ",      ABP_UINT8,  17, AD_ADI_DESC__R_SG, { { NULL,          NULL } }, appl_AdiReadStruct  },
   { 39, "Struct_WRITE",     ABP_UINT8,  17, AD_ADI_DESC___W_G, { { NULL,          NULL } }, appl_AdiWriteStruct }
};


/*------------------------------------------------------------------------------
** Map all ADIs in both directions. Both whole ADIs and individual elements
** are mapped.
** -----------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
   { 20,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 21,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 22,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 23,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 24,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 25,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 26,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 27,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 28,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 29,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 30,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 31,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 32,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 33,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 34,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 35,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 36,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { AD_MAP_PAD_ADI, PD_READ,  1,               0  },
   { 37,             PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { AD_MAP_PAD_ADI, PD_WRITE, 1,               0  },
   { 38,             PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 39,             PD_WRITE, 1,               0  },
   { 39,             PD_WRITE, 1,               1  },
   { 39,             PD_WRITE, 1,               2  },
   { 39,             PD_WRITE, 1,               3  },
   { 39,             PD_WRITE, 1,               4  },
   { 39,             PD_WRITE, 1,               5  },
   { 39,             PD_WRITE, 1,               6  },
   { 39,             PD_WRITE, 1,               7  },
   { 39,             PD_WRITE, 1,               8  },
   { 39,             PD_WRITE, 1,               9  },
   { 39,             PD_WRITE, 1,               10 },
   { 39,             PD_WRITE, 1,               11 },
   { 39,             PD_WRITE, 1,               12 },
   { 39,             PD_WRITE, 1,               13 },
   { 39,             PD_WRITE, 1,               14 },
   { 39,             PD_WRITE, 1,               15 },
   { 39,             PD_WRITE, 1,               16 },
   { AD_MAP_END_ENTRY }
};


UINT16 ABCC_API_CbfGetNumAdi( void )
{
    return( sizeof( ABCC_API_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}


/*------------------------------------------------------------------------------
** This function is called when read and write data have been updated.
** It could for example be used for operations on the ADI data.
** Not used in this example.
**------------------------------------------------------------------------------
*/
void ABCC_API_CbfCyclicalProcessing( void )
{
    if ( ABCC_API_AnbState() == ABP_ANB_STATE_PROCESS_ACTIVE )
    {
    }
    else
    {
        /*
        ** We are not in PROCESS_ACTIVE; no cyclical processing
        ** is performed in this example.
        */
    }	
}
