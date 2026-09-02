/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
*/

// Note: This file must be compiled without precompiled headers.
// PCH may include <windows.h> before the WINVER/_WIN32_WINNT
// defines below.

#define WINVER 0x0500 // _WIN32_WINNT_WIN2K
#define _WIN32_WINNT 0x0500 // _WIN32_WINNT_WIN2K
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "tp.h"
#include "imp_tp.h"


TP_UserSelectPathType TP_UserSelectPath;
TP_UserSelectPathExtType TP_UserSelectPathExtInternal;
TP_SelectPathType TP_SelectPath;
TP_DestroyPathType TP_DestroyPath;

TP_PathNameType TP_PathName;
TP_PathNameWType TP_PathNameW;

TP_GetSupportedBaudRatesType TP_GetSupportedBaudRates;

TP_GetProviderHandleAndPathType TP_GetProviderHandleAndPath;

TP_ProviderSpecificCommandType TP_ProviderSpecificCommand;

TP_ParallelOpenType TP_ParallelOpen;
TP_ParallelCloseType TP_ParallelClose;
TP_ParallelReadType TP_ParallelRead;
TP_ParallelVerifyReadType TP_ParallelVerifyRead;
TP_ParallelWriteType TP_ParallelWrite;
TP_ParallelVerifyWriteType TP_ParallelVerifyWrite;

TP_SerialOpenType TP_SerialOpen;
TP_SerialCloseType TP_SerialClose;
TP_SerialOpenType TP_SerialReopen;
TP_SerialGetInAmountType TP_SerialGetInAmount;
TP_SerialGetOutAmountType TP_SerialGetOutAmount;
TP_SerialReadType TP_SerialRead;
TP_SerialWriteType TP_SerialWrite;

TP_SpiOpenType TP_SpiOpen;
TP_SpiCloseType TP_SpiClose;
TP_SpiTransactionType TP_SpiTransaction;


static HMODULE locRouterDll;


TP_StatusType WINAPI TP_Initialise( const char* aDllFileName, const UINT16 aVersion )
{
   if( locRouterDll )
   {
      return TP_ERR_OPEN;
   }

   locRouterDll = LoadLibraryA( aDllFileName );
   if( !locRouterDll )
   {
      return TP_ERR_WIN_FAIL;
   }

   TP_UserSelectPath = (TP_UserSelectPathType)GetProcAddress( locRouterDll, "TP_UserSelectPath" );
   TP_UserSelectPathExtInternal = (TP_UserSelectPathExtType)GetProcAddress( locRouterDll, "TP_UserSelectPathExt" );
   TP_SelectPath = (TP_SelectPathType)GetProcAddress( locRouterDll, "TP_SelectPath" );
   TP_DestroyPath = (TP_DestroyPathType)GetProcAddress( locRouterDll, "TP_DestroyPath" );

   TP_PathName = (TP_PathNameType)GetProcAddress( locRouterDll, "TP_PathName" );
   TP_PathNameW = (TP_PathNameWType)GetProcAddress( locRouterDll, "TP_PathNameW" );

   TP_GetSupportedBaudRates = (TP_GetSupportedBaudRatesType)GetProcAddress( locRouterDll, "TP_GetSupportedBaudRates" );

   TP_GetProviderHandleAndPath = (TP_GetProviderHandleAndPathType)GetProcAddress( locRouterDll, "TP_GetProviderHandleAndPath" );

   TP_ProviderSpecificCommand = (TP_ProviderSpecificCommandType)GetProcAddress( locRouterDll, "TP_ProviderSpecificCommand" );

   TP_ParallelOpen = (TP_ParallelOpenType)GetProcAddress( locRouterDll, "TP_ParallelOpen" );
   TP_ParallelClose = (TP_ParallelCloseType)GetProcAddress( locRouterDll, "TP_ParallelClose" );
   TP_ParallelRead = (TP_ParallelReadType)GetProcAddress( locRouterDll, "TP_ParallelRead" );
   TP_ParallelVerifyRead = (TP_ParallelVerifyReadType)GetProcAddress( locRouterDll, "TP_ParallelVerifyRead" );
   TP_ParallelWrite = (TP_ParallelWriteType)GetProcAddress( locRouterDll, "TP_ParallelWrite" );
   TP_ParallelVerifyWrite = (TP_ParallelVerifyWriteType)GetProcAddress( locRouterDll, "TP_ParallelVerifyWrite" );

   TP_SerialOpen = (TP_SerialOpenType)GetProcAddress( locRouterDll, "TP_SerialOpen" );
   TP_SerialClose = (TP_SerialCloseType)GetProcAddress( locRouterDll, "TP_SerialClose" );
   TP_SerialReopen = (TP_SerialOpenType)GetProcAddress( locRouterDll, "TP_SerialReopen" );
   TP_SerialGetInAmount = (TP_SerialGetInAmountType)GetProcAddress( locRouterDll, "TP_SerialGetInAmount" );
   TP_SerialGetOutAmount = (TP_SerialGetOutAmountType)GetProcAddress( locRouterDll, "TP_SerialGetOutAmount" );
   TP_SerialRead = (TP_SerialReadType)GetProcAddress( locRouterDll, "TP_SerialRead" );
   TP_SerialWrite = (TP_SerialWriteType)GetProcAddress( locRouterDll, "TP_SerialWrite" );

   TP_SpiOpen = (TP_SpiOpenType)GetProcAddress( locRouterDll, "TP_SpiOpen" );
   TP_SpiClose = (TP_SpiCloseType)GetProcAddress( locRouterDll, "TP_SpiClose" );
   TP_SpiTransaction = (TP_SpiTransactionType)GetProcAddress( locRouterDll, "TP_SpiTransaction" );

   if( !TP_UserSelectPath || !TP_SelectPath || !TP_DestroyPath || !TP_PathName || !TP_ProviderSpecificCommand || 
      !TP_ParallelOpen || !TP_ParallelClose || !TP_ParallelRead || !TP_ParallelVerifyRead || !TP_ParallelWrite || !TP_ParallelVerifyWrite ||
      !TP_SerialOpen || !TP_SerialClose || !TP_SerialGetInAmount || !TP_SerialGetOutAmount || !TP_SerialRead || !TP_SerialWrite )
   {
      TP_Close();
      return TP_ERR_WIN_FAIL;
   }

   if( aVersion >= 0x0102 ) // TP_GetProviderHandleAndPath added in 1.2
   {
      if( !TP_GetProviderHandleAndPath )
      {
         TP_Close();
         return TP_ERR_WIN_FAIL;
      }
   }

   if( aVersion >= 0x0200 ) // SPI, TP_UserSelectPathExt() and TP_PathNameW() added in 2.0
   {
      if( !TP_SpiOpen || !TP_SpiClose || !TP_SpiTransaction ||
         !TP_UserSelectPathExtInternal || !TP_PathNameW )
      {
         TP_Close();
         return TP_ERR_WIN_FAIL;
      }
   }

   if( aVersion >= 0x0201 ) // TP_GetSupportedBaudRates() added in 2.1
   {
      if( !TP_GetSupportedBaudRates )
      {
         TP_Close();
         return TP_ERR_WIN_FAIL;
      }
   }

   if( aVersion >= 0x0202 ) // TP_SerialReopen() added in 2.2
   {
      if( !TP_SerialReopen )
      {
         TP_Close();
         return TP_ERR_WIN_FAIL;
      }
   }

   return TP_ERR_NONE;
}


