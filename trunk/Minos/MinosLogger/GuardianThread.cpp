/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "base_pch.h"
#pragma hdrstop

#include <tlhelp32.h>
#include "ConfigDM.h"
#include "GuardianThread.h"

#pragma package(smart_init) 
//============================================================================
bool TGuardianThread::runApp( bool wait )
{
   return baseRunApp( ce->commandLine, ce->params, ce->rundir, ce->outfname, wait, ce->minimise, ce->hide );
}
//---------------------------------------------------------------------------
void TGuardianThread::runDirectApp( bool wait )
{
   logMessage( std::string( "Guardian" ), std::string( "[" ) + ce->name.c_str() + "] " + "Running  : " + ce->commandLine );
   bool ret = runApp( wait );
   if ( ret )
      logMessage( std::string( "Guardian" ), std::string( "[" ) + ce->name.c_str() + "] " + "Program <" + ce->commandLine + "> logged in " + ce->outfname );
}
//---------------------------------------------------------------------------
DWORD TGuardianThread::FindProcess( String ptf )
{
   DWORD ProcessId = 0;
   String ProcessToFind = ptf.SubString( 1, 15 ).Trim().LowerCase(); // max 15 char names

   HANDLE hProcessSnap = NULL;
   PROCESSENTRY32 pe32 = {0};

   //  Take a snapshot of all processes in the system.

   hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

   if ( hProcessSnap == ( HANDLE ) - 1 )
      return 0;

   //  Fill in the size of the structure before using it.

   pe32.dwSize = sizeof( PROCESSENTRY32 );

   //  Walk the snapshot of the processes, and for each process,
   //  display information.

   if ( Process32First( hProcessSnap, &pe32 ) )
   {
      do
      {
         HANDLE hProcess;

         // Get the actual priority class.
         hProcess = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
         CloseHandle ( hProcess );
         String mname = ExtractFileName( pe32.szExeFile ).SubString( 1, 15 ).LowerCase();
         if ( mname == ProcessToFind )
         {
            ProcessId = pe32.th32ProcessID;
            break;
         }
      }
      while ( Process32Next( hProcessSnap, &pe32 ) );
   }

   // Do not forget to clean up the snapshot object.

   CloseHandle ( hProcessSnap );


   return ProcessId;
}
void TGuardianThread::execute()
{
   if ( ce->run )
   {
      runDirectApp( true );
   }
}
//---------------------------------------------------------------------------

TGuardianThread::TGuardianThread( TConfigElement *ce )
      : ce( ce )
{}
//---------------------------------------------------------------------------

