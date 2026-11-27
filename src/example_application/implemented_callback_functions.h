/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Callback functions implemented by the host application, which are used by
** main.c to request the network address and baud rate of the device.
********************************************************************************
** Services:
**
** APPL_CbfGetNodeAddress() - Get node address / last byte of IP address
** APPL_CbfGetBaudrate()    - Get baud rate
********************************************************************************
*/

#ifndef IMPLEMENTED_CALLBACK_FUNCTIONS_H_
#define IMPLEMENTED_CALLBACK_FUNCTIONS_H_

#include "abcc_types.h"

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Callback function, called from ABCC_API_CbfUserInit() to request the network
** address of the device, if the plugged module has an address that can be
** provided by the host application.
**
** The function shall read the address from the source used by the product, e.g.
** address switches, a configuration menu or a non-volatile memory (NVS). The
** value is written to the module by APPL_NetworkSettingsStart() (see
** network_settings_sequence.h) before the Anybus state changes from SETUP to
** NW_INIT.
**
** The meaning of the value depends on the network type of the module:
** - Fieldbus modules (e.g. PROFIBUS, DeviceNet, CANopen, CC-Link): Node
**   address. Refer to the Network Guide of the module for the permitted range
**   (Network Configuration object, instance 1).
** - Ethernet based modules (only if APPL_CFG_IP_BY_SWITCH_ENABLED is set, e.g.
**   EtherNet/IP and Modbus-TCP): Last byte of the IP address. The upper 3 bytes
**   are fixed. Only values in the range 1..254 are applied. The values 0 and
**   255 are accepted, but no IP settings are written to the module, which for
**   example allows the IP address to be configured by software (DHCP, TCP/IP
**   object or other methods).
**
** Return FALSE if no address shall be provided to the module, e.g. if the
** product has no address switches or the address is configured by other means.
**------------------------------------------------------------------------------
** Arguments:
**    piAddress - Pointer to the variable where the address is stored. Only
**                valid if TRUE is returned.
**
** Returns:
**    TRUE:  *piAddress contains the address to be written to the module
**    FALSE: No address shall be written to the module
**------------------------------------------------------------------------------
*/
BOOL APPL_CbfGetNodeAddress( UINT16* piAddress );

/*------------------------------------------------------------------------------
** Callback function, called from ABCC_API_CbfUserInit() to request the baud
** rate of the device. It is only called, if the plugged module is a fieldbus
** module with a baud rate as network parameter (DeviceNet, CANopen and
** CC-Link).
**
** The function shall read the baud rate from the source used by the product,
** e.g. a configuration switch, a configuration menu or a non-volatile memory
** (NVS). The value is written to the module by APPL_NetworkSettingsStart()
** before the Anybus state changes from SETUP to NW_INIT.
**
** The value is the enumeration value of the baud rate as defined for the
** Network Configuration object, instance 2, in the Network Guide of the module.
**
** Return FALSE if no baud rate shall be provided to the module, e.g. if the
** baud rate is configured by other means, or detected automatically.
**------------------------------------------------------------------------------
** Arguments:
**    pbBaudrate - Pointer to the variable where the baud rate is stored. Only
**                 valid if TRUE is returned.
**
** Returns:
**    TRUE:  *pbBaudrate contains the baud rate to be written to the module
**    FALSE: No baud rate shall be written to the module
**------------------------------------------------------------------------------
*/
BOOL APPL_CbfGetBaudrate( UINT8* pbBaudrate );

#endif  /* inclusion lock */
