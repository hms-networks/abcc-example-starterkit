/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** See network_settings_sequence.h.
********************************************************************************
*/
#include <stdio.h>
#include "abcc_types.h"
#include "abcc_config.h"
#include "abp.h"
#include "abcc.h"
#include "abcc_command_sequencer_interface.h"
#include "abcc_log.h"
#include "abcc_api.h"
#include "implemented_callback_functions.h"
#include "network_settings_sequence.h"

/*
** Network Configuration object instances for the IP settings.
*/
#define NC_INST_IP_ADDRESS    3
#define NC_INST_NETMASK       4
#define NC_INST_GATEWAY       5
#define NC_INST_DHCP          6

/*
** Valid range of the address switch for the last byte of the IP address.
*/
#define IP_SWITCH_VALUE_MIN   1
#define IP_SWITCH_VALUE_MAX   254

/*------------------------------------------------------------------------------
** Values to write and flags telling which of them are valid.
**------------------------------------------------------------------------------
*/
static UINT16 appl_iNodeAddress;
static UINT8  appl_bBaudrate;
static UINT8  appl_bIpLastByte;
static BOOL   appl_fSetNodeAddress = FALSE;
static BOOL   appl_fSetBaudrate = FALSE;
static BOOL   appl_fSetIp = FALSE;

/*------------------------------------------------------------------------------
** Static functions
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType SetNodeAddress( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType SetBaudrate( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType SetIpAddress( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType SetNetmask( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType SetGateway( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType SetDhcp( ABP_MsgType* psMsg, void* pxUserData );
static void NetworkSettingsDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData );

static const ABCC_CmdSeqType appl_asNetworkSettingsCmdSeq[] =
{
   ABCC_CMD_SEQ( SetNodeAddress, NULL ),
   ABCC_CMD_SEQ( SetBaudrate, NULL ),
   ABCC_CMD_SEQ( SetIpAddress, NULL ),
   ABCC_CMD_SEQ( SetNetmask, NULL ),
   ABCC_CMD_SEQ( SetGateway, NULL ),
   ABCC_CMD_SEQ( SetDhcp, NULL ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Command handlers of the sequence. Each one either builds a message
** (ABCC_CMDSEQ_CMD_SEND) or skips the command (ABCC_CMDSEQ_CMD_SKIP).
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType SetNodeAddress( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( appl_fSetNodeAddress )
   {
      ABCC_SetMsgHeader( psMsg, ABP_OBJ_NUM_NC, ABP_NC_INST_NUM_SW1,
                         ABP_NC_VAR_IA_VALUE, ABP_CMD_SET_ATTR,
                         ABP_UINT16_SIZEOF, ABCC_GetNewSourceId() );
      ABCC_SetMsgData16( psMsg, appl_iNodeAddress, 0 );
      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

static ABCC_CmdSeqCmdStatusType SetBaudrate( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( appl_fSetBaudrate )
   {
      ABCC_SetByteAttribute( psMsg, ABP_OBJ_NUM_NC, ABP_NC_INST_NUM_SW2,
                             ABP_NC_VAR_IA_VALUE, appl_bBaudrate,
                             ABCC_GetNewSourceId() );
      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

static ABCC_CmdSeqCmdStatusType SetIpAddress( ABP_MsgType* psMsg, void* pxUserData )
{
   static const UINT8 abIpNetwork[ 4 ] = { 192, 168, 0, 0 };
   UINT8 abIp[ 4 ];

   (void)pxUserData;

   if( appl_fSetIp )
   {
      abIp[ 0 ] = abIpNetwork[ 0 ];
      abIp[ 1 ] = abIpNetwork[ 1 ];
      abIp[ 2 ] = abIpNetwork[ 2 ];
      abIp[ 3 ] = appl_bIpLastByte;
      ABCC_SetMsgHeader( psMsg, ABP_OBJ_NUM_NC, NC_INST_IP_ADDRESS,
                         ABP_NC_VAR_IA_VALUE, ABP_CMD_SET_ATTR, 4,
                         ABCC_GetNewSourceId() );
      ABCC_SetMsgString( psMsg, (char*)abIp, 4, 0 );
      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

static ABCC_CmdSeqCmdStatusType SetNetmask( ABP_MsgType* psMsg, void* pxUserData )
{
   static const UINT8 abNetmask[ 4 ] = { 255, 255, 255, 0 };

   (void)pxUserData;

   if( appl_fSetIp )
   {
      ABCC_SetMsgHeader( psMsg, ABP_OBJ_NUM_NC, NC_INST_NETMASK,
                         ABP_NC_VAR_IA_VALUE, ABP_CMD_SET_ATTR, 4,
                         ABCC_GetNewSourceId() );
      ABCC_SetMsgString( psMsg, (char*)abNetmask, 4, 0 );
      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

static ABCC_CmdSeqCmdStatusType SetGateway( ABP_MsgType* psMsg, void* pxUserData )
{
   static const UINT8 abGateway[ 4 ] = { 0, 0, 0, 0 };

   (void)pxUserData;

   if( appl_fSetIp )
   {
      ABCC_SetMsgHeader( psMsg, ABP_OBJ_NUM_NC, NC_INST_GATEWAY,
                         ABP_NC_VAR_IA_VALUE, ABP_CMD_SET_ATTR, 4,
                         ABCC_GetNewSourceId() );
      ABCC_SetMsgString( psMsg, (char*)abGateway, 4, 0 );
      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

static ABCC_CmdSeqCmdStatusType SetDhcp( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( appl_fSetIp )
   {
      /* DHCP disabled, otherwise the static IP address would be ignored. */
      ABCC_SetByteAttribute( psMsg, ABP_OBJ_NUM_NC, NC_INST_DHCP,
                             ABP_NC_VAR_IA_VALUE, 0, ABCC_GetNewSourceId() );
      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
