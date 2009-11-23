/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef PubSubServerH
#define PubSubServerH
//---------------------------------------------------------------------------
class RPCServerPubSub : public RPCPubSub
{
   protected:
      RPCServerPubSub( const std::string &call, TRPCFunctor *cb ) : RPCPubSub( call, cb )
      {}
      virtual ~RPCServerPubSub()
      {}
      virtual RPCServerPubSub *makeObj() = 0;
   public:
      static void initialisePubSub( TRPCFunctor *notifycb );

      static void serverSubscribeRemote( const std::string &server, const std::string &category );
      static void serverReconnectRemotePubSub( const std::string &server );
      static void close( );
};
//---------------------------------------------------------------------------
// this is the server/server subscription
class RPCServerSubscriber : public RPCSubscriber
{
   private:
      std::string server;
      RPCServerSubscriber( const std::string &server, const std::string &category )
            : RPCSubscriber( category ), server( server )
      {}
   public:
      static void testAndSubscribe( const std::string &server, const std::string &category );
      virtual bool isRemoteEqual( const std::string &server, const std::string &category );
      virtual void serverReSubscribe();
      std::string getServer()
      {
         return server;
      }
};
extern std::vector<RPCServerSubscriber *> serverSubscribeList;
//---------------------------------------------------------------------------
class RPCServerSubscribeClient: public RPCPubSub
{
   public:
      RPCServerSubscribeClient( TRPCFunctor *cb ) : RPCPubSub( "Minos:PubSub:ServerSubscribe", cb )
      {}
      ~RPCServerSubscribeClient()
      {}
      virtual RPCServerSubscribeClient *makeObj()
      {
         return new RPCServerSubscribeClient( callback );
      }
};
class RPCRServerSubscribeServer: public MinosRPCServer
{
   public:
      RPCRServerSubscribeServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:PubSub:ServerSubscribe", cb )
      {}
      ~RPCRServerSubscribeServer()
      {}
      virtual RPCRServerSubscribeServer *makeObj()
      {
         return new RPCRServerSubscribeServer( callback );
      }

};
//---------------------------------------------------------------------------
class RPCPublishServer: public MinosRPCServer
{
   public:
      RPCPublishServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:PubSub:Publish", cb )
      {}
      ~RPCPublishServer()
      {}
      virtual RPCPublishServer *makeObj()
      {
         return new RPCPublishServer( callback );
      }
};
class RPCSubscribeServer: public MinosRPCServer
{
   public:
      RPCSubscribeServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:PubSub:Subscribe", cb )
      {}
      ~RPCSubscribeServer()
      {}
      virtual RPCSubscribeServer *makeObj()
      {
         return new RPCSubscribeServer( callback );
      }

};
//---------------------------------------------------------------------------
class RPCRemoteSubscribeServer: public MinosRPCServer
{
   public:
      RPCRemoteSubscribeServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:PubSub:RemoteSubscribe", cb )
      {}
      ~RPCRemoteSubscribeServer()
      {}
      virtual RPCRemoteSubscribeServer *makeObj()
      {
         return new RPCRemoteSubscribeServer( callback );
      }

};


//---------------------------------------------------------------------------
// Notify should be an event, not a call

class RPCClientNotifyClient: public MinosRPCClient
{
   public:
      RPCClientNotifyClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:PubSub:ClientNotify", cb )
      {} // base class "callback" gets set to cb
      ~RPCClientNotifyClient()
      {}
      virtual RPCClientNotifyClient *makeObj()
      {
         return new RPCClientNotifyClient( callback );
      }
};

//---------------------------------------------------------------------------
// Notify should be an event, not a call

class RPCServerNotifyClient: public MinosRPCClient
{
   public:
      RPCServerNotifyClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:PubSub:ServerNotify", cb )
      {} // base class "callback" gets set to cb
      ~RPCServerNotifyClient()
      {}
      virtual RPCServerNotifyClient *makeObj()
      {
         return new RPCServerNotifyClient( callback );
      }
};
class RPCServerNotifyServer: public MinosRPCServer
{
   public:
      RPCServerNotifyServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:PubSub:ServerNotify", cb )
      {} // base class "callback" gets set to cb
      ~RPCServerNotifyServer()
      {}
      virtual RPCServerNotifyServer *makeObj()
      {
         return new RPCServerNotifyServer( callback );
      }
};
//---------------------------------------------------------------------------
#endif
