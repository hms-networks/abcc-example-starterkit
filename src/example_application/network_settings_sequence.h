/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Application module which writes the network settings (node address, baud
** rate, last byte of the IP address) to the Network Configuration object of the
** ABCC module while the module is in state SETUP. The abcc-driver-api is not
** modified and is not asked to send these settings.
********************************************************************************
*/
#ifndef NETWORK_SETTINGS_SEQUENCE_H_
#define NETWORK_SETTINGS_SEQUENCE_H_

#include "abcc_types.h"
#include "abcc_api.h"

/*------------------------------------------------------------------------------
** APPL_CFG_IP_BY_SWITCH_ENABLED
** Optional compile switch. Set to TRUE if the last byte of the IP address is
** provided by (real or emulated) hardware address switches. Then
**  - the flag "Hardware configurable address" (Application object, attribute
**    #6) is reported as TRUE (see ABCC_CbfApplicationObjHWConfAddress_Get()),
**  - the IP settings are written in state SETUP, but only for Ethernet modules
**    which allow it and only if the switch value is in the range 1..254.
** The values 0 and 255 are not valid and are never written; the module then
** keeps its own IP settings (e.g. DHCP, DCP, web interface).
** Default: FALSE (no IP address is written in state SETUP).
**------------------------------------------------------------------------------
*/
#ifndef APPL_CFG_IP_BY_SWITCH_ENABLED
   #define APPL_CFG_IP_BY_SWITCH_ENABLED   ( FALSE )
#endif

/*------------------------------------------------------------------------------
** APPL_NetworkSettingsStart()
** To be called from ABCC_API_CbfUserInit(). Requests the settings from
** APPL_CbfGetNodeAddress() / APPL_CbfGetBaudrate() (only for module types where
** they apply), writes them to the module with a command sequence and calls
** ABCC_API_UserInitComplete() when the sequence is finished. If there is
** nothing to write, ABCC_API_UserInitComplete() is called immediately.
** Do NOT call ABCC_API_UserInitComplete() elsewhere in ABCC_API_CbfUserInit().
**------------------------------------------------------------------------------
** Arguments:
**    iNetworkType - Network type as received by ABCC_API_CbfUserInit()
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
void APPL_NetworkSettingsStart( ABCC_API_NetworkType iNetworkType );

#endif /* NETWORK_SETTINGS_SEQUENCE_H_ */
