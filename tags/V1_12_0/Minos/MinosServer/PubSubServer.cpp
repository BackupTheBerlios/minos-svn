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

std::vector<RPCServerSubscriber *> serverSubscribeList;

/*static*/
void RPCServerPubSub::initialisePubSub( TRPCFunctor *notifycb )
{
   static objAdded = false;
   if ( !objAdded )
   {
      RPCPubSub::initialisePubSub( notifycb );
      objAdded = true;
   }
}
void RPCServerPubSub::serverSubscribeRemote( const std::string &server, const std::string &category )
{
   RPCServerSubscriber::testAndSubscribe( server, category );
}
void RPCServerPubSub::serverReconnectRemotePubSub( const std::string &server )
{
   for ( std::vector<RPCServerSubscriber *>::iterator i = serverSubscribeList.begin(); i != serverSubscribeList.end(); i++ )
   {
      if ( ( *i ) && ( *i ) ->getServer() == server )
      {
         ( *i ) ->serverReSubscribe();
      }
   }
   connected = true;
}
void RPCServerPubSub::close( )
{
   RPCPubSub::close( );
   for ( std::vector<RPCServerSubscriber *>::iterator i = serverSubscribeList.begin(); i != serverSubscribeList.end(); i++ )
   {
      delete ( *i );
      ( *i ) = 0;
   }
   serverSubscribeList.clear();
}

//==================================================================================
bool RPCServerSubscriber::isRemoteEqual( const std::string &pServer, const std::string &category )
{
   return server == pServer && isEqual( category );
}
void RPCServerSubscriber::testAndSubscribe( const std::string &server, const std::string &category )
{
   RPCServerSubscriber * sub = 0;
   for ( std::vector<RPCServerSubscriber *>::iterator i = serverSubscribeList.begin(); i != serverSubscribeList.end(); i++ )
   {
      if ( ( *i ) ->isRemoteEqual( server, category ) )
      {
         sub = ( *i );
      }
   }
   if ( !sub )
   {
      sub = new RPCServerSubscriber( server, category );
      serverSubscribeList.push_back( sub );
   }
   //if ( RPCPubSub::isConnected() )
   {
      sub->serverReSubscribe();
   }
}
// causes the server to resubscribe to the remote server
void RPCServerSubscriber::serverReSubscribe()
{
   RPCServerSubscribeClient rsc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sServer(new RPCStringParam( server ));
   boost::shared_ptr<RPCParam>sCat(new RPCStringParam( category ));
   st->addMember( sServer, "Server" );
   st->addMember( sCat, "Category" );
   rsc.getCallArgs() ->addParam( st );
   rsc.queueCall( server );       // localhost just causes the server to loop
}

