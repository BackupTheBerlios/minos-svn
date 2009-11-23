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
	#include <ws2tcpip.h>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------

#include "MServerZConf.h"
#include "MServerPubSub.h"
TZConf *ZConf = 0;
std::vector<Server *> serverList;
GJV_thread *zcThread = 0;

#define UPNP_PORT 9999
#define UPNP_GROUP "239.255.0.1"
class UPnPDataObject
{
   public:
      UPnPDataObject(): addrlen(0), AddressList(0), AddressListLength(0),
               NOTIFY_SEND_socks(0), NOTIFY_RECEIVE_sock(0)
      {
         memset(&addr, 0, sizeof(struct sockaddr_in));
         memset(&mreq, 0, sizeof(struct ip_mreq));
         // and most of  TZConf::initialise should be in here...
      }
      ~UPnPDataObject()
      {
         free(NOTIFY_SEND_socks);
         free(AddressList);
      }
      struct sockaddr_in addr;
      int addrlen;

      struct ip_mreq mreq;
      char message[ 4096 ];
      int *AddressList;
      int AddressListLength;

      SOCKET *NOTIFY_SEND_socks;
      SOCKET NOTIFY_RECEIVE_sock;
};
int ILibGetLocalIPAddressList( int** pp_int )
{

   //
   // Use an Ioctl call to fetch the IPAddress list
   //
   int i;
   char buffer[ 16 * sizeof( SOCKET_ADDRESS_LIST ) ];
   DWORD bufferSize;
   SOCKET TempSocket = socket( AF_INET, SOCK_DGRAM, 0 );
   WSAIoctl( TempSocket, SIO_ADDRESS_LIST_QUERY, NULL, 0, buffer, 16 * sizeof( SOCKET_ADDRESS_LIST ), &bufferSize, NULL, NULL );
   *pp_int = ( int* ) malloc( sizeof( int ) * ( 1 + ( ( SOCKET_ADDRESS_LIST* ) buffer ) ->iAddressCount ) );
   for ( i = 0;i < ( ( SOCKET_ADDRESS_LIST* ) buffer ) ->iAddressCount;++i )
   {
      ( *pp_int ) [ i ] = ( ( struct sockaddr_in* ) ( ( ( SOCKET_ADDRESS_LIST* ) buffer ) ->Address[ i ].lpSockaddr ) ) ->sin_addr.s_addr;
   }
   ( *pp_int ) [ i ] = inet_addr( "127.0.0.1" );
   closesocket( TempSocket );
   return ( 1 + ( ( SOCKET_ADDRESS_LIST* ) buffer ) ->iAddressCount );
}
bool TZConf::sendMessage( const UPnPDataObject *upnp, const std::string &mess )
{
   int packetlength;
   struct sockaddr_in addr;
   int addrlen;
   struct in_addr interface_addr;

   memset( ( char * ) &addr, 0, sizeof( addr ) );
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = inet_addr( UPNP_GROUP );
   addr.sin_port = ( unsigned short ) htons( UPNP_PORT );
   addrlen = sizeof( addr );

   memset( ( char * ) &interface_addr, 0, sizeof( interface_addr ) );

   for ( int i = 0;i < upnp->AddressListLength;++i )
   {
      interface_addr.s_addr = upnp->AddressList[ i ];
      if ( setsockopt( upnp->NOTIFY_SEND_socks[ i ], IPPROTO_IP, IP_MULTICAST_IF, ( char* ) & interface_addr, sizeof( interface_addr ) ) == 0 )
      {
         if ( sendto( upnp->NOTIFY_SEND_socks[ i ], mess.c_str(), mess.size(), 0, ( struct sockaddr * ) & addr, addrlen ) < 0 )
         {
            return false;
         }
      }
   }
   return true;
}
bool TZConf::initialise( UPnPDataObject *state )
{
   // I think that all this ZConf stuff will have to end up in it's own thread

   struct sockaddr_in addr;
   struct ip_mreq mreq;
   unsigned char TTL = 4;

   /* Complete State Reset */

   memset( ( char * ) &( state->addr ), 0, sizeof( state->addr ) );
   state->addr.sin_family = AF_INET;
   state->addr.sin_addr.s_addr = htonl( INADDR_ANY );
   state->addr.sin_port = ( unsigned short ) htons( UPNP_PORT );
   state->addrlen = sizeof( state->addr );


   /* Set up socket */
   state->AddressListLength = ILibGetLocalIPAddressList( &( state->AddressList ) );

   state->NOTIFY_SEND_socks = ( SOCKET* ) malloc( sizeof( int ) *( state->AddressListLength ) );

   state->NOTIFY_RECEIVE_sock = socket( AF_INET, SOCK_DGRAM, 0 );
   memset( ( char * ) &( addr ), 0, sizeof( addr ) );
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl( INADDR_ANY );
   addr.sin_port = ( unsigned short ) htons( UPNP_PORT );
   int ra = 1;
   if ( setsockopt( state->NOTIFY_RECEIVE_sock, SOL_SOCKET, SO_REUSEADDR, ( char* ) &ra, sizeof( ra ) ) < 0 )
   {
      trace( "Setting SockOpt SO_REUSEADDR failed\r\n" );
      return false;
   }
   if ( bind( state->NOTIFY_RECEIVE_sock, ( struct sockaddr * ) &( addr ), sizeof( addr ) ) < 0 )
   {
      trace( "Could not bind to UPnP Listen Port\r\n" );
      return false;
   }

   //
   // Iterate through all the current IP Addresses
   //
   for ( int i = 0;i < state->AddressListLength;++i )
   {
      state->NOTIFY_SEND_socks[ i ] = socket( AF_INET, SOCK_DGRAM, 0 );
      memset( ( char * ) & ( addr ), 0, sizeof( addr ) );
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = state->AddressList[ i ];
      addr.sin_port = ( unsigned short ) htons( UPNP_PORT );
      //
      // Set re-use address
      //
      if ( setsockopt( state->NOTIFY_SEND_socks[ i ], SOL_SOCKET, SO_REUSEADDR, ( char* ) & ra, sizeof( ra ) ) == 0 )
      {
         //
         // Set the Multicast TTL
         //
         if ( setsockopt( state->NOTIFY_SEND_socks[ i ], IPPROTO_IP, IP_MULTICAST_TTL, ( char* ) & TTL, sizeof( TTL ) ) < 0 )
         {
            /* Ignore this case */
         }

         //
         // Bind the socket
         //
         if ( bind( state->NOTIFY_SEND_socks[ i ], ( struct sockaddr * ) & ( addr ), sizeof( addr ) ) == 0 )
         {
            mreq.imr_multiaddr.s_addr = inet_addr( UPNP_GROUP );
            mreq.imr_interface.s_addr = state->AddressList[ i ];
            //
            // Join the multicast group
            //
            if ( setsockopt( state->NOTIFY_RECEIVE_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, ( char* ) &mreq, sizeof( mreq ) ) < 0 )
            {
               /* Does not matter */
            }
         }
      }
   }
   return true;
}
void runZcThread( void *t )
{
   TZConf * tzc = ( TZConf * ) t;
   tzc->runThread();
}
void TZConf::runThread()
{
   WSAGuard guardian;
   UPnPDataObject* RetVal = new UPnPDataObject();
   initialise( RetVal );


   std::string beaconreq = getZConfString(true);
   std::string nobeaconreq = getZConfString(false);

   bool sendBeaconResponse = false;
   unsigned int beaconInterval = 60000;   // once a minute
   int lastTick = GetTickCount();

   // send our first beacon message out... with a "report" request attached
   sendMessage( RetVal, beaconreq );

   /*
      <minosServer UUID='xxx' name='name' port='port' request='true' />

      UUID is unique to this "machine" (more likely username)
      , and is to allow us to manage
      station name changes. So where should we keep it?
      Looks like registry may be favourite; we don't really
      want two serevrs running on a single machine.

      Actually we can use the GUID as the event name for servers...

      "request" true asks recipient to beacon immediately
      beacon messages do NOT force recipients to beacon

      ALL servers will periodically set "request", but if a
      request has been received "recently" then we don't... (?)

      This mechanism should allow for networks that fragment, and
      then the server comes online - once the network heals then
      we rely on the periodic beacon

      We also need to time out stations which we haven't heard from.
   */

   // and now we go and sit on a select on all the notify sockets
   // but also remember to beacon occasionally...

   while ( !closeApp )
   {
      fd_set readset;

      FD_ZERO( &readset );

      struct timeval tv;
      tv.tv_sec = 0;  // half a second
      tv.tv_usec = 500000;

      // and we need to put in the sockets we are waiting on
      FD_SET( RetVal->NOTIFY_RECEIVE_sock, &readset );

      if (closeApp)
      {
         break;
      }

      int slct = select( FD_SETSIZE, &readset, NULL, NULL, &tv );
      if ( slct < 1 )    // timeout...
      {
         // so check if it is time to beacon...

         // Is it better to beacon between servers or to use "keep alive" packets?
         if (sendBeaconResponse || GetTickCount() - lastTick > beaconInterval )
         {
            CsGuard guard;
            trace(std::string("Sending beacon, sendBeaconResponse = ") + (sendBeaconResponse?"true":"false"));
            lastTick = GetTickCount();
            sendMessage( RetVal, sendBeaconResponse?nobeaconreq:beaconreq );   // timer requests beaconing, beacon just responds
            sendBeaconResponse = false;
         }
      }
      else
      {
         if ( FD_ISSET( RetVal->NOTIFY_RECEIVE_sock, &readset ) != 0 )
         {
            CsGuard g;
            char message[ 4096 ];
            int cnt = recvfrom( RetVal->NOTIFY_RECEIVE_sock, message, 4095, 0,
                                ( struct sockaddr * ) & ( RetVal->addr ), &( RetVal->addrlen ) );
            if ( cnt < 0 )
            {
               trace( "recvfrom" );
               return ;     // end of thread?
            }
            else
               if ( cnt > 0 )
               {
                  // and we have a message, so we need to parse it and do any necessary publishing
                  message[ cnt ] = 0;
                  std::string recvHost = std::string( inet_ntoa( RetVal->addr.sin_addr ) );
                  trace( "UDP received from " + recvHost + " : " + message );

                  sendBeaconResponse = setZConfString(message, recvHost);
               }
         }
      }
//      ServerScan();
   }
   delete RetVal;
}
Server *findStation( const std::string s )
{
   for ( std::vector<Server *>::iterator i = serverList.begin(); i != serverList.end(); i++ )
   {
      if ( strcmpi( ( *i ) ->station.c_str(), s.c_str() ) == 0 )
      {
         return ( *i );
      }
   }
   return 0;
}
void TZConf::ServerScan()
{
   CsGuard guard;
   for ( std::vector<Server *>::iterator i = serverList.begin(); !closeApp && i != serverList.end(); i++ )
   {
      trace("Server scan - checking " + (*i)->station);
      MinosServerListener::getListener() ->checkServerConnected( ( *i ), false );
      trace("Server scan - checked " + (*i)->station);
   }
}
void TZConf::readServerList()
{
   trace("Reading Server List File");
   // Read the server override file
   TIniFile * servers = new TIniFile( ".\\Configuration\\Servers.ini" );
   TStringList *sl = new TStringList;
   TStringList *params = new TStringList;
   servers->ReadSections( sl );
   for ( int i = 0; i < sl->Count; i++ )
   {
      servers->ReadSectionValues( sl->Strings[ i ], params );

      String uuid = params->Values[ "Uuid" ];
      String host = params->Values[ "Host" ];
      String station = params->Values[ "Station" ];
      String port = params->Values[ "Port" ];

      if ( uuid.Length() == 0 )
      {
         servers->WriteString( sl->Strings[ i ], "Uuid", uuid.c_str() );
         uuid = makeUuid().c_str();
      }
      if ( host.Length() == 0 )
      {
         if ( station.Length() == 0 )
         {
            continue;
         }
         host = station;
      }
      else
         if ( station.Length() == 0 )
         {
            station = host;
         }

      if ( port.Length() == 0 )
      {
         port == String(ServerPort);
      }

      publishServer( uuid.c_str(), station.c_str(), host.c_str(), port.ToIntDef( ServerPort ), true );

   }
   delete params;
   delete servers;
   ServerScan();
   trace("Finished reading Server List File");
}

