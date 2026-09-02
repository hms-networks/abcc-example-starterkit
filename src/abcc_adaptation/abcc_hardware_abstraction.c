/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description: Hardware abstraction layer for the
** Anybus Starter Kit Windows port.
********************************************************************************
*/
#include "windows.h"
#include "process.h"
#include "tp.h"
#include "imp_tp.h"

#include "abcc_config.h"
#include "abcc_port.h"
#include "abcc.h"

#include "abcc_hardware_abstraction.h"
#include "abcc_hardware_abstraction_spi.h"
#include "abcc_hardware_abstraction_parallel.h"
#include "abcc_hardware_abstraction_serial.h"


BOOL ABCC_StartTransportProvider( void );
void ABCC_CloseTransportProvider( void );

EXTFUNC void ( *ABCC_ISR )( void );
unsigned __stdcall ISR( void *pMyID );

HANDLE hThread;
unsigned threadID;
unsigned runISR = 0;

#define TP_USB2_SPECIFIC_CMD_GET_PORT_C ( 0x04 )
#define USB2_PORT_C_MI_MASK 0x03
#define USB2_PORT_C_MD_MASK 0x0C

#define TP_USB2_SPECIFIC_CMD_GET_PORT_E ( 0x06 )
#define USB2_PORT_E_IRQ 0x01

/* The ACI external memory map is 16 kB. */
#define ACI_MEMORY_MAP_SIZE 16384

static ABCC_HAL_SpiDataReceivedCbfType pnSpiDataReadyCbf;
static ABCC_HAL_SerDataReceivedCbfType pnSerDataReadyCbf;

static UINT8 sys_abReadProcessData[ ABCC_CFG_MAX_PROCESS_DATA_SIZE ];  /* Process data byte array. */
static UINT8 sys_abWriteProcessData[ ABCC_CFG_MAX_PROCESS_DATA_SIZE ]; /* Process data byte array. */

static TP_Path xPathHandle = NULL;
static UINT32 lPathId = 0;

static UINT8 sys_bOpmode = 0;
static TP_InterfaceType eInterface = TP_ANY;


/* Global to ease debugging: This is the last received status code from the TP. */
static TP_StatusType eLastReceivedTpPariStatus;

void TP_Shutdown( void )
{
   ABCC_CloseTransportProvider();
}

/*
** Explicitly set which TP Path ID to use. It is optional to call this
** function. lPathId is initialised to '0' which means "let the user select
** the path ID manually at startup".
*/
void TP_vSetPathId( UINT32 lValue )
{
   lPathId = lValue;
   return;
}

static UINT8 TP_Command( UINT8 bCommand )
{
   TP_StatusType eStatus;
   TP_MessageType  sMsg;
   sMsg.sReq.eCommand = TP_CMD_USB2_SPECIFIC;
   sMsg.sReq.bDataSize = 1;
   sMsg.sReq.abData[0] = bCommand;

   ABCC_PORT_EnterCritical();
   eStatus = TP_ProviderSpecificCommand( xPathHandle, &sMsg );
   ABCC_PORT_ExitCritical();

   if ( eStatus != TP_ERR_NONE )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)eStatus, "Transport Provider error %d\n", eStatus );
   }
   return( sMsg.sRsp.abData[0] );
}


#if( ABCC_CFG_INT_ENABLED )
unsigned __stdcall ISR( void *pMyID )
{
   UINT8 bTpPortE;
   (void) pMyID;

   while( runISR )
   {
      Sleep(1);
      bTpPortE = TP_Command( TP_USB2_SPECIFIC_CMD_GET_PORT_E );
      if ( ( bTpPortE & USB2_PORT_E_IRQ ) != USB2_PORT_E_IRQ )
      {
         ABCC_PORT_EnterCritical();
         ABCC_ISR();
         ABCC_PORT_ExitCritical();
      }
   }
   _endthreadex( 0 );
   return( 0 );
}
#endif


BOOL ABCC_HAL_HwInit( void )
{
   if( ABCC_StartTransportProvider() )
   {
      ABCC_HAL_HWReset();
      return( TRUE );
   }
   return( FALSE );
}

BOOL ABCC_HAL_Init( void )
{
   return( TRUE );
}

