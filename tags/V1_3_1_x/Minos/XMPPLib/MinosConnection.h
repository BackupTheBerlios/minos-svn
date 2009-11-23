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

#ifndef minosConnectionH
#define minosConnectionH 
//---------------------------------------------------------------------------

class MinosConnection
{
#define RXBUFFLEN 4096
   private:
      RPCDispatcher *user_data;
      SOCKET sock;
      char rxbuff[ RXBUFFLEN ];
      std::string packetbuff;
      void io_close ();
      bool io_connect ( const char *server, int port );
      bool io_send ( const char *data, size_t len );
      int io_recv ( char *buffer, size_t buf_len, int timeout );

   public:
      MinosConnection( RPCDispatcher *ud );
      ~MinosConnection();
      bool startConnection();
      bool runConnection();
      bool closeConnection();
      bool minos_send( const std::string &data );
      bool minos_send( TiXmlElement *data );
};
#endif
