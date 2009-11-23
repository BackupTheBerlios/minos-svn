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
//==============================================================================
WSAGuard::WSAGuard( void )
{
   // class to ensure that WinSock is opened and closed correctly
   WSADATA wsaData;
   const int major = 1;
   const int minor = 1;

   WORD wVersionRequested = MAKEWORD( major, minor );
   volatile int WSAerr = WSAStartup( wVersionRequested, &wsaData );
   if ( WSAerr != 0 )
   {
      //      std::string csText = "Unable to initialise Winsock.DLL : error no ";
      //      csText += string_cast( WSAerr );
      //      ::logMessage(iKernelBaseAPI::iKernelAPIFramework,  csText );
      exit( 1 );
   }
   else
   {

      if ( LOBYTE( wsaData.wVersion ) != major || HIBYTE( wsaData.wVersion ) != minor )
      {
         //         const char * csText = "This Winsock.DLL doesn't support this application";
         //         ::logMessage(iKernelBaseAPI::iKernelAPIFramework,  csText );
         exit( 1 );
      }
   }
}
//---------------------------------------------------------------------------

WSAGuard::~WSAGuard()
{
   WSACleanup();
}

//==============================================================================