TP_StatusType WINAPI TP_Close( void )
{
   if( !locRouterDll )
   {
      return TP_ERR_NOT_OPEN;
   }

   TP_UserSelectPath = NULL;
   TP_UserSelectPathExtInternal = NULL;
   TP_SelectPath = NULL;
   TP_DestroyPath = NULL;
   TP_GetSupportedBaudRates = NULL;
   TP_GetProviderHandleAndPath = NULL;
   TP_PathName = NULL;
   TP_PathNameW = NULL;
   TP_ProviderSpecificCommand = NULL;
   TP_ParallelOpen = NULL;
   TP_ParallelClose = NULL;
   TP_ParallelRead = NULL;
   TP_ParallelVerifyRead = NULL;
   TP_ParallelWrite = NULL;
   TP_ParallelVerifyWrite = NULL;
   TP_SerialOpen = NULL;
   TP_SerialClose = NULL;
   TP_SerialReopen = NULL;
   TP_SerialGetInAmount = NULL;
   TP_SerialGetOutAmount = NULL;
   TP_SerialRead = NULL;
   TP_SerialWrite = NULL;
   TP_SpiOpen = NULL;
   TP_SpiClose = NULL;
   TP_SpiTransaction = NULL;

   FreeLibrary( locRouterDll );
   locRouterDll = NULL;

   return TP_ERR_NONE;
}


TP_StatusType WINAPI TP_UserSelectPathExt( TP_InterfaceType* anInterface, UINT32* aReturnPathId, TP_Path* aReturnPath, const char* aLabel )
{
   if( TP_UserSelectPathExtInternal )
   {
      return TP_UserSelectPathExtInternal( anInterface, aReturnPathId, aReturnPath, aLabel );
   }
   else
   {
      return TP_UserSelectPath( anInterface, aReturnPathId, aReturnPath ); // fallback to using the old interface with the default text label
   }
}


TP_StatusType TP_SerialReadBlocking( TP_Path aPath, UINT8* someData, UINT16 anAmount )
{
   UINT16 readOfs = 0;
   while( readOfs < anAmount )
   {
      UINT16 amount = anAmount - readOfs;

      TP_StatusType tpStat = TP_SerialRead( aPath, someData + readOfs, &amount, 1000 );
      if( tpStat != TP_ERR_NONE )
      {
         return tpStat;
      }

      readOfs += amount;
   }

   return TP_ERR_NONE;
}


TP_StatusType TP_SerialWriteBlocking( TP_Path aPath, const UINT8* someData, UINT16 anAmount )
{
   UINT16 sendOfs = 0;
   while( sendOfs < anAmount )
   {
      UINT16 amount = anAmount - sendOfs;

      TP_StatusType tpStat = TP_SerialWrite( aPath, someData + sendOfs, &amount, 1000 );
      if( tpStat != TP_ERR_NONE )
      {
         return tpStat;
      }

      sendOfs += amount;
   }

   return TP_ERR_NONE;
}
