/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Example of an ADI setup with two simple UINT16 ADIs representing speed and
** reference speed of a motor. Both are mapped as cyclical process data
** parameters.
**
** Make sure that the following definitions, if they exist in
** abcc_driver_config.h, are set to the following:
**    ABCC_CFG_STRUCT_DATA_TYPE_ENABLED     0
**    ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED 0
********************************************************************************
*/

#include "abcc_api.h"


#ifndef ABCC_CFG_DRV_STRUCTURED_ADIS
/******************************************************************************************************/
#if (  ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
#error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to 0 and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to 0 in order to run this example
#endif

/*------------------------------------------------------------------------------
** Data holder for the network data parameters (ADI)
**------------------------------------------------------------------------------
*/
uint16_t appl_iSpeed;
uint16_t appl_iRefSpeed;

/*------------------------------------------------------------------------------
** Min, max and default value for appl_aiUint16
**------------------------------------------------------------------------------
*/
static AD_UINT16Type appl_sUint16Prop = { { 0, 0xFFFF, 0 } };

/*-------------------------------------------------------------------------------------------------------------
** 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6. pxValuePtr | 7. pxValuePropPtr
**--------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   {  0x1,  "SPEED",     ABP_UINT16,   1, AD_ADI_DESC___W_G, { { &appl_iSpeed,    &appl_sUint16Prop } } },
   {  0x2,  "REF_SPEED", ABP_UINT16,   1, AD_ADI_DESC__R_S_, { { &appl_iRefSpeed, &appl_sUint16Prop } } }
};


/*------------------------------------------------------------------------------
** Example - electric motor control loop
**------------------------------------------------------------------------------
*/
void ABCC_API_CbfCyclicalProcessing()
{
    if (ABCC_API_AnbState() == ABP_ANB_STATE_PROCESS_ACTIVE)
    {
        /*
        ** An example of ADI data handling.
        */
        if (appl_iSpeed > appl_iRefSpeed)
        {
            /*
            ** Do something that lowers speed.
            */
            appl_iSpeed -= 1;
        }
        else if (appl_iSpeed < appl_iRefSpeed)
        {
            /*
            ** Do something that increases speed.
            */
            appl_iSpeed += 1;
        }
    }
    else
    {
        /*
        ** We are not in process active, the default should be that the motor
        ** should not run.
        */
        appl_iSpeed = 0;
    }
}
/*------------------------------------------------------------------------------
** Map all adi:s in both directions
**------------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { 2, PD_READ,  AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

/******************************************************************************************************/


#else
/******************************************************************************************************/
/*
** Example with all access flags set.
*/
#define ADI_DESCR_ALL_ACCESS  ( ABP_APPD_DESCR_GET_ACCESS | ABP_APPD_DESCR_SET_ACCESS | ABP_APPD_DESCR_MAPPABLE_WRITE_PD | ABP_APPD_DESCR_MAPPABLE_READ_PD )


/*******************************************************************************
** Example of ADI mapping structured data type
** (ABCC_CFG_STRUCT_DATA_TYPE_ENABLED defined to 1 in abcc_driver_config.h )
********************************************************************************
*/


/*------------------------------------------------------------------------------
** Variable place holders for the ADI:s
**------------------------------------------------------------------------------
*/
/*
** The content of struct1 is described by  ABCC_API_AdiStruct1 below
*/
struct
{
    UINT16   iData;
    UINT32   lData;
} struct1;

/*
** The content of struct2 is described by  ABCC_API_AdiStruct2 below
*/
struct
{
    UINT8    bBitData[2];
    UINT16   iData;
} struct2;

/*
** Basic 8 bit ADI
*/
UINT8    bData;


/*------------------------------------------------------------------------------
** ABCC_API_AdiStruct1 describes a structured ADI with two members. ( struct1 )
**------------------------------------------------------------------------------
*/
static const AD_StructDataType ABCC_API_AdiStruct1[] =
{
    /*
    ** --------------------------------------------------------------------------------------------------------------------
    ** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
    ** --------------------------------------------------------------------------------------------------------------------
    */
    /* Index: 0 */ {  "Element01",  ABP_UINT16,  1,  ADI_DESCR_ALL_ACCESS,  0,  { {  &struct1.iData,  NULL  } } },
    /* Index: 1 */ {  "Element02",  ABP_SINT32,  1,  ADI_DESCR_ALL_ACCESS,  0,  { {  &struct1.lData,  NULL  } } }
};


