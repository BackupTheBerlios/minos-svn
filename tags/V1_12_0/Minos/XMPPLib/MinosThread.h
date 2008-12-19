/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef minosThreadH
#define minosThreadH

//---------------------------------------------------------------------------
extern bool connected;
extern bool startDaemonThread( const std::string &jid );
extern bool runDaemonLoop();
extern void closeDaemonThread();
extern void sendAction( XStanza *a );

extern HANDLE MinosConnectEvent;

extern std::string myId;

bool XMPPInitialise( const std::string &myId );
bool XMPPClosedown();
class MinosConnection;
//---------------------------------------------------------------------------
class MinosThread: RPCDispatcher
{
   private:
      std::string jabberId;

      long lastEventTick;

      void sendError( TiXmlElement *pak, char *type, char *defined_condition );

      MinosConnection *ic;
   public:
      static MinosThread *minosThread;
      MinosThread( const std::string &jid );
      ~MinosThread();
      bool startDaemonThread();
      bool runDaemonLoop();
      void closeDaemonThread();
      virtual void dispatchResponse( XStanza *a );

      void onLog ( const char *data, size_t size, int is_incoming );

      void sendAction( XStanza *a );
      void setJid( const std::string &jid )
      {
         jabberId = jid;
      }
};

#endif
