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
bool RPCPubSub::connected = false;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*static*/
void RPCPubSub::initialisePubSub( TRPCFunctor *notifycb )
{
   static objAdded = false;
   if ( !objAdded )
   {
      objAdded = true;
      MinosRPCObj::addObj( new RPCSubscribeClient( 0 ) );
      MinosRPCObj::addObj( new RPCPublishClient( 0 ) );
      MinosRPCObj::addObj( new RPCNotifyServer( notifycb ) );
   }
}
void RPCPubSub::publish( const std::string &category, const std::string &key, const std::string &value, PublishState pState )
{
   RPCPublisher::testAndPublish( category, key, value, pState );
}

void RPCPubSub::subscribe( const std::string &category )
{
   RPCSubscriber::testAndSubscribe( category );
}
void RPCPubSub::subscribeRemote( const std::string &server, const std::string &category )
{
   RPCRemoteSubscriber::testAndSubscribe( server, category );
}
void RPCPubSub::reconnectPubSub( )
{
   // iterate the publish and subscribe objects and re-do them
   for ( std::vector<RPCPublisher *>::iterator i = publishList.begin(); i != publishList.end(); i++ )
   {
      if ( *i )
      {
         ( *i ) ->rePublish();
      }
   }
   for ( std::vector<RPCSubscriber *>::iterator i = subscribeList.begin(); i != subscribeList.end(); i++ )
   {
      if ( *i )
      {
         ( *i ) ->reSubscribe();
      }
   }
   connected = true;
}

void RPCPubSub::close( )
{
   for ( std::vector<RPCPublisher *>::iterator i = publishList.begin(); i != publishList.end(); i++ )
   {
      delete ( *i );
      ( *i ) = 0;
   }
   publishList.clear();
   for ( std::vector<RPCSubscriber *>::iterator i = subscribeList.begin(); i != subscribeList.end(); i++ )
   {
      delete ( *i );
      ( *i ) = 0;
   }
   subscribeList.clear();
}
//---------------------------------------------------------------------------
AnalysePubSubNotify::AnalysePubSubNotify( bool err, MinosRPCObj *mro ) :
      OK( false )
{
   if ( !err )
   {
      boost::shared_ptr<RPCParam> psServer;
      boost::shared_ptr<RPCParam> psCategory;
      boost::shared_ptr<RPCParam> psKey;
      boost::shared_ptr<RPCParam> psValue;
      boost::shared_ptr<RPCParam> psState;
      RPCArgs *args = mro->getCallArgs();
      if (
         args->getStructArgMember( 0, "Server", psServer )
         && args->getStructArgMember( 0, "Category", psCategory )
         && args->getStructArgMember( 0, "Key", psKey )
         && args->getStructArgMember( 0, "Value", psValue )
         && args->getStructArgMember( 0, "State", psState )
      )
      {
         int stemp;
         if (
            psServer->getString( server ) &&
            psCategory->getString( category ) &&
            psKey->getString( key ) &&
            psValue->getString( value ) &&
            psState->getInt( stemp )
         )
         {
            state = static_cast<PublishState>(stemp);
            OK = true;
         }
      }
   }
}