//---------------------------------------------------------------------------
TZConf::TZConf( )
      : waitNameReply( false )
{}
TZConf::~TZConf( )
{
   zcThread->GJV_join();
   delete zcThread;
   
   for ( std::vector<Server *>::iterator i = serverList.begin(); i != serverList.end(); i++ )
   {
      delete ( *i );
   }
   serverList.clear();
}
//---------------------------------------------------------------------------
std::string trimr( const std::string &s )
{
   int i;
   for ( i = s.length(); i >= 0; i-- )
   {
      if ( !iscntrl( s[ i ] ) && !isspace( s[ i ] ) )
         break;
   }
   std::string s2;
   if ( i >= 0 )
      s2 = s.substr( 0, i + 1 );
   return s2;
}
std::string trim( const std::string &s )
{
   std::string s2 = trimr( s );
   unsigned int i;
   for ( i = 0; i < s2.length(); i++ )
   {
      if ( !iscntrl( s[ i ] ) && !isspace( s[ i ] ) )
         break;
   }
   std::string s3 = s2.substr( i, s2.length() );
   return s3;
}
void TZConf::setName( const std::string &name )
{
   localName = trim( name );
   zcThread = new GJV_thread( "ZConf", &runZcThread, ( void * ) this );
   readServerList();
}
//---------------------------------------------------------------------------
// uuid is the machines uuid (more unique than the server name!)
// name is server name
// hosttarget is the IP address/DNS name
// PortAsNumber is the remote port
// state is available/not available

