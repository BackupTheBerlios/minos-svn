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

#ifndef MinosLinkH
#define MinosLinkH 
//---------------------------------------------------------------------------
class MinosId
{
   public:
      std::string user;
      std::string server;
      std::string fullId;

      bool empty();

      MinosId();
      MinosId( const std::string & );
      //      MinosId(const std::string &id);
      virtual ~MinosId();

      void setId( const std::string & );
};
class MinosSocket
{
   private:
   protected:
      SOCKET sock;
      bool txConnection;      // set if we can transmit on this connection
   public:
      MinosSocket();
      virtual ~MinosSocket();

      virtual void process() = 0;

      bool remove;
      static bool newConnection;

      SOCKET &getSocket()
      {
         return sock;
      }
      void setSocket( SOCKET s )
      {
         sock = s;
      }
      void closeSocket()
      {
         if ( sock != INVALID_SOCKET )
         {
            closesocket( sock );
         }
         sock = INVALID_SOCKET;
      }
      virtual bool tryForwardStanza( TiXmlElement *pak )
      {
         return false;
      }
      virtual bool checkServer( const MinosId &s )
      {
         return false;
      }
      virtual bool checkServer( const std::string &s )
      {
         return false;
      }
      virtual bool checkUser( const MinosId &s )
      {
         return false;
      }
      virtual std::string getIdentity() = 0;
      virtual void sendKeepAlive( )
      {}
      bool isTxConnection()
      {
         return txConnection;
      }
};
//==============================================================================
#define RXBUFFLEN 4096

class MinosCommonConnection: public MinosSocket
{
   private:
      char rxbuff[ RXBUFFLEN ];
      std::string packetbuff;
      std::string rxBuff;

   protected:

      // who is connected?
      std::string clientServer;     // server name
      std::string clientUser;       // client name (or empty)
      std::string makeJid();        // return the Jabber ID

      void onLog ( const char *data, size_t size, int is_incoming );
      bool sendRaw ( const char *xmlstr );
   public:
      bool fromIdSet;
      bool connchecked;
      std::string connectHost;

      MinosCommonConnection();
      virtual bool initialise( ) = 0;
      virtual ~MinosCommonConnection();

      virtual void process();
      virtual void analyseNode( TiXmlElement *pak );
      virtual bool tryForwardStanza( TiXmlElement *pak );
      virtual void sendError( TiXmlElement *pak, char *type, char *defined_condition );
      virtual void sendAction( XStanza *a );

      virtual bool isServer() = 0;
      virtual bool checkFrom( TiXmlElement *pak ) = 0;
      virtual bool setFromId( MinosId &from, RPCRequest *req ) = 0;

      bool CheckLocalConnection();

      virtual bool checkServer( const MinosId &s )
      {
         return ( stricmp( clientServer.c_str(), s.server.c_str() ) == 0 );
      }
      virtual bool checkServer( const std::string &s )
      {
         return ( stricmp( clientServer.c_str(), s.c_str() ) == 0 ) ;
      }
      virtual bool checkUser( const MinosId &u )
      {
         return ( stricmp( clientUser.c_str(), u.user.c_str() ) == 0 );
      }
      virtual std::string getIdentity() = 0;
};
//==============================================================================
class MinosListener: public MinosSocket
{
   private:
   protected:
#define MAXIPSLOTS 64         // max for a single select call

      std::vector<MinosSocket *>i_array;
   public:
      void processSockets();
      void clearSockets();
      bool connectFreeSlot( MinosCommonConnection * );
      bool acceptFreeSlot( MinosCommonConnection * );
      int getConnectionCount();
      MinosListener();
      ~MinosListener();
      virtual bool initialise( AnsiString type, int port );
      virtual std::string getIdentity() = 0;

      bool isServerConnection( const MinosId &s );
      bool isClientConnection( const MinosId &s );
      bool checkServerConnection( const std::string &sname );
};
//==============================================================================
#endif
