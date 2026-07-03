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
** 1. iInstance | 2. pacName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6. pxValuePtr | 7. pxValueProps
**-------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   {  0x1,  "SPEED",     ABP_UINT16,   1, AD_ADI_DESC___W_G, { { &appl_iSpeed,    &appl_sUint16Prop } } },
   {  0x2,  "REF_SPEED", ABP_UINT16,   1, AD_ADI_DESC__R_S_, { { &appl_iRefSpeed, &appl_sUint16Prop } } }
};

/*------------------------------------------------------------------------------
** Map all ADI:s in both directions
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
   return( sizeof( ABCC_API_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

/*------------------------------------------------------------------------------
** Example - electric motor control loop
**------------------------------------------------------------------------------
*/
void ABCC_API_CbfCyclicalProcessing()
{
   if( ABCC_API_AnbState() == ABP_ANB_STATE_PROCESS_ACTIVE )
   {
      /*
      ** An example of ADI data handling.
      */
      if( appl_iSpeed > appl_iRefSpeed )
      {
         /*
         ** Do something that lowers speed.
         */
         appl_iSpeed -= 1;
      }
      else if( appl_iSpeed < appl_iRefSpeed )
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
