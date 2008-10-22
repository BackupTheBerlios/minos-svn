/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "minos_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
GJV_thread *clientThread = 0;
MinosClientListener *MinosClientListener::MCL = 0;
//---------------------------------------------------------------------------
void runClientThread( void * )
{
   WSAGuard guardian;

   if ( MinosClientListener::getListener() )
      return ;
   try
   {
      /*
         First open listener for Clients to connect to
      */
      new MinosClientListener();
      MinosClientListener::getListener() ->initialise( "Client", ClientPort );
   }
   catch ( Exception & e )
   {
		logMessage( "Exception", std::string( "Exception while setting up Client listener: " ) + e.Message.t_str() );
      return ;
   }
   try
   {
      makeServerEvent( true );
      while ( !closeApp )
      {
         MinosClientListener::getListener() ->processSockets();
      }
      makeServerEvent( false );
   }
   catch ( Exception & e )
   {
		logMessage( "Exception", std::string( "Exception while running Client listener: " ) + e.Message.t_str() );
      return ;
   }
   try
   {
      //      MinosClientListener::getListener() ->clearSockets();
      delete MinosClientListener::getListener();
   }
   catch ( Exception & e )
   {
      logMessage( "Exception", std::string( "Exception while closing Client listener: " ) + e.Message.t_str() );
      return ;
   }

}
//==============================================================================
void MinosClientListener::process()
{
   newConnection = acceptFreeSlot( new MinosClientConnection() );
}
//==============================================================================
bool MinosClientListener::sendClient( MinosCommonConnection *il, TiXmlElement *tix )
{
   MinosId from( getAttribute( tix, "from" ) );
   MinosId to( getAttribute( tix, "to" ) );

   if ( to.user.size() == 0 )
   {
      // invalid to servername (still default) or no "to" user
      return false;
   }
   bool fromLocal = false;
   bool fromLocalHost = false;
   bool toLocal = false;
   bool toLocalHost = false;

   bool addressOK = false;

   if ( from.empty() || stricmp( from.server.c_str(), MinosServer::getMinosServer() ->getServerName().c_str() ) == 0 )
      fromLocal = true;

   if ( from.empty() && stricmp( from.server.c_str(), DEFAULT_SERVER_NAME ) == 0 )
      fromLocalHost = true;

   if ( stricmp( to.server.c_str(), MinosServer::getMinosServer() ->getServerName().c_str() ) == 0 )
      toLocal = true;

   if ( stricmp( to.server.c_str(), DEFAULT_SERVER_NAME ) == 0 )
      toLocalHost = true;

   if ( toLocal )
      addressOK = true;      // proper "to" server, from anywhere

   if ( ( fromLocal || fromLocalHost ) && toLocalHost )      // "to" localhost OK if "from" local app
      addressOK = true;

   if ( fromLocal && !toLocal && !toLocalHost )      // from proper address, to remote address
      addressOK = true;

   if ( addressOK && !toLocal && !toLocalHost )
      return false;

   /* BUT (and we don't need to check...
   if (fromLocalHost && !(toLocal || toLocalHost))
      addressOK = false;
   */

   if ( !addressOK )
   {
      if ( il )
         il->sendError( tix, "cancel", "invalid-addressing" );  // deletes pak
      return true;   // don't pass it on - its rubbish
   }

   // OK, it is for what might be one of our clients

   for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
   {
      // worry about the details
      if ( ( *i ) ->checkUser( to ) )
      {
         if ( !( *i ) ->tryForwardStanza( tix ) )
         {
            // send failed; stash the message
            // (but some stanza types should be ignored?)
            break;
         }
         return true;
      }
   }
   // client is not connected; we have to ignore it (send an error)

   if ( il )
      il->sendError( tix, "cancel", "item-not-found" );
   return true;   // don't pass it on - either we have dealt with it, or its rubbish
}
bool MinosClientListener::checkStillClientConnection( const std::string &s )
{
   MinosId id( s );
   for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
   {
      // worry about the details
      if ( ( *i ) ->checkUser( id ) )
      {
         return true;
      }
   }
   return false;
}
//==============================================================================
//==============================================================================

MinosClientConnection::MinosClientConnection()
{
   txConnection = true;
}
bool MinosClientConnection::initialise()
{
   return true;
}

MinosClientConnection::~MinosClientConnection()
{
   logMessage( "Client Link", "Closing" );
   // here we need to revoke all of this clients published keys
   PubSubMain->revokeClient(makeJid());
}
//==============================================================================
bool MinosClientConnection::checkFrom( TiXmlElement *tix )
{
   if ( !fromIdSet )
   {
      return true;
   }
   // if no "from" insert one

   const char *sfrom = tix->Attribute( "from" );
   if ( !sfrom )
   {
      // insert a from of ourselves

      std::string from = makeJid();
      if ( from.size() )
      {
         tix->SetAttribute( "from", from.c_str() );
      }
   }
   else
   {
      MinosId from( sfrom );
      if ( !checkServer( from ) )
         return false;

      if ( !checkUser( from ) )
         return false;
   }
   return true;
}
//==============================================================================
bool MinosClientConnection::setFromId( MinosId &from, RPCRequest */*req*/ )
{
   // this should always be a local client
   clientServer = from.server;
   clientUser = from.user;
   fromIdSet = true;

   assert(CheckLocalConnection());
   return true;
}
//==============================================================================

