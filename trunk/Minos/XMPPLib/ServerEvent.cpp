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
#include <Registry.hpp> 
//---------------------------------------------------------------------------

#pragma package(smart_init)

static HANDLE serverEvent = 0;

std::string makeUuid()
{
   // if there isn't one, then make one
   GUID uuid;
   CoCreateGuid( &uuid );

   unsigned char *pid = 0;
   UuidToString( &uuid, &pid );
   std::string myUuid = ( char * ) pid;
   RpcStringFree( &pid );

   return myUuid;
}
std::string getServerId()
{
   const char * key = "ServerUUID";
   // First, look in the registry for a GUID for us
   static std::string myUuid;
   if ( myUuid.size( ) )
   {
      return myUuid;
   }
   TRegistry *Reg = new TRegistry;
   Reg->RootKey = HKEY_CURRENT_USER;
   if ( Reg->OpenKeyReadOnly( "SOFTWARE\\G0GJV\\Minos\\Server" ) )
   {
      myUuid = Reg->ReadString( key ).c_str();
   }
   if ( myUuid.size() == 0 )
   {
      // if there isn't one, then make one
      myUuid = makeUuid();
      Reg->CloseKey();
      if ( Reg->OpenKey( "SOFTWARE\\G0GJV\\Minos\\Server", true ) )
      {
         Reg->WriteString( key, myUuid.c_str() );
      }
      else
      {
         myUuid = "MinosServer"; // as we cannot write it, use something constant
      }
   }
   Reg->CloseKey();
   delete Reg;
   return myUuid ;
}

HANDLE makeServerEvent( bool create )
{
   static std::string serverEventName = getServerId();
   if ( create )
   {
      if ( !serverEvent )
      {
         // we should set up th ACLs etc so we could run serevr as a service
         serverEvent = CreateEvent( 0, TRUE, FALSE, serverEventName.c_str() ); // Named Manual reset
      }
   }
   else
   {
      if ( serverEvent )
      {
         CloseHandle( serverEvent );
         serverEvent = 0;
      }
   }
   return serverEvent;
}

bool checkServerReady()
{
   static std::string serverEventName = getServerId();

   bool ret = false;
   HANDLE serverEvent = CreateEvent( 0, TRUE, FALSE, serverEventName.c_str() ); // Named Manual reset
   if ( serverEvent )
   {
      if ( GetLastError() == ERROR_ALREADY_EXISTS )
      {
         ret = true;
      }
      CloseHandle( serverEvent );
   }
   return ret;
}
