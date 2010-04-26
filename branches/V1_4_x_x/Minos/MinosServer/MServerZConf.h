/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MServerZConfH
#define MServerZConfH 
//---------------------------------------------------------------------------

class Server
{
   public:
      std::string uuid;
      std::string host;
      std::string station;
      int port;
      bool autoReconnect;

//      bool available;
      bool zconf;
      bool local;

      Server( const std::string &uuid, const std::string &h, const std::string &s, int p )
            : /*available( false ),*/ zconf( true ), local( false ),
            uuid(uuid), host( h ), station( s ), port( p ), autoReconnect(false)
      {}
      Server( const std::string &s )
            : /*available( false ),*/ zconf( false ), local( false ),
            station( s ), port( -1 ), autoReconnect(false)
      {}
};
extern std::vector<Server *> serverList;
extern Server *findStation( const std::string s );
struct UPnPDataObject;
class TZConf
{
   private:  	// User declarations

      static void publishServer( const std::string &uuid, const std::string &name,
                        const std::string &hosttarget, int PortAsNumber, bool autoReconnect );
      void readServerList();
      bool waitNameReply;
      std::string localName;

      bool initialise( struct UPnPDataObject *state );
      bool sendMessage( const struct UPnPDataObject *upnp, const std::string &mess );

   public:  		// User declarations

      TZConf( );
      ~TZConf( );
//      static void publishServer( const std::string &name );

      void runThread();

      void setName( const std::string &name );
      std::string getName()
      {
         return localName;
      }
      void ServerScan();

      std::string getZConfString(bool beacon = false);
      bool setZConfString(const std::string &message, const std::string &recvHost);
      void publishDisconnect(const std::string &name);
};
extern TZConf *ZConf;
#endif
