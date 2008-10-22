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

bool baseRunApp( const std::string &commandLine, const std::string &params, const std::string &rundir, std::string &outfname, bool wait, bool minimise )
{
   STARTUPINFO StartInfo;

   StartInfo.hStdInput = 0;
   StartInfo.hStdOutput = 0;
   StartInfo.hStdError = 0;

   char appFName[ 255 ];
   int nLen = 0;
   nLen = ::GetModuleFileName( HInstance, appFName, 255 );
   appFName[ nLen ] = '\0';

   String fname = ExtractFilePath( appFName );
   fname += "TraceLog\\Runapp_";
   String progname = ExtractFileName( commandLine.c_str() );
   progname = progname.SubString( 1, progname.Pos( "." ) - 1 );
	std::string outFileName = ( fname + progname + ".log" ).t_str() ;

   // use the system defined TEMP directory
   std::string inFileName = "%TEMP%\\NullInput.txt";
   char cline[ 1024 ];
   DWORD cnt = ExpandEnvironmentStrings( inFileName.c_str(), cline, 1023 );
   if ( !cnt )
   {
		logMessage( std::string( fname.t_str() ), std::string( "Error Expanding Environment Strings in " ) + inFileName + lastError() );
      outfname = outFileName.c_str();
      return false;
   }
   cline[ cnt ] = 0;
   inFileName = cline;

   PROCESS_INFORMATION ProcessInformation;
   memset( &StartInfo, 0, sizeof( STARTUPINFO ) );
   StartInfo.cb = sizeof( STARTUPINFO );
   StartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
   StartInfo.wShowWindow = ( minimise ? SW_MINIMIZE : SW_NORMAL );

   SECURITY_ATTRIBUTES outsa;
   outsa.nLength = sizeof( outsa );
   outsa.lpSecurityDescriptor = 0;
   outsa.bInheritHandle = TRUE;

   StartInfo.hStdOutput = CreateFile( outFileName.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, &outsa,
                                      CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0 );

   if ( StartInfo.hStdOutput == INVALID_HANDLE_VALUE )
   {
		logMessage( std::string( fname.t_str() ), std::string( "Error opening output file " ) + outFileName.c_str() + " : " + lastError() );
		logMessage( std::string( fname.t_str() ), std::string( "Current directory is " ) + GetCurrentDir().t_str() );
      outfname = outFileName.c_str();
      return false;
   }
	logMessage( std::string( fname.t_str() ), std::string( "Output file is " ) + outFileName );
   SECURITY_ATTRIBUTES insa;
   insa.nLength = sizeof( insa );
   insa.lpSecurityDescriptor = 0;
   insa.bInheritHandle = TRUE;

   StartInfo.hStdInput = CreateFile( inFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, &insa,
                                     OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

   if ( StartInfo.hStdInput == INVALID_HANDLE_VALUE )
   {
		logMessage( std::string( fname.t_str() ), std::string( "Error opening input file " ) + inFileName.c_str() + " : " + lastError() );
		logMessage( std::string( fname.t_str() ), std::string( "Current directory is " ) + GetCurrentDir().t_str() );
      outfname = outFileName.c_str();
      return false;
   }

   // StdErr is a duplicate of StdOut

   BOOL cret = DuplicateHandle(
                  GetCurrentProcess(),   	// handle to process with handle to duplicate
                  StartInfo.hStdOutput,   	// handle to duplicate
                  GetCurrentProcess(),   	// handle to process to duplicate to
                  &StartInfo.hStdError,   	// pointer to duplicate handle
                  0,   	// access for duplicate handle
                  TRUE,   	// handle inheritance flag
                  DUPLICATE_SAME_ACCESS 	// optional actions
               );
   if ( !cret )
   {
		logMessage( std::string( fname.t_str() ), std::string( "Error duplicating handle for StdErr : " ) + lastError() );
      outfname = outFileName.c_str();
      return false;
   }

   std::string cmdl = commandLine;
   if ( params.size() )
   {
      cmdl += " " + params;
   }
   cnt = ExpandEnvironmentStrings( cmdl.c_str(), cline, 1023 );
   if ( !cnt )
   {
		logMessage( std::string( fname.t_str() ), std::string( "Error Expanding Environment Strings in " ) + cmdl + lastError() );
      outfname = outFileName.c_str();
      return false;
   }
   cline[ cnt ] = 0;

   // All of our handles get passed on.

   cret = CreateProcess(
             0,    						// pointer to name of executable module
             cline,   					// pointer to command line string - NOT CONST!
             0,   							// pointer to process security attributes
             0,   							// pointer to thread security attributes

             TRUE,   						// handle inheritance flag - must be true to assign even the standard handles

             CREATE_DEFAULT_ERROR_MODE,   	// creation flags
             0,   							// pointer to new environment block
             ( rundir.size() ? rundir.c_str() : 0 ),  		// pointer to current directory name
             &StartInfo,   				// pointer to STARTUPINFO
             &ProcessInformation 	// pointer to PROCESS_INFORMATION
          );

   // close all the handles - they are now given to the child process
   if ( StartInfo.hStdInput != INVALID_HANDLE_VALUE )
      CloseHandle( StartInfo.hStdInput );
   if ( StartInfo.hStdError != INVALID_HANDLE_VALUE )
      CloseHandle( StartInfo.hStdError );
   if ( StartInfo.hStdOutput != INVALID_HANDLE_VALUE )
      CloseHandle( StartInfo.hStdOutput );

   if ( !cret )
   {
		logMessage( std::string( fname.t_str() ), std::string( "Error running <" ) + commandLine + "> : " + lastError() );
      outfname = outFileName.c_str();
      return false;
   }

   DWORD exitstat = 1;
   if ( wait )
   {
      // we need to look at a closedown flag as well...
      WaitForSingleObject( ProcessInformation.hProcess, INFINITE );

      if ( !GetExitCodeProcess( ProcessInformation.hProcess, &exitstat ) )
      {
         logMessage( std::string( outfname ), std::string( "Unable to get process exit status" ) + lastError() );
      }

   }

   CloseHandle( ProcessInformation.hThread );
   CloseHandle( ProcessInformation.hProcess );

   if ( wait )
   {
      logMessage( std::string( fname.t_str() ), std::string( "Exit status was " ) + makeStr( exitstat ) );
   }
   outfname = outFileName.c_str();
   return true;
}

