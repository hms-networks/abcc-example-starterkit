/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Simple example implementations of callback functions used by command handler
** lookup table.
********************************************************************************
*/

#include "abcc_types.h"
#include "abcc_api_command_handler_lookup.h"
#include "abcc_log.h"
#include "abcc_api.h"

static BOOL8 fFirmwareAvailable = FALSE;
static UINT32 lSerialNumber = 0xFF000001;
static UINT16 iDeviceType_EtherNetIP = 0xFFFF;
static UINT16 iProductCode_EtherNetIP = 0x4321;
static UINT16 iDeviceID_PROFINET = 0x1234;
static const char* pacProductName = "My dynamic product name";

UINT32 ABCC_CbfApplicationObjSerialNum_Get( void )
{
   return( lSerialNumber );
}

UINT16 ABCC_CbfApplicationObjProductName_Get( char* pPackedStrDest, UINT16 iBuffSize )
{
   UINT16 iStrLength = (UINT16)strlen( pacProductName );

   iStrLength = iStrLength > iBuffSize ? iBuffSize : iStrLength;
   memcpy( pPackedStrDest, pacProductName, iStrLength );
   return( iStrLength );
}

void ABCC_CbfApplicationObjFirmwareAvailable_Set( BOOL8 fValue )
{
   /* Store FW flag in non-volatile storage (NVS). */
   fFirmwareAvailable = fValue;
   ABCC_LOG_INFO( "Candidate FW flag: %u\n", fValue );
}

BOOL8 ABCC_CbfApplicationObjFirmwareAvailable_Get( void )
{
   /* Return FW flag from non-volatile storage (NVS). */
   return( fFirmwareAvailable );
}

BOOL8 ABCC_CbfApplicationObjHWConfAddress_Get( void )
{
   /* Return FALSE to indicate that network address is not configurable in 
      hardware. */
   return( FALSE );
}

UINT16 ABCC_CbfEthernetIpObjDeviceType_Get( void )
{
   return( iDeviceType_EtherNetIP );
}

UINT16 ABCC_CbfEthernetIpObjProductCode_Get( void )
{
   return( iProductCode_EtherNetIP );
}

UINT16 ABCC_CbfProfinetIoObjDeviceId_Get( void )
{
   return( iDeviceID_PROFINET );
}

UINT16 ABCC_CbfProfinetIoObjOrderId_Get( char* pPackedStrDest, UINT16 iBuffSize )
{
   static const char* my_product_name = "My dynamic OrderId";
   UINT16 iStrLength = (UINT16)strlen( my_product_name );

   iStrLength = iStrLength > iBuffSize ? iBuffSize : iStrLength;
   memcpy( pPackedStrDest, my_product_name, iStrLength );
   return( iStrLength );
}

void ABCC_CbfApplicationObj_Reset( ABP_ResetType eResetType )
{
   switch( eResetType )
   {
   case ABP_RESET_FACTORY_DEFAULT:
      /*
      ** Todo: PORTING ALERT!
      ** Restore parameters stored in NVS to their default values
      */
      break;

   case ABP_RESET_POWER_ON_FACTORY_DEFAULT:
      /*
      ** Todo: PORTING ALERT!
      ** Restore parameters stored in NVS to their default values
      */
      ABCC_API_Restart();
      break;

   case ABP_RESET_POWER_ON:
      ABCC_API_Restart();
      break;

   default:
      break;
   }
}

BOOL8 ABCC_CbfApplicationObj_ResetRequest( ABP_ResetType eResetType )
{
   ABCC_LOG_INFO( "Accepted reset request of type: %u\n", eResetType );
   return ( TRUE );
}
