/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Example of an ADI setup with 11 ADIs covering all data types and structures.
**
** In abcc_drv_cfg.h make sure that the following definitions are set to:
** ABCC_CFG_STRUCT_DATA_TYPE     ( TRUE )
** ABCC_CFG_ADI_GET_SET_CALLBACK ( FALSE )
********************************************************************************
********************************************************************************
*/

#include "abcc_api.h"


#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_ALL_TYPES )

#if ( !ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
#error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to FALSE and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to TRUE in order to run this example
#endif


/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Access descriptor for the ADIs
**------------------------------------------------------------------------------
*/
#define APPL_READ_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_READ_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_SET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_WRITE_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

#define APPL_NOT_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS )

#define APPL_NOT_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_SET_ACCESS )

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ADI variables
**------------------------------------------------------------------------------
*/
typedef struct APPL_Adi
{
    UINT32  lUint32;
    INT32   lInt32;
    UINT16  iUint16;
    INT16   iInt16;
    UINT16  iBit16;
    UINT8   bUint8;
    INT8    bInt8;
    UINT8   bBit8;
    UINT8   bBitTypes[4];
    UINT16  iBitTypes[2];
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

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structured ADI #38
**------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_AdiReadStruct[] =
{
    /* Index: 0 */  { "ABP_UINT32", ABP_UINT32, 1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.lUint32,        NULL } } },
    /* Index: 1 */  { "ABP_SINT32", ABP_SINT32, 1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.lInt32,         NULL } } },
    /* Index: 2 */  { "ABP_UINT16", ABP_UINT16, 1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.iUint16,        NULL } } },
    /* Index: 3 */  { "ABP_SINT16", ABP_SINT16, 1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.iInt16,         NULL } } },
    /* Index: 4 */  { "ABP_BITS16", ABP_BITS16, 1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.iBit16,         NULL } } },
    /* Index: 5 */  { "ABP_UINT8",  ABP_UINT8,  1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.bUint8,         NULL } } },
    /* Index: 6 */  { "ABP_SINT8",  ABP_SINT8,  1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.bInt8,          NULL } } },
    /* Index: 7 */  { "ABP_BITS8",  ABP_BITS8,  1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.bBit8,          NULL } } },
    /* Index: 8 */  { "ABP_PAD8",   ABP_PAD8,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { NULL,                           NULL } } },
 #ifdef ABCC_SYS_16_BIT_CHAR
    /* Index: 9 */  { "ABP_BIT1",   ABP_BIT1,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  1,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  3,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  6,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  10, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  15, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  5,  { { &APPL_StructAdi.iBitTypes[1], NULL } } },
    /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  4,  { { NULL,                           NULL } } }
 #else
    /* Index: 9 */ { "ABP_BIT1",   ABP_BIT1,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  0,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  1,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  3,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  6,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  2,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
    /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  7,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
    /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  5,  { { &APPL_StructAdi.bBitTypes[2], NULL } } },
    /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  4,  { { NULL,                           NULL } } }
 #endif
};

