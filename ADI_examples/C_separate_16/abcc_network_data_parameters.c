/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
**
** ADI example "C_separate_16"
**
** File Description:
** Example of an ADI setup with an array of 16 bit values.
**
** ADI#10: An array of 32 ABP_UINT16 (64 octets mapped to read process data)
** ADI#11: An array of 32 ABP_UINT16 (64 octets mapped to write process data)
** ADI#12: ABP_UINT16 (not mapped to process data)
**
** ADI#10 triggers a callback each time a new value is written from the network.
** The callback copies the data to ADI 11 (looped to write process data).
**
** ADI#11 triggers a callback on its own each time the value is read by the
** network. The callback also increments ADI 12.
**
** Ensure the following definitions, if defined in abcc_driver_config.h,
** are set to:
**    ABCC_CFG_STRUCT_DATA_TYPE_ENABLED      0
**    ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED  1
********************************************************************************
*/
#include "abcc_api.h"

#if ( ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || !ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
#error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to 1 and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to 0 in order to run this example
#endif


/*------------------------------------------------------------------------------
** Forward declarations.
**------------------------------------------------------------------------------
*/
static void SetAdi10Value( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex );
static void GetAdi11Value( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex );


/*------------------------------------------------------------------------------
** Data holders for the ADI instances.
**------------------------------------------------------------------------------
*/
static UINT16  appl_aiUint16_10[ 32 ];
static UINT16  appl_aiUint16_11[ 32 ];
static UINT16  appl_Uint16_12 = 0;


/*------------------------------------------------------------------------------
** ADI table.
**------------------------------------------------------------------------------
*/

/*-----------------------------------------------------------------------------------------------------------------------
** iInstance | pabName | bDataType | bNumOfElements | bDesc | pxValuePtr | pxValuePropPtr| pnGetAdiValue | pnSetAdiValue
**-----------------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   { 10, "ABP_UINT16_SET",     ABP_UINT16, 32, AD_ADI_DESC__R_SG, { { appl_aiUint16_10, NULL } }, NULL, SetAdi10Value },
   { 11, "ABP_UINT16_GET",     ABP_UINT16, 32, AD_ADI_DESC___W_G, { { appl_aiUint16_11, NULL } }, GetAdi11Value, NULL },
   { 12, "ABP_UINT16_COUNTER", ABP_UINT16, 1,  AD_ADI_DESC____SG,  { { &appl_Uint16_12, NULL  } }, NULL, NULL          },
};


/*
**------------------------------------------------------------------------------
** Map all ADIs in both directions.
** See abcc_application_data_interface.h for a more detailed description.
**------------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
   { 10, PD_READ,  AD_MAP_ALL_ELEM, 0 },
   { 11, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};


/*------------------------------------------------------------------------------
** Callback of type ABCC_GetAdiValueFuncType. The function will be called when
** the network reads ADI #11. It will increment the value of ADI#12 upon each
** call.
**
** ABCC_GetAdiValueFuncType is declared in abcc_application_data_interface.h
**------------------------------------------------------------------------------
*/
static void GetAdi11Value( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex )
{
   appl_Uint16_12++;
}


/*------------------------------------------------------------------------------
** Callback of type ABCC_SetAdiValueFuncType. The function will be called when
** the network writes to ADI#10. It copies the changed values from ADI#10 to
** ADI#11.
**
** ABCC_SetAdiValueFuncType is declared in abcc_application_data_interface.h
**------------------------------------------------------------------------------
*/
static void SetAdi10Value( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex )
{
   UINT8 index;
   for ( index = bStartIndex; index < bStartIndex + bNumElements; index++ )
   {
      appl_aiUint16_11[ index ] = appl_aiUint16_10[ index ];
   }
}


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
