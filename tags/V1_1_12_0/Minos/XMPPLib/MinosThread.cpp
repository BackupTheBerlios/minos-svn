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
/*
   The minos protocol is related to the XMPP version, but only vaguely, in that
   the payload is again XML/RPC, and made up exactly as for the XMPP version.
 
   The "wrapper" is a decimal length string at the start (length of the XML payload
   only), with a null ternminator on the end. The body between these two
   elements must be a well formed XML document.
 
   The whole message is enclosed in double amphersands; these should never
   appear in "real" XML/RPC data, as they should always be escaped (no CData allowed!)
 
   &&length<XML tag>...</XML tag>&&\0
 
   <?xml version="1.0" encoding="UTF-8"?> is allowed to be present
   but it will be assumed anyway.
 
   WE ONLY WORK HERE WITH UTF-8 ENCODINGS.
 
   The top level node is either <request>, <response>, or <event>.
 
   In practice, requests all come from content, responses from minos.
   events come from either side.
 
   Requests and responses are tied together by the "id" attribute; it is the
   responsability of the requester to provide a unique id string (we normally
   use numbers, but this is not required.
 
   <request requestName='name of request' id='1234'>
      <XML/RPC parameter body/>
   </request>
 
   <event eventName='name of event'>
      <XML/RPC parameter body/>
   </event>
 
   <response id='1234'>
      <XML/RPC parameter body/>
   </response>
 
*/ 
//---------------------------------------------------------------------------
MinosThread *MinosThread::minosThread = 0;
HANDLE MinosConnectEvent = 0;
bool connected = false;

std::string myId;

GJV_thread *MinosThreadObj = 0;

static bool terminated = false;
//---------------------------------------------------------------------------
bool startDaemonThread( const std::string &jid )
{
   MinosThread::minosThread = new MinosThread( jid );
   return MinosThread::minosThread->startDaemonThread();
}
bool runDaemonLoop()
{
   // if the server connection disappears then we exit this thread here
   return MinosThread::minosThread->runDaemonLoop();
}
void closeDaemonThread()
{
   if ( MinosThread::minosThread )
   {
      MinosThread::minosThread->closeDaemonThread();
      delete MinosThread::minosThread;
   }
   MinosThread::minosThread = 0;
   connected = false;
   if ( MinosConnectEvent )
      SetEvent( MinosConnectEvent );  // Tell ourselves that we can continue
}

//---------------------------------------------------------------------------

void MinosThreadExecute( void * )
{
   //---- Place thread code here ----
   while ( !terminated )
   {
      ::logMessage( "MinosThread", "Starting Minos thread" );
      if ( startDaemonThread( myId ) )
      {
         ::logMessage( "MinosThread", "Minos thread now running" );

         while ( !terminated )
         {
            if ( !runDaemonLoop() )     // one loop iteration
               break;
         }
         // it might be that we don't want to terminate - in which
         // case we probably want to restart the thread
         logMessage( "MinosThread", "Minos thread exiting" );
         if ( terminated || checkCloseEvent() )
         {
            terminated = true;   // make sure we don't attempt to reconnect
         }
      }
      else
      {
         // we want to wait for a time here, as the server might be restarting
      }
      closeDaemonThread();
      // BUT if not terminated, we need to re-subscribe and re-publish everything
   }
   terminated = false;
}
bool XMPPInitialise( const std::string &pmyId )
{

   if ( pmyId.size() == 0 )
   {
      logMessage( "XMPP", "No user ID set" );
      return false;
   }

   myId = std::string( pmyId ) + "@localhost";

   if ( MinosThreadObj )
   {
      logMessage( "Minos", "Threads already running" );
      return false;
   }

   int waitct = 100;
   while ( !checkServerReady() && waitct-- > 0 )
   {
      Sleep( 100 );
   }
   if ( !checkServerReady() )
      return false;

   terminated = false;
   // start boilerplate threads
   MinosConnectEvent = CreateEvent( 0, FALSE, FALSE, 0 ); // Unnamed (internal) Auto reset
   if ( !MinosConnectEvent )
      logMessage( "Minos", "Failed to create connect event" );

   MinosThreadObj = new GJV_thread( "MinosThread", &MinosThreadExecute, ( void * ) 0 );
   // wait for connection complete
   int i = 0;
   while ( i < 60 && WaitForSingleObject( MinosConnectEvent, 500 ) == WAIT_TIMEOUT )       // This resets the event
   {
      i++;
   }
   CloseHandle( MinosConnectEvent );
   MinosConnectEvent = 0;
   if ( !connected )
   {
      logMessage( "Minos", "Connect event not seen - terminating Minos thread" );
      XMPPClosedown();
      return false;
   }

   return true;
}
bool XMPPClosedown()
{
   // signal to close down all boilerplate threads

   terminated = true;

   if ( MinosThreadObj )
   {
      // wait for XMPP and Request threads to finish
      MinosThreadObj->GJV_join();
      delete MinosThreadObj;
      MinosThreadObj = 0;
   }

   RPCPubSub::close();
   return true;
}

