/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef RPCPubSubH
#define RPCPubSubH 
//---------------------------------------------------------------------------
enum PublishState {psPublished, psRevoked, psNotConnected};

class RPCPubSub : public MinosRPCClient
{
   protected:
      static bool connected;
      RPCPubSub( const std::string &call, TRPCFunctor *cb ) : MinosRPCClient( call, cb )
      {}
      virtual ~RPCPubSub()
      {}
      virtual RPCPubSub *makeObj() = 0;
   public:
      static bool isConnected()
      {
         return connected;
      }

      static void subscribe( const std::string &category );
      static void subscribeRemote( const std::string &server, const std::string &category );

      static void publish( const std::string &category, const std::string &key, const std::string &value, PublishState pState );

      static void initialisePubSub( TRPCFunctor *notifycb );
      static void reconnectPubSub( );
      static void close( );
};

class AnalysePubSubNotify
{
      std::string server;
      std::string category;
      std::string key;
      std::string value;
      PublishState state;

      bool OK;
   public:
      AnalysePubSubNotify( bool err, MinosRPCObj *mro );
      std::string getServer()
      {
         return server;
      }
      std::string getCategory()
      {
         return category;
      }
      std::string getKey()
      {
         return key;
      }
      std::string getValue()
      {
         return value;
      }
      PublishState getState()
      {
         return state;
      }
      bool getOK()
      {
         return OK;
      }

};
//---------------------------------------------------------------------------
#endif
