/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "MinosVK_pch.h"
#pragma hdrstop

#include "ServerEvent.h"
#include "GJVThreads.h"
#include "LogEvents.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"

#include "XMPPRPCObj.h"
#include "KeyerRPCObj.h"
#include "RPCPubSub.h"
#include "KeyerRPCServer.h"

#include "keyctrl.h"

#pragma package(smart_init)

KeyerServer *KS = 0;
//---------------------------------------------------------------------------
KeyerServer::KeyerServer() : connected( false ), subscribed( false )
{}
//---------------------------------------------------------------------------
KeyerServer::~KeyerServer()
{
   MinosRPCObj::clearRPCObjects();
   XMPPClosedown();
}
//---------------------------------------------------------------------------
void KeyerServer::doCheckConnection()
{
   if ( !connected && checkServerReady() )
   {
      MinosRPCObj::addObj( new RPCKeyerControlServer( new TRPCCallback <KeyerServer> ( KS, &KeyerServer::keyerCallback ) ) );
      XMPPInitialise( "Keyer" );
      connected = true;
   }
}
/*static*/void KeyerServer::checkConnection()
{
   if ( !KS )
      KS = new KeyerServer;
   KS->doCheckConnection();
}
//---------------------------------------------------------------------------
void KeyerServer::doPublishState( const std::string &state )
{
   checkConnection();
   if ( connected && !subscribed )
   {
      RPCPubSub::initialisePubSub( new TRPCCallback <KeyerServer> ( KS, &KeyerServer::notifyCallback ) );
      RPCPubSub::subscribe( "LineControl" );
      subscribed = true;
   }
   if ( connected && subscribed )
   {
      static std::string old;

      if ( state != old )
      {
         old = state;
         RPCPubSub::publish( "Keyer", "Report", state, psPublished );
      }
   }
}
/*static*/void KeyerServer::publishState(  const std::string &state )
{
   if ( !KS )
      KS = new KeyerServer;
   KS->doPublishState( state );
}
//---------------------------------------------------------------------------
void KeyerServer::doPublishCommand( const std::string &cmd )
{
   checkConnection();
   if ( connected && !subscribed )
   {
      RPCPubSub::initialisePubSub( new TRPCCallback <KeyerServer> ( KS, &KeyerServer::notifyCallback ) );
      RPCPubSub::subscribe( "LineControl" );
      subscribed = true;
   }
   if ( connected && subscribed )
   {
      static std::string old;

      if ( cmd != old )
      {
         old = cmd;
         RPCPubSub::publish( "Keyer", "Command", cmd, psPublished );
      }
   }
}
/*static*/void KeyerServer::publishCommand(  const std::string &cmd )
{
   if ( !KS )
      KS = new KeyerServer;
   KS->doPublishCommand( cmd );
}
//---------------------------------------------------------------------------
void KeyerServer::keyerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "Keyer callback from " + from + ( err ? ":Error" : ":Normal" ) );

   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      boost::shared_ptr<RPCParam>piValue;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) && args->getStructArgMember( 0, "Value", piValue ) )
      {
         std::string Name;
         int Value;

         if ( psName->getString( Name ) && piValue->getInt( Value ) )
         {

            bool callOK = false;
            if ( Value >= 1 && Value <= 12 )
            {
               if ( Name == "PlayFile" )
               {
                  playKeyerFile( Value, true );    // do actual transmit, and repeat as required
                  callOK = true;
               }
               else
                  if ( Name == "RecordFile" )
                  {
                     startRecordDVPFile( Value );
                     callOK = true;
                  }
            }

            mro->clearCallArgs();
            boost::shared_ptr<RPCParam>st(new RPCParamStruct);
            if ( callOK )
            {
               st->addMember( true, "KeyerResult" );
               mro->getCallArgs() ->addParam( st );
               mro->queueResponse( from );
            }
            else
            {
               st->addMember( "RPC error", "KeyerResult" );
               mro->getCallArgs() ->addParam( st );
               mro->queueErrorResponse( from );

            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void KeyerServer::notifyCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "Notify callback from " + from + ( err ? ":Error" : ":Normal" ) );
   AnalysePubSubNotify an( err, mro );

   // called whenever line changes
   if ( an.getOK() )
   {
      if ( an.getCategory() == "LineControl" )
      {
         lineStates[ an.getKey() ] = ( ( an.getValue() == "set" ) ? true : false );
         trace( "LineControl" + an.getKey() + ":" + an.getValue() );
      }
   }
}
//---------------------------------------------------------------------------
bool KeyerServer::getState( const std::string &line )
{
   std::map<std::string, bool>::iterator l = lineStates.find( line );
   if ( l != lineStates.end() )
   {
      return ( *l ).second;
   }
   return false;
}