// ONLY trouble is... clients will now address their servers by UUID!
// when they have subscribed to stations.

void TZConf::publishServer( const std::string &uuid, const std::string &name,
            const std::string &hosttarget, int PortAsNumber, bool autoReconnect )
{
   CsGuard g;
   trace( "publishServer Host " + hosttarget + " Station " + name +
            " Port " + makeStr( PortAsNumber ) + " uuid " + uuid + " auto " + ( autoReconnect ? "true" : "false" ) );
   Server *s = findStation( name );
   if ( s )
   {
      trace("Station " + name + " found zconf is " + ( s->zconf ? "true" : "false" ));
//      s->available = state;
      // we may not have all the details... we may need to add them
      if (!s->zconf)
      {
         s->uuid = uuid;
         s->host = hosttarget;
//      station should already be there
         s->port = PortAsNumber;

//         s->available = state;
         s->zconf = true;;
         if ( name == ZConf->getName() )
         {
            s->local = true;
         }
      }
   }
   else
   {
      trace("Station " + name + " not found");
      s = new Server( uuid, hosttarget, name, PortAsNumber );
//      s->available = state;
      s->autoReconnect = autoReconnect;
      if ( name == ZConf->getName() )
      {
         s->local = true;
      }
      serverList.push_back( s );
   }
   if ( s->local )
   {
      PubSubMain->publish( "", "LocalStation", name, hosttarget, psPublished );
   }
   else
   {
      MinosServerListener::getListener() ->checkServerConnected(s, true);
   }
   PubSubMain->publish( "", "Station", name, hosttarget, psPublished );
//   PubSubMain->publish( "", uuid, "Name", name );
//   PubSubMain->publish( "", uuid, "State", state?"true":"false" );
   trace("publishServer finished");
}
void TZConf::publishDisconnect(const std::string &name)
{
   Server *s = findStation( name );
   if ( s )
   {
      PubSubMain->publish( "", "Station", name, s->host, psNotConnected );
   }
}
//==============================================================================
std::string TZConf::getZConfString(bool beacon)
{
   std::string Uuid = getServerId();
   return  std::string("<minosServer ")
               + "UUID='" + Uuid
               + "' name='" + getName()
               + "' port='" + makeStr(ServerPort) + "'"
               + (beacon?" request='true'":"")
               + " />";
}
//==============================================================================
bool TZConf::setZConfString(const std::string &message, const std::string &recvHost)
{
   CsGuard g;
   bool sendBeaconResponse = false;

   TiXmlDocument xdoc;
   xdoc.Parse( message.c_str(), 0 );
   TiXmlElement * tix = xdoc.RootElement();
   if ( tix && checkElementName( tix, "minosServer" ) )
   {
      //"<minosServer UUID='" + Uuid + "' name='" + getName() + "' port='7778' request='true' />";
      std::string UUID = getAttribute( tix, "UUID" );
      std::string station = getAttribute( tix, "name" );
      std::string port = getAttribute( tix, "port" );
      std::string request = getAttribute( tix, "request" );
      //if ( UUID != Uuid && station != getName() )
      {
         // publish what came in - possibly we should publish the XML string
         // against the UUID?
         publishServer( UUID, station, recvHost, String(port.c_str()).ToIntDef(ServerPort), false );
         if ( request.size() && UUID != getServerId())
         {
            sendBeaconResponse = true;   // delay the response, give the other end a chance...
                        // BUT this means we could try to connect before they will recognise us.
                        // Should we queue the publish until then as well?

                        // But the UDP may fail to get there, so we need to be able to cope with new
                        // unadvertised connectors
         }
      }
   }
   return sendBeaconResponse;
}
//==============================================================================

