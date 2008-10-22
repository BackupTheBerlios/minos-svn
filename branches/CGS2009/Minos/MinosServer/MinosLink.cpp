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

//==============================================================================
void
xperror( int test_val, char *mesg, bool endit = true )
{
   DWORD lerrno = GetLastError();
   if ( test_val )
   {
		std::string s = std::string( mesg ) + " : error number " + makeStr( lerrno );
		logMessage( "xperror", s );

      if ( endit )
      {
			throw Exception( ("JServer error: " + s ).c_str());
      }
   }
}
//==============================================================================
/*
   class MinosId
   {
      public:
         std::string user;
         std::string server;
         std::string fullId;
*/

bool MinosId::empty()
{
   return fullId.size() == 0;
}

MinosId::MinosId()
{
}
MinosId::MinosId( const std::string &s )
{
   setId( s );
}
MinosId::~MinosId()
{
}

void MinosId::setId( const std::string &s )
{
   fullId = s;
   unsigned int atpos = s.find( "@" );
   if ( atpos == std::string::npos )
   {
      server = s;
   }
   else
   {
      user = s.substr( 0, atpos );
      server = s.substr( atpos + 1, s.size() );
   }
}

//==============================================================================
bool MinosSocket::newConnection = false;
MinosSocket::MinosSocket() : sock( INVALID_SOCKET ),
   remove( false ),
   txConnection(false)

{}
MinosSocket::~MinosSocket()
{
   closeSocket();
}
//=============================================================================
MinosListener::MinosListener()
{}
MinosListener::~MinosListener()
{
   clearSockets();
}
//-----------------------------------------------------------------------------
bool MinosListener::initialise( AnsiString type, int port )
{
   sock = socket( AF_INET, SOCK_STREAM, 0 );
   xperror( sock == INVALID_SOCKET, ( type + " socket" ).c_str() );

   SOCKADDR_IN ad;
   memset( &ad, 0, sizeof( ad ) );
   ad.sin_family = AF_INET;
   ad.sin_addr.s_addr = INADDR_ANY;
   ad.sin_port = htons( port );

   int res = bind( sock, ( LPSOCKADDR ) & ad, sizeof( ad ) );
   xperror( res == SOCKET_ERROR, ( type + " bind" ).c_str() );

   res = listen( sock, 5 );
   xperror( res == SOCKET_ERROR, ( type + " listen" ).c_str() );

   i_array.push_back( this );

   return true;
}
bool MinosListener::acceptFreeSlot( MinosCommonConnection *il )
{
   if ( i_array.size() < MAXIPSLOTS )
   {
      /* Create a new end point */
      i_array.push_back( il );
      il->initialise();

      SOCKADDR_IN sin;
      int in_addr_len = sizeof( sin );

      SOCKET s = accept( sock, ( LPSOCKADDR ) & sin, &in_addr_len );
      il->connectHost = std::string( inet_ntoa( sin.sin_addr ) );
      logMessage( "acceptFreeSlot", "from " + il->connectHost );
      if ( s == INVALID_SOCKET )
      {
         return false;
      }

      il ->setSocket( s );
      return true;
   }
   else
   {
      logMessage( "acceptFreeSlot", "Socket table full" );
      SOCKET tmp = accept( sock, NULL, NULL );
      closesocket( tmp );
      delete il;
   }
   return false;
}
// add a connected socket
bool MinosListener::connectFreeSlot( MinosCommonConnection *il )
{
   if ( i_array.size() < MAXIPSLOTS )
   {
      /* Create a new end point */
      newConnection = true;
      i_array.push_back( il );
      il->initialise();
      return true;
   }
   else
   {
      logMessage( "connectFreeSlot", "Socket table full" );
      delete il;
   }
   return false;
}
int MinosListener::getConnectionCount()
{
   int cnt = 0;
   for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
   {
      if ( ( *i ) && ( *i ) ->getSocket() != INVALID_SOCKET )
      {
         cnt++;
      }
   }
   int cnt2 = ( int ) i_array.size();
   if ( cnt != cnt2 )
   {
      cnt = -1;
   }
   return cnt;
}
bool MinosListener::isServerConnection( const MinosId &s )
{
   for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
   {
      // worry about the details
      if ( ( *i ) ->checkServer( s ) )
      {
         return false;
      }
      if ( ( *i ) -> checkServer( s ) )
      {
         return true;
      }
   }
   return false;
}
bool MinosListener::isClientConnection( const MinosId &s )
{
   for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
   {
      // worry about the details
      if ( ( *i ) ->checkUser( s ) )
      {
         return true;
      }
      if ( ( *i ) -> checkServer( s ) )
      {
         return false;
      }
   }
   return false;
}
//==============================================================================
/* */
/* Check for activity on the sockets */
/* */