void ABCC_HAL_Close( void )
{
}

#if( ABCC_CFG_DRV_SPI_ENABLED )
void ABCC_HAL_SpiRegDataReceived( ABCC_HAL_SpiDataReceivedCbfType pnDataReceived )
{
   pnSpiDataReadyCbf = pnDataReceived;
}

void ABCC_HAL_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   TP_StatusType eStatus;
   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();
   eStatus = TP_SpiTransaction( xPathHandle, pxSendDataBuffer, pxReceiveDataBuffer, iLength );

   ABCC_PORT_ExitCritical();

   if ( eStatus == TP_ERR_NONE )
   {
      if( pnSpiDataReadyCbf )
      {
         pnSpiDataReadyCbf();
      }
   }
   else
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)eStatus,
         "ERROR in SPI transaction: ERR: 0x%x\n",
         eStatus );
   }
}
#endif


#if( ABCC_CFG_DRV_PARALLEL_ENABLED && !ABCC_CFG_MEMORY_MAPPED_ACCESS_ENABLED )
void ABCC_HAL_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   if( iLength == 0 )
   {
      return;
   }

   ABCC_PORT_EnterCritical();
   eLastReceivedTpPariStatus = TP_ParallelRead( xPathHandle, iMemOffset, pxData, iLength );
   ABCC_PORT_ExitCritical();
   if( eLastReceivedTpPariStatus == TP_ERR_NONE )
   {
   }
   else
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)eLastReceivedTpPariStatus,
         "Read failed with error code %x\n",
         eLastReceivedTpPariStatus );
   }
}

#if( ABCC_CFG_DRV_PARALLEL_ENABLED )
UINT16 ABCC_HAL_ParallelRead16( UINT16 iMemOffset )
{
   UINT16 iData;
   ABCC_HAL_ParallelRead( iMemOffset, (UINT8*)&iData, sizeof( UINT16 ) );
   return( iData );
}
#endif


void ABCC_HAL_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{

   ABCC_PORT_EnterCritical();
   eLastReceivedTpPariStatus = TP_ParallelWrite( xPathHandle, iMemOffset, pxData, iLength );
   ABCC_PORT_ExitCritical();

   if( eLastReceivedTpPariStatus != TP_ERR_NONE )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)eLastReceivedTpPariStatus,
         "Write failed with error code %x\n",
         eLastReceivedTpPariStatus );
   }
}

#if( ABCC_CFG_DRV_PARALLEL_ENABLED )
void ABCC_HAL_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   ABCC_HAL_ParallelWrite( iMemOffset, (UINT8*)&piData, sizeof( UINT16 ) );
}
#endif
#endif

#if( ABCC_CFG_OP_MODE_SETTABLE )
void ABCC_HAL_SetOpmode( UINT8 bOpmode )
{
   /*
   ** This is already done in ABCC_HAL_HwInit. Otherwise we cannot read the MD or MI
   ** values from the USB2 board.
   */
   (void)bOpmode;
}
#endif


#if( ABCC_CFG_OP_MODE_GETTABLE )
UINT8 ABCC_HAL_GetOpmode( void )
{
   return( sys_bOpmode );
}
#endif


void ABCC_HAL_HWReset( void )
{
   TP_StatusType eStatus;
   TP_MessageType sMsg;

   sMsg.sReq.eCommand = TP_CMD_RESET;
   sMsg.sReq.bDataSize = 1;
   sMsg.sReq.abData[0] = 0;

   ABCC_PORT_EnterCritical();
   eStatus = TP_ProviderSpecificCommand( xPathHandle, &sMsg );
   ABCC_PORT_ExitCritical();
}


void ABCC_HAL_HWReleaseReset( void )
{
   TP_StatusType eStatus;
   TP_MessageType sMsg;
   sMsg.sReq.eCommand = TP_CMD_RESET;
   sMsg.sReq.bDataSize = 1;
   sMsg.sReq.abData[0] = 1;

   ABCC_PORT_EnterCritical();
   eStatus = TP_ProviderSpecificCommand( xPathHandle, &sMsg );
   ABCC_PORT_ExitCritical();

}


