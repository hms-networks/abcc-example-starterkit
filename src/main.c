/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Source file containing the main loop, a basic user interface,
** as well as basic timer and serial port handling.
********************************************************************************
*/

/*
** For "timeGetTime()"
*/
#pragma comment( lib, "Winmm.lib" )

#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "abcc.h"
#include "abcc_log.h"
#include "abcc_hardware_abstraction.h"
#include "abcc_types.h"
#include "abcc_api.h"

#ifndef ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS
   /*
   ** Some networks require the application to set a network device address and
   ** some also require a network communication bit rate to be configured.
   **
   ** Although this primarily affects the fieldbus versions of the modules,
   ** Industrial Ethernet modules (such as Ethernet POWERLINK) may also be
   ** affected.
   ** This compiler switch ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS can be used to
   ** enable the setting of a hard-coded address and baud rate during system
   ** startup.
   ** The values passed must be adapted to your specific test scenario and fall
   ** within the permitted ranges.
   ** Whether a device address is supported and the permitted value range (if
   ** applicable) is documented in the Network Guides for the respective
   ** networks. (Network Configuration Object (04h), Instance 1)
   ** There you will also find information on whether a bit rate can/must be
   ** configured and which actual bit rates the various permitted ENUM values
   ** represent. (Network Configuration Object (04h), Instance 2)
   **
   ** Note 1:
   ** When the test code for setting the address is used in conjunction with
   ** Ethernet modules, the current version of the code sets an IP address of
   ** 192.168.0.xxx (subnet 255.255.255.0, gateway 0.0.0.0) based on the
   ** provided address, using the provided address as the fourth octet ("xxx").
   ** 
   ** Note 2:
   ** For CC-Link IE TSN, there are additional communication settings that can
   ** or must be set via ABCC_API_SetCommSettings(). The call to this function
   ** must be added if necessary.
   ** 
   ** Note 3:
   ** The field device will need address switches or another configuration
   ** option directly at the device.
   ** Even if they might be allowed as additional option, it is not recommended
   ** to implement proprietary tools as only way to change the network address
   ** and/or bit rate.
   */
   #define ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS (FALSE)
#endif /* !ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS */
#if ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS
   #define NODE_ADDRESS_TEST_VALUE    10
   #define BIT_RATE_TEST_VALUE         1
#endif /* ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS */


extern void TP_Shutdown( void );
extern void TP_vSetPathId( UINT32 lValue );

/*------------------------------------------------------------------------------
** RunUi()
** This function handles the user interface.
** Returns TRUE is the user have pressed the Q key.
**------------------------------------------------------------------------------
** Inputs:
**    -
**
** Outputs:
**    Returns:    - TRUE if user have pressed the "Q" key.
**
** Usage:
**    fQuit = RunUi();
**------------------------------------------------------------------------------
*/
BOOL8 RunUi( void )
{
   static char   abUserInput;
   BOOL8         fKbInput = FALSE;

   if( _kbhit() )
   {
      abUserInput = (char)_getch();
      fKbInput = TRUE;

      if( ( abUserInput == 'q' ) ||
          ( abUserInput == 'Q' ) )
      {
         /*
         ** Q is for quit.
         */
         return( TRUE );
      }
   }
   return( FALSE );
} /* End of RunUi() */

/*------------------------------------------------------------------------------
** ABCC_API_CbfUserInit()
** Place to take action based on ABCC module network type and firmware version.
** Calls ABCC_API_UserInitComplete() to indicate to the abcc-api to continue.
**------------------------------------------------------------------------------
*/
void ABCC_API_CbfUserInit( ABCC_API_NetworkType iNetworkType, ABCC_API_FwVersionType iFirmwareVersion )
{
   printf( "\nABCC_API_CbfUserInit() entered.\n" );
   printf( " - Network type:     0x%X\n", iNetworkType );
   printf( " - Firmware version: %u.%u.%u\n", iFirmwareVersion.bMajor, iFirmwareVersion.bMinor, iFirmwareVersion.bBuild );
   printf( "Now calling ABCC_API_UserInitComplete() to progress from SETUP state to NW_INIT.\n\n" );
   ABCC_API_UserInitComplete();
   return;
}

