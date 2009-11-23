/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include <vcl.h>
#include "minos_pch.h"
#pragma hdrstop
#include <inifiles.hpp>

#include "XMPPEvents.h"
#include "MServerZConf.h"
#include "MServerPubSub.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
MinosServer *MinosServer::singleton = 0;
/*static*/
MinosServer *MinosServer::getMinosServer()
{
   if ( !singleton )
   {
      singleton = new MinosServer();
   }
   return singleton;
}
//---------------------------------------------------------------------------

MinosServer::MinosServer() : serverName( DEFAULT_SERVER_NAME )
{
   TIniFile * config = new TIniFile( ".\\Configuration\\MinosConfig.ini" );
   String circleOfHell = config->ReadString( "Circle Of Hell", "Name", "No_name_in_config" ).Trim();
   serverName = circleOfHell.c_str();
   if ( serverName.size() == 0 )
   {
      char buff[ 256 ];
      if ( gethostname( buff, 255 ) == 0 )
         serverName = buff;
   }
   delete config;
}
MinosServer::~MinosServer()
{}
//---------------------------------------------------------------------------
bool MinosServer::forwardStanza( MinosCommonConnection *il, TiXmlElement *tix )
{
   // while we don't know our name, we can accept client connections, but NOT server connections
   bool res = false;
   std::string sto = getAttribute( tix, "to" );
   MinosId to( sto );

   if ( sto.size() && to.server.size() && ( stricmp( to.server.c_str(), "localhost" ) != 0 ) &&
        ( stricmp( to.server.c_str(), getServerName().c_str() ) != 0 ) )
   {
      res = false;        // not our host name in to
   }
   else
      if ( !sto.size() || to.user.size() == 0 )
      {
         // There is a "to" (with our server name??), or no "to" - which means us!
         res = analyseNode( il, tix );
      }
   return res;
}
//---------------------------------------------------------------------------
bool MinosServer::analyseNode( MinosCommonConnection *il, TiXmlElement *tix )
{
   // response factory - look at the node, and build the correct response stanza object

   // we need to sort out the "not implemented" bits here.
   // what do these stanzas mean to a service?
   // it need to be up to the individual service to see!

   MinosId from( getAttribute( tix, "from" ) );
   std::string subtype = getAttribute( tix, "type" );
   std::string id = getAttribute( tix, "id" );

   TiXmlElement *query = findNode( tix, "query" );
   std::string ns = getAttribute( query, "xmlns" );
   if ( ns != "minos:iq:rpc" )
   {
      return false;
   }

   TiXmlElement *mcall = findNode( query, "methodCall" );
   TiXmlElement *mresp = findNode( query, "methodResponse" );
   if ( subtype == "set" )
   {
      RPCRequest * xs = new RPCRequest( from.fullId, mcall );
      xs->setId( id.c_str() );
      dispatchStanza( il, xs );
      delete xs;
      return true;
   }
   else
      if ( subtype == "result" )
      {
         RPCResponse * rr = new RPCResponse( from.fullId, mresp );
         rr->setId( id.c_str() );
         dispatchStanza( il, rr );
         delete rr;
         return true;
      }
   return true;      // which may not be right?
}
//---------------------------------------------------------------------------
void MinosServer::dispatchStanza( MinosCommonConnection *il, RPCRequest *req )
{
   // really need to go to an RPC Object factory here
   logMessage( "XMPP", "RPCRequest received - " + req->methodName );

   if ( req->methodName == "ServerSetFromId" )
   {
      std::string from;
      if ( req->getStringArg( 0, from ) )
      {
         // check that from is what connected to us...
         MinosId id( from );

         std::string clientServerName = ( ( stricmp( id.server.c_str(), "localhost" ) == 0 ) ? serverName.c_str() : id.server.c_str() );
         std::string fid = std::string( id.user ) + "@" + clientServerName;

         MinosId id2( fid );

         il->setFromId( id2, req );

         RPCResponse * m = new RPCResponse( req->getFrom(), serverName, req->getId(), req->methodName );
         m->addParam( clientServerName );
         il->sendAction( m );
         delete m;
      }
   }
   else if ( req->methodName == "ClientSetFromId" )
   {
      std::string from;
      if ( req->getStringArg( 0, from ) )
      {
         // check that from is what connected to us...
         MinosId id( from );

         std::string clientServerName = ( ( stricmp( id.server.c_str(), "localhost" ) == 0 ) ? serverName.c_str() : id.server.c_str() );
         std::string fid = std::string( id.user ) + "@" + clientServerName;

         MinosId id2( fid );

         il->setFromId( id2, req );   // don't check the reply

         RPCResponse * m = new RPCResponse( req->getFrom(), serverName, req->getId(), req->methodName );
         m->addParam( clientServerName );
         il->sendAction( m );
         delete m;
      }
   }

   else if ( req->methodName == "Minos:PubSub:Publish"
        || req->methodName == "Minos:PubSub:ServerSubscribe"
        || req->methodName == "Minos:PubSub:RemoteSubscribe"
        || req->methodName == "Minos:PubSub:Subscribe"
        || req->methodName == "Minos:PubSub:ServerNotify"
        || req->methodName == "Minos:PubSub:ClientNotify" )
   {
      CsGuard g;
      makeXMPPEvent( req );
   }
}
//---------------------------------------------------------------------------
/*
void MinosServer::dispatchStanza( MinosConnection *il, RPCEvent *ev )
{
   // really need to go to an RPC Object factory here
   // if we have any for the server...
 
   delete ev;
}
*/ 
//---------------------------------------------------------------------------
void MinosServer::dispatchStanza( MinosCommonConnection * /*il*/, RPCResponse * /*resp*/ )
{
   // really need to go to an RPC Object factory here
   // response to (?our) RPC call
   logMessage( "XMPP", "RPCResponse received" );
}
//---------------------------------------------------------------------------

