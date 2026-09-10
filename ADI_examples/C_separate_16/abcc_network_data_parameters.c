/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Example of an ADI setup with an array of 16 bit values;
** there are 2 separate arrays of output and input data which are not handled 
** in this example 
**
** Make sure that the following definitions, if they exist in
** abcc_driver_config.h, are set to the following:
**    ABCC_CFG_STRUCT_DATA_TYPE_ENABLED     0
**    ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED 1 (callback functions are used in this example)
********************************************************************************
*/
#include "abcc_api.h"

#if (  ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || !ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
#error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to 1 and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to 0 in order to run this example
#endif


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

/*******************************************************************************
** Private Globals
********************************************************************************
*/
/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/
static void SetAdi10Value(const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex);
static void GetAdi11Value(const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex);

/*------------------------------------------------------------------------------
** Data holder for the ADI instances
**------------------------------------------------------------------------------
*/
static UINT16  appl_aiUint16_10[32];
static UINT16  appl_aiUint16_11[32];
static UINT16  appl_Uint16_12 = 0;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** 32 16-bit values individually
** See abcc_ad_if.h for a more detailed description.
**------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------------------------------------------------
** iInstance | pabName | bDataType | bNumOfElements | bDesc | pxValuePtr | pxValuePropPtr| pnGetAdiValue | pnSetAdiValue
**-----------------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   { 10,   "ABP_UINT16_SET",        ABP_UINT16,   32, APPL_READ_MAP_WRITE_ACCESS_DESC, { { appl_aiUint16_10, NULL } }, NULL, SetAdi10Value },
   { 11,   "ABP_UINT16_GET",        ABP_UINT16,   32, APPL_WRITE_MAP_READ_ACCESS_DESC, { { appl_aiUint16_11, NULL } }, GetAdi11Value, NULL },
   { 12,   "ABP_UINT16_COUNTER",    ABP_UINT16,   1,  APPL_NOT_MAP_WRITE_ACCESS_DESC,  { { &appl_Uint16_12, NULL  } }, NULL, NULL          },
};

/*
**------------------------------------------------------------------------------
** Map all adi:s in both directions
** See abcc_ad_if.h for a more detailed description.
**------------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
   { 10,  PD_READ,   AD_MAP_ALL_ELEM, 0 },
   { 11,  PD_WRITE,  AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};
/*******************************************************************************
** Private Services
********************************************************************************
*/
/*------------------------------------------------------------------------------
** Callback of type ABCC_GetAdiValueFuncType. The function will be called when
** the network reads ADI #11. It will increment the value of ADI#12 every time
** this is done.
**
** ABCC_GetAdiValueFuncType is declared in abcc_ad_if.h
**------------------------------------------------------------------------------
*/
static void GetAdi11Value(const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex)
{
    appl_Uint16_12++;
}
/*------------------------------------------------------------------------------
** Callback of type ABCC_SetAdiValueFuncType. The function will be called when
** the network writes to ADI#10. It copies the changed values from ADI#10 to ADI#11.
**
** ABCC_SetAdiValueFuncType is declared in abcc_ad_if.h
**------------------------------------------------------------------------------
*/
static void SetAdi10Value(const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex)
{
    UINT8 index;
    for (index = bStartIndex; index < bStartIndex + bNumElements; index++)
    {
        appl_aiUint16_11[index] = appl_aiUint16_10[index];
    }
}


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




