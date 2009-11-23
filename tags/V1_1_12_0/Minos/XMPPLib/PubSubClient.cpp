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
std::vector<RPCSubscriber *> subscribeList;
std::vector<RPCPublisher *> publishList;

bool RPCSubscriber::isEqual( const std::string &pcategory )
{
   return ( pcategory == category );
}
bool RPCSubscriber::isRemoteEqual( const std::string &/*pserver*/, const std::string &/*pcategory*/ )
{
   return false;
}
void RPCSubscriber::testAndSubscribe( const std::string &category )
{
   RPCSubscriber * sub = 0;
   for ( std::vector<RPCSubscriber *>::iterator i = subscribeList.begin(); i != subscribeList.end(); i++ )
   {
      if ( ( *i ) ->isEqual( category ) )
      {
         sub = ( *i );
         break;
      }
   }
   if ( !sub )
   {
      sub = new RPCSubscriber( category );
      subscribeList.push_back( sub );
   }
   if ( RPCPubSub::isConnected() )
   {
      sub->reSubscribe();
   }
}
bool RPCRemoteSubscriber::isRemoteEqual( const std::string &pServer, const std::string &category )
{
   return server == pServer && isEqual( category );
}
void RPCRemoteSubscriber::testAndSubscribe( const std::string &server, const std::string &category )
{
   RPCRemoteSubscriber * sub = 0;
   for ( std::vector<RPCSubscriber *>::iterator i = subscribeList.begin(); i != subscribeList.end(); i++ )
   {
      if ( ( *i ) ->isRemoteEqual( server, category ) )
      {
         sub = dynamic_cast<RPCRemoteSubscriber *>( *i );
         if (sub)
         {
            break;
         }
      }
   }
   if ( !sub )
   {
      sub = new RPCRemoteSubscriber( server, category );
      subscribeList.push_back( sub );
   }
   if ( RPCPubSub::isConnected() )
   {
      sub->reSubscribe();
   }
}
void RPCSubscriber::reSubscribe()
{
   RPCSubscribeClient rsc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sServer(new RPCStringParam( "localhost" ));
   boost::shared_ptr<RPCParam>sCat(new RPCStringParam( category ));
   st->addMember( sServer, "Server" );
   st->addMember( sCat, "Category" );
   rsc.getCallArgs() ->addParam( st );
   rsc.queueCall( "localhost" );
}
// client resubscribing to remote event
void RPCRemoteSubscriber::reSubscribe()
{
   RPCRemoteSubscribeClient rsc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sServer(new RPCStringParam( server ));
   boost::shared_ptr<RPCParam>sCat(new RPCStringParam( category ));
   st->addMember( sServer, "Server" );
   st->addMember( sCat, "Category" );
   rsc.getCallArgs() ->addParam( st );
   rsc.queueCall( "localhost" );       // localhost just causes the server to loop
}

void RPCPublisher::testAndPublish( const std::string &category, const std::string &key, const std::string &value, PublishState pState )
{
   RPCPublisher * pub = 0;
   for ( std::vector<RPCPublisher *>::iterator i = publishList.begin(); i != publishList.end(); i++ )
   {
      if ( ( *i ) ->category == category && ( *i ) ->key == key )
      {
         pub = ( *i );
      }
   }
   if ( !pub )
   {
      pub = new RPCPublisher( category, key, value, pState );
      publishList.push_back( pub );
   }
   else
   {
      pub->value = value;
      pub->state = pState;
   }
   if ( RPCPubSub::isConnected() )
   {
      pub->rePublish();
   }
}
void RPCPublisher::rePublish()
{
   RPCPublishClient rpc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sCat(new RPCStringParam( category ));
   boost::shared_ptr<RPCParam>sKey(new RPCStringParam( key ));
   boost::shared_ptr<RPCParam>sValue(new RPCStringParam( value ));
   boost::shared_ptr<RPCParam>sState(new RPCIntParam( state ));
   st->addMember( sCat, "Category" );
   st->addMember( sKey, "Key" );
   st->addMember( sValue, "Value" );
   st->addMember( sState, "State" );
   rpc.getCallArgs() ->addParam( st );
   rpc.queueCall( "localhost" );
}

//---------------------------------------------------------------------------