#if ABCC_CFG_MODULE_ID_PINS_CONN
UINT8 ABCC_HAL_ReadModuleId( void )
{
   UINT8 bTpPortC;
   bTpPortC = TP_Command( TP_USB2_SPECIFIC_CMD_GET_PORT_C );
   return( bTpPortC & USB2_PORT_C_MI_MASK );
}
#endif


#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
BOOL ABCC_HAL_ModuleDetect( void )
{
   UINT8 bTpPortC;
   bTpPortC = TP_Command( TP_USB2_SPECIFIC_CMD_GET_PORT_C );

   return( ( bTpPortC & USB2_PORT_C_MD_MASK ) == 0 );
}
#endif

#if( !ABCC_CFG_MEMORY_MAPPED_ACCESS_ENABLED )
void* ABCC_HAL_ParallelGetRdPdBuffer( void )
{
   return( sys_abReadProcessData );
}


void* ABCC_HAL_ParallelGetWrPdBuffer( void )
{
   return( sys_abWriteProcessData );
}
#endif

#ifdef ABCC_CFG_DRV_SERIAL_ENABLED
void ABCC_HAL_SerRegDataReceived( ABCC_HAL_SerDataReceivedCbfType pnDataReceived  )
{
   pnSerDataReadyCbf = pnDataReceived;
}


void ABCC_HAL_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{

   TP_StatusType  eStatus;
   UINT16         iRdOffset;
   UINT16         iTemp;

   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();
   eStatus = TP_SerialWriteBlocking( xPathHandle, pxTxDataBuffer, iTxSize );
   ABCC_PORT_ExitCritical();

   if (eStatus != TP_ERR_NONE )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)eStatus,
         "Serial TX error: ERR: 0x%x", eStatus );
      return;
   }

   iRdOffset = 0;
   ABCC_PORT_EnterCritical();
   while( iRdOffset < iRxSize )
   {
      iTemp = iRxSize - iRdOffset;
      /*
      ** The timeout is only used in the RX direction, so the longest
      ** listed "Tsend" (175ms) plus some arbitrary margin ought to be
      ** enough for the TP_SerialRead() call.
      */
      eStatus = TP_SerialRead( xPathHandle, (UINT8*)pxRxDataBuffer + iRdOffset, &iTemp, 200 );
      if( eStatus == TP_ERR_NONE )
      {
         if( iTemp > 0 )
         {
            iRdOffset += iTemp;
         }
         else
         {
            ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, 0, "Serial RX timeout!\n" );
            break;
         }
      }
      else
      {
         break;
      }
   }
   ABCC_PORT_ExitCritical();

   if ( eStatus != TP_ERR_NONE )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)eStatus,
         "Serial RX error: ERR: 0x%x\n", eStatus );
      return;
   }

   if( pnSerDataReadyCbf )
   {
      pnSerDataReadyCbf();
   }
}


void ABCC_HAL_SerRestart( void )
{
   UINT8          bTemp;
   TP_StatusType  eStatus;
   UINT16         iSize;

   /*
   ** Empty the RX buffer in the TP system.
   */
   do
   {
      iSize = 1;
      eStatus = TP_SerialRead( xPathHandle, &bTemp, &iSize, 0 );
   }
   while( ( eStatus == TP_ERR_NONE ) && ( iSize > 0 ) );
}
#endif

#if( ABCC_CFG_INT_ENABLED )
void ABCC_HAL_AbccInterruptEnable( void )
{
   runISR  = TRUE;
   hThread = (HANDLE)_beginthreadex( NULL, 0, &ISR, NULL, 0, &threadID );
}
#endif


#if( ABCC_CFG_INT_ENABLED )
void ABCC_HAL_AbccInterruptDisable( void )
{
   if(  runISR )
   {
      runISR  = FALSE;
      WaitForSingleObject( hThread, INFINITE );
      CloseHandle( hThread );
   }
}
#endif

#if( ABCC_CFG_POLL_ABCC_IRQ_PIN_ENABLED )
BOOL ABCC_HAL_IsAbccInterruptActive( void )
{
   UINT8 bTpPortE;
   BOOL fIrq;

   fIrq = FALSE;
   bTpPortE = TP_Command( TP_USB2_SPECIFIC_CMD_GET_PORT_E );
   if( ( bTpPortE & USB2_PORT_E_IRQ ) != USB2_PORT_E_IRQ )
   {
      fIrq = TRUE;
   }

   return( fIrq );
}
#endif


