/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
**
** ADI example "B_simple_16"
**
** File Description:
** Example of an ADI setup with an array of 16 bit values;
** the output data of the PLC are mirrored as the same array is 
** used for output and input data.
**
** Ensure the following definitions, if defined in abcc_driver_config.h,
** are set to:
**    ABCC_CFG_STRUCT_DATA_TYPE_ENABLED      0
**    ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED  0
********************************************************************************
*/

#include "abcc_api.h"

#if ( ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
#error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to 0 and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to 0 in order to run this example
#endif


/*------------------------------------------------------------------------------
** Data holders for the ADI instances.
**------------------------------------------------------------------------------
*/
static UINT16 appl_aiUint16[ 32 ];


/*------------------------------------------------------------------------------
** Min, max and default value for appl_aiUint16.
**------------------------------------------------------------------------------
*/
static AD_UINT16Type appl_sUint16Prop = { { 0, 0xFFFF, 0 } };


/*------------------------------------------------------------------------------
** ADI table.
**------------------------------------------------------------------------------
*/

/*-------------------------------------------------------------------------------------------------------------
** 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6. pxValuePtr | 7. pxValuePropPtr
**--------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   { 0x1, "ABP_UINT16_WRITE", ABP_UINT16, 32, AD_ADI_DESC___W_G, { { appl_aiUint16, &appl_sUint16Prop } } },
   { 0x2, "ABP_UINT16_READ",  ABP_UINT16, 32, AD_ADI_DESC__R_SG, { { appl_aiUint16, &appl_sUint16Prop } } }
};


/*------------------------------------------------------------------------------
** Map all ADIs in both directions.
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


UINT16 ABCC_API_CbfGetNumAdi( void )
{
    return( sizeof( ABCC_API_asAdiEntryList ) / sizeof( AD_AdiEntryType ));
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