// Predicate function for remove_if
bool nosock( MinosSocket *ip )
{
   if ( ip == 0 )
      return true;
   else
      return false;
}
void MinosListener::processSockets( void )
{
   try
   {
      int ready_cnt = 0;

      fd_set in_socks;

      FD_ZERO( &in_socks );

      /*
           Add the active sockets to the list to be checked
      */
      {
         CsGuard g;
         for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
         {
            if ( ( *i ) && ( *i ) ->getSocket() != INVALID_SOCKET )
            {
               FD_SET( ( *i ) ->getSocket(), &in_socks );
            }
         }
      }

      // When we get going, then we also want to queue output on each socket
      // and send when the socket is ready - i.e. when a send is likely to
      // succeed

      timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = 50000;

      ready_cnt = select( 0, &in_socks, NULL, NULL, &timeout );

      if (closeApp)
      {
         return;
      }

      /*
         Pick up on the timeout
      */
      CsGuard g;
      if ( ready_cnt < 1 )
      {
         for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
         {
            try
            {
               if ( ( *i ) )
               {
                  ( *i ) ->sendKeepAlive();
               }
            }
            catch ( ... )
            {
               logMessage( "process_sockets", "Unknown Exception in sendKeepAlive" );
               closeApp = true;
               break;
            }
         }
         return ;
      }
      /*
              Check for each of the sockets which could have fired for read
      */
      bool clearup = false;
      for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
      {
         if ( ( *i ) && FD_ISSET( ( *i ) ->getSocket(), &in_socks ) )
         {
            try
            {
               MinosSocket * ip = ( *i );
               ip ->process();  // This may add an element...  in which case the iterator is screwed
               if ( ip->newConnection )
               {
                  ip->newConnection = false;
                  break;
               }
            }
            catch ( Exception & e )
            {
					logMessage( "process_sockets", std::string( "Exception processing socket : " ) + e.Message.t_str() );
               closeApp = true;
               break;
            }
            catch ( ... )
            {
               logMessage( "process_sockets", "Unknown Exception processing socket" );
               closeApp = true;
               break;
            }
         }
      }
      for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
      {
         try
         {
            if ( ( *i ) ->remove
               )
            {
               // process says to finish off
               logMessage( "process_sockets", std::string( "deleting socket : " ) + ( *i ) ->getIdentity() );
               delete ( *i );
               *i = 0;
               clearup = true;
            }
         }
         catch ( Exception & e )
         {
				logMessage( "process_sockets", std::string( "Exception tidying socket : " ) + e.Message.t_str() );
            closeApp = true;
            break;
         }
         catch ( ... )
         {
            logMessage( "process_sockets", "Unknown Exception tidying socket" );
            closeApp = true;
            break;
         }
      }
      if ( clearup )
      {
         i_array.erase( std::remove_if( i_array.begin(), i_array.end(), nosock ), i_array.end() );
      }
   }
   catch ( Exception & e )
   {
		logMessage( "process_sockets", std::string( "Exception while processing: " ) + e.Message.t_str() );
      closeApp = true;
   }
}
void MinosListener::clearSockets()
{
   GJV_scoped_lock lck;
   try
   {
      for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
      {
         // we have to be careful - we ARE one of these...
         //( *i ) ->remove = true;
         if ( ( *i ) != this )
            delete ( *i );
      }
      //      i_array.erase( std::remove_if( i_array.begin(), i_array.end(), nosock ), i_array.end() );
      i_array.clear();
   }
   catch ( Exception & e )
   {
      logMessage( "clear_sockets", std::string( "Exception while closing sockets: " ) + e.Message.t_str() );
   }
}
bool MinosListener::checkServerConnection( const std::string &sname )
{
   for ( std::vector<MinosSocket *>::iterator i = i_array.begin(); i != i_array.end(); i++ )
   {
      if ( ( *i ) ->checkServer( sname ) )
      {
         return true;               // we only need ONE that is right...
      }
   }
   return false;
}
//==============================================================================
//==============================================================================
MinosCommonConnection::MinosCommonConnection()
: fromIdSet( false ), connchecked( false )
{}
MinosCommonConnection::~MinosCommonConnection()
{}
bool MinosCommonConnection::CheckLocalConnection()
{
   if ( !connchecked )
   {
      std::string sname = MinosServer::getMinosServer() ->getServerName();
      if ( ( stricmp( clientServer.c_str(), "localhost" ) != 0 ) &&
           ( stricmp( clientServer.c_str(), sname.c_str() ) != 0 ) )
      {
         return false;        // not a good host name in from
      }
      unsigned int connaddr = inet_addr( connectHost.c_str() );
      if ( connaddr == inet_addr( "127.0.0.1" ) )
      {
         connchecked = true;
         return true;             // connection address is 127.0.0.1
      }
      Server *serv = findStation( sname );
      if ( serv )
      {
         connchecked = true;
         return true;          // connection address is one we own
      }
      return false;
   }
   return true;
}
std::string MinosCommonConnection::makeJid()
{
   std::string id;
   if ( clientUser.length() )
      id = clientUser + "@";
   id += clientServer;

   return id;
}
bool MinosCommonConnection::sendRaw ( const char *xmlstr )
{
   unsigned int xmllen = strlen( xmlstr );
   if ( xmllen )
   {
      char * xmlbuff = new char[ 10 + 1 + xmllen + 1 ];
      sprintf( xmlbuff, "&&%d%s&&", xmllen, xmlstr );
      xmllen = strlen( xmlbuff );
      int ret = send ( getSocket(), xmlbuff, xmllen, 0 );
      onLog ( xmlbuff, xmllen, 0 );
      if ( ret == -1 )
         return false;
      delete [] xmlbuff;
   }
   return true;
}
void MinosCommonConnection::onLog ( const char *data, size_t /*size*/, int is_incoming )
{
   std::string logbuff;
   if ( is_incoming )
      logbuff += "RECV";
   else
      logbuff += "SEND";

   logbuff += "[";
   logbuff += data;
   logbuff += "]";

   logMessage( "MinosCommonConnection", logbuff.c_str() );
}
//---------------------------------------------------------------------------
bool MinosCommonConnection::tryForwardStanza( TiXmlElement *tix )
{
   std::string s;
   s << *tix;
   bool res = sendRaw( s.c_str() );
   return res;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void MinosCommonConnection::sendError( TiXmlElement *tix, char * /*type*/, char *defined_condition )
{
   /*
   <stanza-kind to='sender' type='error'>
     [RECOMMENDED to include sender XML here]
     <error type='error-type'>
       <defined-condition xmlns='urn:ietf:params:xml:ns:xmpp-stanzas'/>
       <text xmlns='urn:ietf:params:xml:ns:xmpp-stanzas'
             xml:lang='langcode'>
         OPTIONAL descriptive text
       </text>
       [OPTIONAL application-specific condition element]
     </error>
   </stanza-kind>
   */

   // to a server, we might have to include our own "from" - or it might reject
   // as no "from"

   // we need to check if this is an error response already...
   // and if so, we cannot send an error(again)

   std::string from = getAttribute( tix, "from" );
   std::string to = getAttribute( tix, "to" );
   std::string id = getAttribute( tix, "id" );
   std::string subtype = getAttribute( tix, "type" );

   if ( stricmp( subtype.c_str(), "error" ) == 0 )
      return ;

   TiXmlElement x( "iq" );

   x.SetAttribute( "type", "error" );
   x.SetAttribute ( "id", id );

   TiXmlElement condition( "error_condition" );
   TiXmlText t( defined_condition );
   condition.InsertEndChild( t );
   x.InsertEndChild( condition );

   std::string s;
   s << x;

   sendRaw ( s.c_str() );
}
//---------------------------------------------------------------------------
void MinosCommonConnection::sendAction( XStanza *a )
{
   // use the stanza to send itself
   a->setNextId();   // only happens if no Id already
   std::string s = a->getActionMessage();
   sendRaw( s.c_str() );
}
//=============================================================================
void sendAction( XStanza *a )
{
   // stanza has a "to" - but this is internal, so we need to dispatch it
   std::string mess = a->getActionMessage();
   int err;

   // convert from a RPCParam structure to a DOM

   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument xdoc;
   xdoc.Parse( mess.c_str(), 0 );
   TiXmlElement *x = xdoc.RootElement();

   if ( a->getFrom().size() == 0 )
   {
      // insert a from of ourselves.

      std::string from = MinosServer::getMinosServer() ->getServerName();
      if ( from.length() )
      {
         x->SetAttribute( "from", from.c_str() );
      }
   }
   std::string to = a->getTo();
   if ( to.size() != 0 )
   {
      x->SetAttribute( "to", to.c_str() );
   }
   // and now dispatch to its destination

   if ( !MinosServer::getMinosServer() ->forwardStanza( 0, x ) )              // our own services
   {
      if ( !MinosClientListener::getListener() ->sendClient( 0, x ) )         // look at real and potential clients
      {
         if ( !MinosServerListener::getListener() ->sendServer( 0, x ) )         // look at real and potential servers
         {
            // or no valid destination found
            //sendError( pak, "cancel", "item-not-found" );
            return ;
         }
      }
   }
   // or no valid destination found
}
//=============================================================================
void MinosCommonConnection::process()
{
   // select says we have data, so read it
   // and send the data through the parser
   logMessage ( "XMPP test", "MinosCommonConnection::process called to receive data from " + clientUser );

   int rxlen = recv ( getSocket(), rxbuff, 4096 - 1, 0 );
   if ( rxlen > 0 )
   {
      rxbuff[ rxlen ] = '\0';

      // We might have embedded nulls between message parts - so strip them
      int rxpt = 0;
      while ( rxpt < rxlen )
      {
         int ptlen = ( int ) strlen( &rxbuff[ rxpt ] );
         if ( ptlen )
         {
            onLog ( &rxbuff[ rxpt ], ptlen, 1 );  // but this ignores the wrapper
            packetbuff += &rxbuff[ rxpt ];   // which will strip out any nulls
         }
         rxpt += ptlen + 1;
      }

      while ( packetbuff.size() > 2 && packetbuff.substr( 0, 2 ) == "&&" )
      {
         int packetoffset = ( int ) packetbuff.find( '<' );
         if ( packetoffset > 0 )    // length field should always be followed by XML
         {
            char * ec;
            int packetlen = strtol( packetbuff.c_str() + 2, &ec, 10 );
            if ( *ec == '<' && packetlen <= ( int ) strlen( ec ) + 2 && packetbuff.find( ">&&" ) )
            {
               std::string packet = packetbuff.substr( packetoffset, packetlen );
               packetbuff = packetbuff.substr( packetoffset + packetlen + 2, strlen( ec + packetlen ) );

               TiXmlBase::SetCondenseWhiteSpace( false );
               TiXmlDocument xdoc;
               xdoc.Parse( packet.c_str(), 0 );
               TiXmlElement *tix = xdoc.RootElement();
               analyseNode( tix );
            }
            else
            {
               // partial message, keep receiving until we get more
               return ;
            }
         }
      }
   }
   else
   {
      remove
         = true;
   }
}
//==============================================================================
void MinosCommonConnection::analyseNode( TiXmlElement *tix )
{
   // response factory - look at the node, and build the correct response stanza object

   // checkFrom looks at the "from" address, and validates it against the connection
   // A server connection has to have a "from" (but it isn't necessarily correct, if its been proxied)
   // A client must either have a from address, or nothing - when checkFrom will insert it

   if ( !checkFrom( tix ) )
   {
      if ( isServer() )
      {
         closeSocket();
         remove = true;
      }
      else
      {
         sendError( tix, "cancel", "invalid-from" );
      }
      return ;
   }
   // Dispatch to its destination

   // we should first look for "built in" clients - initially this means PubSub
   // ZConf is a possibility, but it (currently) works via PubSub
   // Actually, we could build these in to the server as RPC calls
   // - don't need a decent "to", just the server name
   if ( !MinosServer::getMinosServer() ->forwardStanza( this, tix ) )              // our own services
   {
      if ( !MinosClientListener::getListener() ->sendClient( this, tix ) )         // look at real and potential clients
      {
         if ( !MinosServerListener::getListener() ->sendServer( this, tix ) )         // look at real and potential servers
         {
            // or no valid destination found
            sendError( tix, "cancel", "item-not-found" );
            return ;
         }
      }
   }

}
//=============================================================================
