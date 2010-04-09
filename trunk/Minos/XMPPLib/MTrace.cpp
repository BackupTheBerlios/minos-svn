/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#include "XMPP_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------
static MLogFile mLogFile;
static bool logEnabled = false;

void trace( char *mess )
{
   mLogFile.log( mess );
   //   std::cout << mess << std::endl;
}
void trace( const std::string & mess )
{
   mLogFile.log( mess );
   //   std::cout << mess.c_str() << std::endl;
}
//---------------------------------------------------------------------------
void enableTrace( const std::string &where )
{
   if ( !logEnabled )
   {
      String path = GetCurrentDir();
      if ( where[ 0 ] == '.' )
      {
         path += where.substr( 1, where.size() - 1 ).c_str();
      }
      else
      {
         path += where.c_str();
      }
      mLogFile.createLogFile( AnsiString(path).c_str(), 10 );
      logEnabled = true;
   }
}
void disableTrace( )
{
   if ( logEnabled )
   {
      mLogFile.close();
      logEnabled = false;
   }
}
//---------------------------------------------------------------------------
std::string lastError( DWORD erno )
{
   LPVOID lpMsgBuf;

   FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      erno,
      MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),    // Default language
      ( LPTSTR ) & lpMsgBuf,
      0,
      NULL
   );
   std::string s;
   s = reinterpret_cast<const char*>( lpMsgBuf );

   // Free the buffer.
   LocalFree( lpMsgBuf );
   return s;
}
std::string lastError( void )
{
   return lastError( GetLastError() );
}
std::string getTraceFileName()
{
   return mLogFile.getTraceFileName();
}
//---------------------------------------------------------------------------