/*
 ** This function will start the Transport Provider connection
 ** Note! This function is called by the application before
 ** the driver is accessed.
 */
BOOL ABCC_StartTransportProvider( void )
{
   TP_StatusType eStatus;

   if ( xPathHandle !=  NULL )
   {
      return( TRUE );
   }

   eStatus = TP_Initialise( "HMSTPRTR.DLL", 0x200 );

   if ( eStatus != TP_ERR_NONE )
   {
      ABCC_LOG_ERROR( ABCC_EC_HAL_ERR, (UINT32)eStatus, "TP_Init failed: %d\n", eStatus );
      return( FALSE );
   }

   if( lPathId == 0 )
   {
      /*
      ** lPathId == 0 -> no path has been set, let the user select one
      ** manually.
      */
      eStatus = TP_UserSelectPath( &eInterface, &lPathId, &xPathHandle );
   }
   else
   {
      /*
      ** lPathId != 0 -> a Path ID has been set explicitly by someone.
      ** Use that.
      */
      eStatus = TP_SelectPath( &eInterface, lPathId, &xPathHandle );
   }

   if( eStatus != TP_ERR_NONE )
   {
      ABCC_LOG_ERROR( ABCC_EC_HAL_ERR, (UINT32)eStatus, "TP_UserSelectPath failed: %d\n", eStatus );
      return( FALSE );
   }


   switch( eInterface )
   {
   case TP_SPI:

      eStatus = TP_SpiOpen( xPathHandle, 12000000, TP_SPI_4WIRE );

      if( eStatus != TP_ERR_NONE )
      {
         ABCC_LOG_ERROR( ABCC_EC_HAL_ERR, (UINT32)eStatus, "TP_SpiOpen failed: %d\n", eStatus );
         return( FALSE );
      }
      sys_bOpmode = ABP_OP_MODE_SPI;

      break;

   case TP_PARALLEL:

      eStatus = TP_ParallelOpen( xPathHandle, ACI_MEMORY_MAP_SIZE );

      if( eStatus != TP_ERR_NONE )
      {
         ABCC_LOG_ERROR( ABCC_EC_HAL_ERR, (UINT32)eStatus, "TP_ParallelOpen failed: %d\n", eStatus );
         return( FALSE );
      }

      if( ( TP_Command( 0x17 ) & 0x03 ) == 0x01 )
      {
         sys_bOpmode = ABP_OP_MODE_16_BIT_PARALLEL;
      }
      else
      {
         sys_bOpmode = ABP_OP_MODE_8_BIT_PARALLEL;
      }

      break;

   case TP_SERIAL:

      eStatus = TP_SerialOpen( xPathHandle, 57600, 8, TP_PARITY_NONE, TP_STOPBIT_ONE );

      if( eStatus != TP_ERR_NONE )
      {
         ABCC_LOG_ERROR( ABCC_EC_HAL_ERR, (UINT32)eStatus, "TP_SerialOpen failed: %d\n", eStatus );
         return( FALSE );
      }
       sys_bOpmode = ABP_OP_MODE_SERIAL_57_6;
      break;

   default:

      ABCC_LOG_ERROR( ABCC_EC_HAL_ERR, (UINT32)eStatus, "Unsupported operating mode: %d\n", eInterface );
      sys_bOpmode = 0;
      return( FALSE );
      break;
   }

   ABCC_HAL_HWReset();

   return( TRUE );
}


/*
 ** This function will close the Transport Provider connection.
 ** It is called by the application at system shutdown
 ** to release Transport Provider resources.
 */
void ABCC_CloseTransportProvider( void )
{
   if ( xPathHandle )
   {
      switch( eInterface )
      {
      case TP_SPI:
         TP_SpiClose( xPathHandle );
         break;

      case TP_PARALLEL:
         TP_ParallelClose( xPathHandle );
         break;

      case TP_SERIAL:
         TP_SerialClose( xPathHandle );

      default:
         /* ERROR: Unexpected interface. Throw an exception? */
         break;
      }
   }

   TP_Close();
   xPathHandle = NULL;
}