/*------------------------------------------------------------------------------
** Structured ADI #39
**------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_AdiWriteStruct[] =
{
    /* Index: 0 */  { "ABP_UINT32", ABP_UINT32, 1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.lUint32,        NULL } } },
    /* Index: 1 */  { "ABP_SINT32", ABP_SINT32, 1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.lInt32,         NULL } } },
    /* Index: 2 */  { "ABP_UINT16", ABP_UINT16, 1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.iUint16,        NULL } } },
    /* Index: 3 */  { "ABP_SINT16", ABP_SINT16, 1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.iInt16,         NULL } } },
    /* Index: 4 */  { "ABP_BITS16", ABP_BITS16, 1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.iBit16,         NULL } } },
    /* Index: 5 */  { "ABP_UINT8",  ABP_UINT8,  1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.bInt8,          NULL } } },
    /* Index: 6 */  { "ABP_SINT8",  ABP_SINT8,  1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.bUint8,         NULL } } },
    /* Index: 7 */  { "ABP_BITS8",  ABP_BITS8,  1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.bBit8,          NULL } } },
    /* Index: 8 */  { "ABP_PAD8",   ABP_PAD8,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { NULL,                           NULL } } },
 #ifdef ABCC_SYS_16_BIT_CHAR
    /* Index: 9 */  { "ABP_BIT1",   ABP_BIT1,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 1,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 3,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 6,  { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 10, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 15, { { &APPL_StructAdi.iBitTypes[0], NULL } } },
    /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 5,  { { &APPL_StructAdi.iBitTypes[1], NULL } } },
    /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 4,  { { NULL,                           NULL } } }
 #else
    /* Index: 9 */ { "ABP_BIT1",   ABP_BIT1,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 0,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 10 */ { "ABP_BIT2",   ABP_BIT2,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 1,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 11 */ { "ABP_BIT3",   ABP_BIT3,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 3,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 12 */ { "ABP_BIT4",   ABP_BIT4,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 6,  { { &APPL_StructAdi.bBitTypes[0], NULL } } },
    /* Index: 13 */ { "ABP_BIT5",   ABP_BIT5,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 2,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
    /* Index: 14 */ { "ABP_BIT6",   ABP_BIT6,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 7,  { { &APPL_StructAdi.bBitTypes[1], NULL } } },
    /* Index: 15 */ { "ABP_BIT7",   ABP_BIT7,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 5,  { { &APPL_StructAdi.bBitTypes[2], NULL } } },
    /* Index: 16 */ { "ABP_PAD4",   ABP_PAD4,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, 4,  { { NULL,                           NULL } } }
 #endif
};

/*******************************************************************************
** Public Globals
********************************************************************************
*/

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
   { 20, "ABP_UINT32_READ",  ABP_UINT32, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_lUint32, NULL } }, NULL },
   { 21, "ABP_UINT32_WRITE", ABP_UINT32, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_lUint32, NULL } }, NULL },
   { 22, "ABP_SINT32_READ",  ABP_SINT32, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_lInt32,  NULL } }, NULL },
   { 23, "ABP_SINT32_WRITE", ABP_SINT32, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_lInt32,  NULL } }, NULL },
   { 24, "ABP_UINT16_READ",  ABP_UINT16, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_iUint16, NULL } }, NULL },
   { 25, "ABP_UINT16_WRITE", ABP_UINT16, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_iUint16, NULL } }, NULL },
   { 26, "ABP_SINT16_READ",  ABP_SINT16, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_iInt16,  NULL } }, NULL },
   { 27, "ABP_SINT16_WRITE", ABP_SINT16, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_iInt16,  NULL } }, NULL },
   { 28, "ABP_BITS16_READ",  ABP_BITS16, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_iBit16,  NULL } }, NULL },
   { 29, "ABP_BITS16_WRITE", ABP_BITS16, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_iBit16,  NULL } }, NULL },
   { 30, "ABP_UINT8_READ",   ABP_UINT8,  1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_bUint8,  NULL } }, NULL },
   { 31, "ABP_UINT8_WRITE",  ABP_UINT8,  1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_bUint8,  NULL } }, NULL },
   { 32, "ABP_SINT8_READ",   ABP_SINT8,  1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_bInt8,   NULL } }, NULL },
   { 33, "ABP_SINT8_WRITE",  ABP_SINT8,  1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_bInt8,   NULL } }, NULL },
   { 34, "ABP_PAD8_READ",    ABP_PAD8,   1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { NULL,          NULL } }, NULL },
   { 35, "ABP_PAD8_WRITE",   ABP_PAD8,   1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { NULL,          NULL } }, NULL },
   { 36, "ABP_BIT7_READ",    ABP_BIT7,   1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_bBit8,   NULL } }, NULL },
   { 37, "ABP_BIT7_WRITE",   ABP_BIT7,   1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_bBit8,   NULL } }, NULL },
   { 38, "Struct_READ",      ABP_UINT8,  17, APPL_READ_MAP_WRITE_ACCESS_DESC,  { { NULL,          NULL } }, appl_AdiReadStruct },
   { 39, "Struct_WRITE",     ABP_UINT8,  17, APPL_WRITE_MAP_READ_ACCESS_DESC, { { NULL,          NULL } }, appl_AdiWriteStruct }
};

/*------------------------------------------------------------------------------
** Map all adi:s in both directions. Both whole ADI:s and individual elements
** are mapped.
** -----------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
   { 20, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 21, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 22, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 23, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 24, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 25, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 26, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 27, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 28, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 29, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 30, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 31, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 32, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 33, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 34, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 35, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { 36, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { AD_MAP_PAD_ADI, PD_READ,  1,           0  },
   { 37, PD_WRITE, AD_MAP_ALL_ELEM, 0  },
   { AD_MAP_PAD_ADI, PD_WRITE, 1,           0  },
   { 38, PD_READ,  AD_MAP_ALL_ELEM, 0  },
   { 39, PD_WRITE, 1,                       0  },
   { 39, PD_WRITE, 1,                       1  },
   { 39, PD_WRITE, 1,                       2  },
   { 39, PD_WRITE, 1,                       3  },
   { 39, PD_WRITE, 1,                       4  },
   { 39, PD_WRITE, 1,                       5  },
   { 39, PD_WRITE, 1,                       6  },
   { 39, PD_WRITE, 1,                       7  },
   { 39, PD_WRITE, 1,                       8  },
   { 39, PD_WRITE, 1,                       9  },
   { 39, PD_WRITE, 1,                       10 },
   { 39, PD_WRITE, 1,                       11 },
   { 39, PD_WRITE, 1,                       12 },
   { 39, PD_WRITE, 1,                       13 },
   { 39, PD_WRITE, 1,                       14 },
   { 39, PD_WRITE, 1,                       15 },
   { 39, PD_WRITE, 1,                       16 },
   { AD_MAP_END_ENTRY }
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/
UINT16 ABCC_API_CbfGetNumAdi(void)
{
    return(sizeof(ABCC_API_asAdiEntryList) / sizeof(AD_AdiEntryType));
}




void ABCC_API_CbfCyclicalProcessing()
{
    /*
    ** This function is called when read and write data have been updated. It
    ** could for example be used for operations on the ADI data.
    ** Not used in this example.
    */
}

/*******************************************************************************
** Tasks
********************************************************************************
*/

#endif



