/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "XMPPClient_pch.h"
#pragma hdrstop
#include "MinosConnection.h" 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void
MinosConnection::io_close ()
{
   closesocket ( sock );
   sock = 0;
}

bool
MinosConnection::io_connect ( const char *server, int port )
{
   sock = -1;
   int tmp;
   struct hostent *host;
   struct sockaddr_in sin;

   host = gethostbyname ( server );
   if ( !host )
      return false;

   memcpy ( &sin.sin_addr, host->h_addr, host->h_length );
   sin.sin_family = host->h_addrtype;
   sin.sin_port = htons ( port );
   sock = socket ( host->h_addrtype, SOCK_STREAM, 0 );
   if ( sock == ( SOCKET ) - 1 )
      return false;

   tmp = connect ( sock, ( struct sockaddr * ) & sin, sizeof ( struct sockaddr_in ) );
   if ( tmp != 0 )
   {
      io_close ();
      return false;
   }

   return true;
}

bool
MinosConnection::io_send ( const char *data, size_t len )
{
   if ( send ( sock, data, len, 0 ) == -1 )
      return false;
   return true;
}

int
MinosConnection::io_recv ( char *buffer, size_t buf_len, int timeout )
{
   fd_set fds;
   struct timeval tv, *tvptr;
   int len;

   tv.tv_sec = 0;
   tv.tv_usec = 0;

   FD_ZERO ( &fds );
   FD_SET ( sock, &fds );
   tv.tv_sec = timeout;
   if ( timeout != -1 )
      tvptr = &tv;
   else
      tvptr = NULL;
   if ( select ( sock + 1, &fds, NULL, NULL, tvptr ) > 0 )
   {
      len = recv ( sock, buffer, buf_len, 0 );
      if ( len > 0 )
      {
         return len;
      }
      else
         if ( len <= 0 )
         {
            return -1;
         }
   }
   return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
MinosConnection::MinosConnection( RPCDispatcher *ud ) : sock( 0 ), user_data( ud )
{}
MinosConnection::~MinosConnection()
{}
bool MinosConnection::minos_send( const std::string &xmlstr )
{
   if ( xmlstr.size() )
   {
      unsigned int xmllen = ( unsigned int ) xmlstr.size();
      char *xmlbuff = new char[ 10 + 1 + xmllen + 1 ];
      sprintf( xmlbuff, "&&%d%s&&", xmllen, xmlstr.c_str() );
      io_send ( xmlbuff, strlen( xmlbuff ) + 1 );  // always include the zero terminator
      MinosThread::minosThread->onLog( xmlbuff, strlen( xmlbuff ), false );
      delete [] xmlbuff;
   }
   return false;
}
bool MinosConnection::minos_send( TiXmlElement *data )
{
   // serialise the data node, send it to the socket in a packet
   std::string xmlstr;
   xmlstr << ( *data );
   return minos_send( xmlstr );
}
bool MinosConnection::startConnection()
{
   // open up the socket connection to the minos
   return io_connect ( "localhost", ClientPort );
}
bool MinosConnection::runConnection()
{
   // read from the connection; buffer until a complete packet received

   int rxlen = io_recv( rxbuff, RXBUFFLEN, 1 );
   if ( rxlen < 0 )
   {
      closeDaemonThread();
      return false;
   }
   else
      if ( rxlen > 0 )
      {
         rxbuff[ rxlen ] = 0;

         // We might have embedded nulls between message parts - so strip them
         int rxpt = 0;
         while ( rxpt < rxlen )
         {
            unsigned int ptlen = strlen( &rxbuff[ rxpt ] );
            if ( ptlen )
            {
               MinosThread::minosThread->onLog( &rxbuff[ rxpt ], ptlen, true );
               packetbuff += &rxbuff[ rxpt ];   // which will strip out any nulls
            }
            rxpt += ptlen + 1;
         }

         while ( packetbuff.size() > 2 && packetbuff.substr( 0, 2 ) == "&&" )
         {
            unsigned int packetoffset = packetbuff.find( '<' );
            if ( packetoffset > 0 )    // length field should always be followed by XML
            {
               char * ec;
               int packetlen = strtol( packetbuff.c_str() + 2, &ec, 10 );
               if ( *ec == '<' && packetlen <= ( int ) strlen( ec ) + 2 && packetbuff.find( ">&&" ) )
               {
                  std::string packet = packetbuff.substr( packetoffset, packetlen );
                  packetbuff = packetbuff.substr( packetoffset + packetlen + 2, strlen( ec + packetlen ) );

#ifdef TRACE_PACKETS

                  TraceIncoming( packet.c_str() );
#endif

                  analyseNode( user_data, packet );
                  // and go round again...
               }
               else
               {
                  // partial message, keep receiving until we get more
                  return true;
               }
            }
         }
      }
   return true;
}
bool MinosConnection::closeConnection()
{
   // close down the socket connection
   io_close();
   return false;
}