/*
**------------------------------------------------------------------------------
**   ABCC_API_AdiStruct2 describes a structured ADI with 4 members (struct2). The
**   structure includes bit data types and padding. The picture below shows the
**   packed ADI result. The numbers are referring to the index in
**   ABCC_API_AdiStruct2.
**
**
**   Bit:     7   6   5   4   3   2   1   0
**          ---------------------------------
**          | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |   struct2.bBitData[ 0 ]
**          ---------------------------------
**          ---------------------------------
**          | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 1 |   struct2.bBitData[ 1 ]
**          ---------------------------------
**          ---------------------------------
**          | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 |   struct2.iData
**          ---------------------------------
**          ---------------------------------
**          | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 |   struct2.iData
**          ---------------------------------
**------------------------------------------------------------------------------
*/
static const AD_StructDataType ABCC_API_AdiStruct2[] =
{
    /*
    ** --------------------------------------------------------------------------------------------------------------------
    ** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
    ** --------------------------------------------------------------------------------------------------------------------
    */
    /* Index: 0 */ {  "Element01",  ABP_BIT5,    1,  ADI_DESCR_ALL_ACCESS,  0,  { {  &struct2.bBitData[0],  NULL  } } }, /* First 5 bits in struct2.bBitData[ 0 ]                                       */
    /* Index: 1 */ {  "Element02",  ABP_BIT4,    1,  ADI_DESCR_ALL_ACCESS,  5,  { {  &struct2.bBitData[0],  NULL  } } }, /* 4 bits starting at position 5 in struct2.bBitData[ 0 ]                      */
    /* Index: 2 */ {  "Element03",  ABP_PAD7,    1,  ADI_DESCR_ALL_ACCESS,  1,  { {  NULL,                  NULL  } } }, /* 7 bits padding is needed starting at position 1 in  struct2.bBitData[ 1 ]   */
    /* Index: 3 */ {  "Element04",  ABP_UINT16,  1,  AD_ADI_DESC____SG,  0,  { {  &struct2.iData,        NULL  } } }  /* 16 bit type must at least be on a 8 bit aligned position                    */
};

/*------------------------------------------------------------------------------
** ADI instance allocation with structured ADI:s. The last member
** ( 8. psStruct ) is referring the structured data types described above.
** Note that member 3,5,6 and 7 are ignored if psStruct ( 8.) is a valid
** pointer.
**------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
//const AD_AdiEntryType AD_asADIEntryList[] =
{
    /*
    ** ----------------------------------------------------------------------------------------------------------------------------
    ** | 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6.pxValuePtr | 7. pxValuePropPtr | 8. psStruct |
    ** ----------------------------------------------------------------------------------------------------------------------------
    */
    {  10,  "Struct1",   ABP_BOOL,   2,  ADI_DESCR_ALL_ACCESS,  { { NULL,    NULL } },  ABCC_API_AdiStruct1  },
    {  20,  "Struct2",   ABP_BOOL,   4,  ADI_DESCR_ALL_ACCESS,  { { NULL,    NULL } },  ABCC_API_AdiStruct2  },
    {  30,  "NoStruct",  ABP_UINT8,  1,  AD_ADI_DESC____SG,  { { &bData,  NULL } },  NULL  }
};





























const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
    /*




    {  10,              PD_READ,   AD_MAP_ALL_ELEM,          0  },  /* ADI 10  is mapped. ( All elements )                  */
    {  20,              PD_READ,   1,                        1  },  /* ADI 20  element 1 (index) is mapped ( 4 Bits )       */
    {  AD_MAP_PAD_ADI,  PD_READ,   4,                        0  },  /* PAD process data with 4 bits for correct alignment   */
    {  30,              PD_READ,   1,                        0  },  /* Map ADI 30                                           */
    {  20,              PD_WRITE,  2,                        0  },  /* ADI 20 element 0 and 1 is mapped ( 9 bits )          */
    {  AD_MAP_PAD_ADI,  PD_WRITE,  7,                        0  },  /* PAD process data with 7 bits for correct alignment   */
    {  AD_MAP_END_ENTRY}
};






void ABCC_API_CbfCyclicalProcessing()
{
}

#endif 


UINT16 ABCC_API_CbfGetNumAdi(void)
{
    return(sizeof(ABCC_API_asAdiEntryList) / sizeof(AD_AdiEntryType));
}