/*------------------------------------------------------------------------------
** printReadableTime()
** Formats time from milliseconds to hours, minutes, seconds, milliseconds and
** prints it.
**------------------------------------------------------------------------------
*/
void printReadableTime( DWORD ms ) {
    uint32_t hours = ms / 3600000;  // 1 hour = 3600000 ms
    ms %= 3600000;
    uint32_t minutes = ms / 60000;  // 1 min = 60000 ms
    ms %= 60000;
    uint32_t seconds = ms / 1000;   // 1 sec = 1000 ms
    uint32_t milliseconds = ms % 1000;

    printf("%02uh %02um %02u.%03us", hours, minutes, seconds, milliseconds);
}

/*------------------------------------------------------------------------------
** main()
** Initializes the driver and runs the main loop.
**------------------------------------------------------------------------------
*/
int main( void )
{
   /*
   ** Note: Make sure the ABCC reset signal is kept low by the platform specific
   ** initialization to keep the ABCC module in reset until the driver releases
   ** it.
   */

   BOOL8          fQuit = FALSE;
   ABCC_ErrorCodeType eErrorCode = ABCC_EC_NO_ERROR;
   const UINT16   iSleepTimeMS = 10;
   DWORD          lThen, lNow, lDiff;

#if( _DEBUG )
   /*
   ** Set a hard-coded Path ID if this is a debug build. In those cases we
   ** probably want to use the same, and already known, Path ID for every run.
   ** Since possible Path IDs start at '1' the value '0' means "let the user
   ** select the path".
   **
   ** For more information about Microsoft's predefined macros see
   ** https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
   */
   TP_vSetPathId( 0 );
#endif


   printf( "-------------------------------------------------\n" );
   printf( "Program started at: " );
   printReadableTime( timeGetTime() );
   printf( "\n" );
   printf( "-------------------------------------------------\n" );
   printf( "HMS Networks\n" );
   printf( "Anybus CompactCom Starter Kit\n" );
   printf( "Windows example port\n\n" );
   printf( "Press 'Q' to quit.\n\n" );

   /*
   ** Function to initialize CompactCom-related systems.
   ** Note: This function in not required to call unless
   ** ABCC_HAL_HwInit() contain anything.
   */
   if( ABCC_API_Init() != ABCC_EC_NO_ERROR )
   {
      return( 0 );
   }

   lThen = timeGetTime();

#if ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS
   /*
   ** This test code sets the address and bit rate to hard coded values.
   ** The real implementation shall use switches or a configuration menu at the
   ** device itself.
   */
   printf( "Test code for hard coded Node Address and Bit rate used!\n" );
   ABCC_API_SetAddress( NODE_ADDRESS_TEST_VALUE );
   printf( "   Node Address value provided to API: %d\n", NODE_ADDRESS_TEST_VALUE );
   ABCC_API_SetBaudrate( BIT_RATE_TEST_VALUE );
   printf( "   Bit rate value provided to API: %d\n\n", BIT_RATE_TEST_VALUE );
#endif // ABCC_CFG_SET_FIELDBUS_TEST_ADDRESS



   while( !fQuit  )
   {
      /*
      ** Primary function start and drive the abcc-api.
      */
      eErrorCode = ABCC_API_Run();
      /*
      ** Handle potential error codes returned from the abcc-api here.
      */
      if( eErrorCode != ABCC_EC_NO_ERROR )
      {
         printf( "ABCC_API_Run() returned status code: %d\n", eErrorCode );
         fQuit = TRUE;
      }
      else
      {
         fQuit = RunUi();
      }

      lNow = timeGetTime();
      lDiff = lNow - lThen;
      if( lDiff > 0 )
      {
         /*
         ** Truncate intervals to 65535ms because 'ABCC_API_RunTimerSystem()'
         ** takes a UINT16.
         */
         if( lDiff > (UINT16)0xFFFF )
         {
            lDiff = (UINT16)0xFFFF;
         }

         /*
         ** Provide the abcc-api with a time base. Required for timers to function.
         */
         ABCC_API_RunTimerSystem( (UINT16)lDiff );
         lThen = lNow;
      }

      if( iSleepTimeMS > 0 )
      {
         Sleep( iSleepTimeMS );
      }
   }

   /*
   ** Shut down the abcc-api and the CompactCom.
   */
   ABCC_API_Shutdown();

   TP_Shutdown();

   if( eErrorCode != ABCC_EC_NO_ERROR )
   {
      printf( "Press any key to quit.\n" );
      while( !_kbhit() )
      {
         Sleep( 100 );
      }
   }

   return ( 0 );

} /* End of main() */