//---------------------------------------------------------------------------
/*
   void onLog ( void *user_data, const char *data, size_t size, int is_incoming )
   {
      MinosThread * t = ( MinosThread * ) user_data;
      if ( t )
      {
         t->onLog( data, size, is_incoming );
      }
   }   
*/
void MinosThread::onLog ( const char *data, size_t /*size*/, int is_incoming )
{
   std::string logbuff;
   if ( is_incoming )
      logbuff += "RECV";
   else
      logbuff += "SEND";
   logbuff += "[";
   logbuff += data;
   logbuff += "]";

   logMessage( "XMPP", logbuff );
}

//---------------------------------------------------------------------------
MinosThread::MinosThread( const std::string &jid )
      : ic( 0 ), jabberId( jid ), lastEventTick( GetTickCount() )
{
   WSADATA wsaData;
   WSAStartup ( MAKEWORD ( 1, 1 ), &wsaData );
}
MinosThread::~MinosThread()
{
   WSACleanup ();
}
//---------------------------------------------------------------------------
bool MinosThread::startDaemonThread()
{

   logMessage( "Minos", "Connecting..." );

   // set up the stream

   ic = new MinosConnection( this );
   if ( ic->startConnection() )
   {
      connected = true;    // allow requests to go through...
      if ( MinosConnectEvent )
         SetEvent( MinosConnectEvent );  // Tell ourselves that we can continue
      RPCRequest *rpa = new RPCRequest( "", myId, "ClientSetFromId" );   // for our local server, this one MUST have a from
      rpa->addParam( myId );
      sendAction( rpa );
      delete rpa;
      RPCPubSub::reconnectPubSub();
      return true;
   }
   return false;

}
//---------------------------------------------------------------------------
bool MinosThread::runDaemonLoop()
{
   return ic->runConnection();
}
//---------------------------------------------------------------------------
void MinosThread::closeDaemonThread()
{
   if ( connected )
   {
      connected = false;
   }
   if ( ic )
   {
      ic->closeConnection();
      delete ic;
      ic = 0;
   }
}
//---------------------------------------------------------------------------
void sendAction( XStanza *a )
{
   if ( MinosThread::minosThread )
   {
      MinosThread::minosThread->sendAction( a );
   }
}
void MinosThread::sendAction( XStanza *a )
{
   if ( connected )
   {
      // Two stage so we can keep prs private
      a->setNextId();   // only happens if no Id already
      std::string xmlstr = a->getActionMessage();
      ic->minos_send ( xmlstr );
   }
}
//---------------------------------------------------------------------------
void MinosThread::dispatchResponse( XStanza *xs )
{
   RPCRequest * req = dynamic_cast<RPCRequest *>( xs );
   if ( req )
   {
      makeXMPPEvent( req );
   }
   else
   {
      RPCResponse * rr = dynamic_cast<RPCResponse *>( xs );
      if ( rr )
      {
         if ( rr->methodName == "ClientSetFromId" )
         {
            // server will return the REAL Jid
            std::string ouraddr;
            std::string from;
            if ( rr->getStringArg( 0, from ) )
            {
               // check that from is what connected to us...
               setJid( ouraddr );
            }
         }
         else
            makeXMPPEvent( rr );
      }
   }
}
//---------------------------------------------------------------------------

