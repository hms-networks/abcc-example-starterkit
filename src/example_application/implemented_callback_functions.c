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
#include "implemented_callback_functions.h"
#include "network_settings_sequence.h"

#ifndef APPL_CFG_SET_FIELDBUS_TEST_ADDRESS
   /*
   ** Some networks require the application to set a network device address and
   ** communication bit rate.
   **
   ** While primarily intended for fieldbus versions of the modules, this may
   ** also apply to Industrial Ethernet (e.g. Ethernet POWERLINK).
   ** Defining the compiler switch APPL_CFG_SET_FIELDBUS_TEST_ADDRESS to TRUE
   ** activates embedded test code to set a hardcoded address and bit rate
   ** during system startup.
   ** The values passed must be adapted to your specific test scenario and fall
   ** within the permitted ranges.
   ** Refer to the respective Network Guides for supported device addresses and
   ** their permitted value ranges (Network Configuration Object 04h, Instance
   ** 1). Supported bit rates and ENUM definitions are listed there, as well.
   ** (Network Configuration Object (04h), Instance 2)
   **
   ** Note 1:
   ** The node address (and baud rate) is provided to modules for fieldbus
   ** networks that have these as network parameters. For Ethernet based
   ** modules, the address value sets the last byte of the IP address (upper 3
   ** bytes are fixed), if APPL_CFG_IP_BY_SWITCH_ENABLED is set.
   **
   ** Note 2:
   ** For CC-Link IE TSN, there are additional communication settings that can
   ** or must be set via ABCC_API_SetCommSettings(). The call to this function
   ** must be added if necessary.
   **
   ** Note 3:
   ** The field device requires physical address switches or another
   ** configuration option directly on the device. While proprietary tools may
   ** be offered as an optional convenience, it is not recommended for them to
   ** be the sole method for configuring the network address or bit rate.
   */
   #define APPL_CFG_SET_FIELDBUS_TEST_ADDRESS (FALSE)
#endif /* !ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS */

#if APPL_CFG_SET_FIELDBUS_TEST_ADDRESS
   #define NODE_ADDRESS_TEST_VALUE    10
   #define BIT_RATE_TEST_VALUE         1
#endif /* ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS */

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
   /* Return TRUE, if the IP address is set by (emulated) address switches
      (APPL_CbfGetNodeAddress()), even if currently configured by software.
      Otherwise FALSE to indicate that network address is not configurable in
      hardware. */
   return( (BOOL8)APPL_CFG_IP_BY_SWITCH_ENABLED );
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

BOOL APPL_CbfGetNodeAddress( UINT16* piAddress )
{
   BOOL fAddressProvided = FALSE;

   /*
   ** Todo: PORTING ALERT!
   ** Read the address from switches, a configuration menu or NVS, store it in
   ** *piAddress and set fAddressProvided to TRUE. See description in
   ** implemented_callback_functions.h.
   **
   ** NOTE: fAddressProvided is initialized to FALSE, which means that no
   ** address is provided. It shall be set to TRUE when an address is stored in
   ** *piAddress, because the returned value tells the caller whether the
   ** address shall be written to the module.
   */
   (void)piAddress;
#if APPL_CFG_SET_FIELDBUS_TEST_ADDRESS
   /*
   ** TEMPORARY: Hardcoded value for use with the starterkit, which has no HW
   ** switches.
   */
   *piAddress = NODE_ADDRESS_TEST_VALUE;
   fAddressProvided = TRUE;
#endif /* APPL_CFG_SET_FIELDBUS_TEST_ADDRESS */

   return( fAddressProvided );
}

BOOL APPL_CbfGetBaudrate( UINT8* pbBaudrate )
{
   BOOL fBaudrateProvided = FALSE;

   /*
   ** Todo: PORTING ALERT!
   ** Read the baud rate from switches, a configuration menu or NVS, store it in
   ** *pbBaudrate and set fBaudrateProvided to TRUE. See description in
   ** implemented_callback_functions.h.
   **
   ** NOTE: fBaudrateProvided is initialized to FALSE, which means that no baud
   ** rate is provided. It shall be set to TRUE when a baud rate is stored in
   ** *pbBaudrate, because the returned value tells the caller whether the baud
   ** rate shall be written to the module.
   */
   (void)pbBaudrate;
#if APPL_CFG_SET_FIELDBUS_TEST_ADDRESS
   /*
   ** TEMPORARY: Hardcoded value for use with the starterkit, which has no HW
   ** switches.
   */
   *pbBaudrate = BIT_RATE_TEST_VALUE;
   fBaudrateProvided = TRUE;
#endif /* APPL_CFG_SET_FIELDBUS_TEST_ADDRESS */

   return( fBaudrateProvided );
}