** Sequence done: Let the abcc-api continue with the setup of the module.
** This is done also if the sequence was aborted, so the module does not stay
** in state SETUP.
**------------------------------------------------------------------------------
*/
static void NetworkSettingsDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData )
{
   (void)pxUserData;

   if( eSeqResult != ABCC_CMDSEQ_RESULT_COMPLETED )
   {
      ABCC_LOG_INFO( "Network settings sequence was aborted (%d).\n", eSeqResult );
   }
   printf( "Now calling ABCC_API_UserInitComplete() to progress from SETUP state to NW_INIT.\n\n" );
   ABCC_API_UserInitComplete();
}

/*------------------------------------------------------------------------------
** APPL_NetworkSettingsStart()
**------------------------------------------------------------------------------
*/
void APPL_NetworkSettingsStart( ABCC_API_NetworkType iNetworkType )
{
   BOOL fNodeIdType = FALSE;
   BOOL fBaudrateType = FALSE;
   BOOL fIpType = FALSE;
   UINT16 iAddress;
   UINT8 bBaudrate;

   appl_fSetNodeAddress = FALSE;
   appl_fSetBaudrate = FALSE;
   appl_fSetIp = FALSE;

   switch( iNetworkType )
   {
   case ABP_NW_TYPE_DEV:
   case ABP_NW_TYPE_COP:
   case ABP_NW_TYPE_CCL:
      /* Node address and baud rate */
      fNodeIdType = TRUE;
      fBaudrateType = TRUE;
      break;

   case ABP_NW_TYPE_PDPV0:
   case ABP_NW_TYPE_PDPV1:
   case ABP_NW_TYPE_CNT:
   case ABP_NW_TYPE_CFN:
   case ABP_NW_TYPE_CPN:
   case ABP_NW_TYPE_ECT:
   case ABP_NW_TYPE_EPL:
      /* Node address only */
      fNodeIdType = TRUE;
      break;

   case ABP_NW_TYPE_EIP_1P:
   case ABP_NW_TYPE_EIP_2P:
   case ABP_NW_TYPE_EIP_2P_BB:
   case ABP_NW_TYPE_EIP_2P_BB_IIOT:
   case ABP_NW_TYPE_ETN_1P:
   case ABP_NW_TYPE_ETN_2P:
      /*
      ** Last byte of the IP address (upper 3 bytes are fixed), only if enabled.
      ** Note: PROFINET modules are deliberately not listed. HMS recommends not
      ** to set the IP address in state SETUP for them (GSD file would need
      ** AddressAssignment=DCP;LOCAL).
      */
      fIpType = (BOOL)APPL_CFG_IP_BY_SWITCH_ENABLED;
      break;

   default:
      /* Nothing to provide for other module types. */
      break;
   }

   if( ( fNodeIdType || fIpType ) &&
       ( APPL_CbfGetNodeAddress( &iAddress ) ) )
   {
      if( fNodeIdType )
      {
         appl_iNodeAddress = iAddress;
         appl_fSetNodeAddress = TRUE;
         printf( "   Node address value provided: %d\n", iAddress );
      }
      else if( ( iAddress >= IP_SWITCH_VALUE_MIN ) &&
               ( iAddress <= IP_SWITCH_VALUE_MAX ) )
      {
         appl_bIpLastByte = (UINT8)iAddress;
         appl_fSetIp = TRUE;
         printf( "   Last byte of IP address provided: %d\n", iAddress );
      }
      else
      {
         /* 0 and 255: not valid as IP address, nothing is written. */
      }
   }

   if( ( fBaudrateType ) &&
       ( APPL_CbfGetBaudrate( &bBaudrate ) ) )
   {
      appl_bBaudrate = bBaudrate;
      appl_fSetBaudrate = TRUE;
      printf( "   Bit rate value provided: %d\n", bBaudrate );
   }

   if( ( appl_fSetNodeAddress ) ||
       ( appl_fSetBaudrate ) ||
       ( appl_fSetIp ) )
   {
      if( ABCC_CmdSeqAdd( appl_asNetworkSettingsCmdSeq, NetworkSettingsDone,
                          NULL, NULL ) != ABCC_EC_NO_ERROR )
      {
         /* Could not start the sequence: do not block the setup. */
         NetworkSettingsDone( ABCC_CMDSEQ_RESULT_ABORT_INT, NULL );
      }
   }
   else
   {
      NetworkSettingsDone( ABCC_CMDSEQ_RESULT_COMPLETED, NULL );
   }
}
