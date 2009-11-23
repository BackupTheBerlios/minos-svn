/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "XMPP_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class LogEventQ
{
   public:
      static std::deque <LogEvent *> logQueue;
};
std::deque <LogEvent *> LogEventQ::logQueue;
//---------------------------------------------------------------------------
static HANDLE logEventEvent = 0;
HANDLE makeLogEventEvent( bool create )
{
   if ( create )
   {
      if ( !logEventEvent )
         logEventEvent = CreateEvent( 0, TRUE, FALSE, 0 ); // Unnamed (internal) Manual reset
   }
   else
   {
      if ( logEventEvent )
      {
         CloseHandle( logEventEvent );
         logEventEvent = 0;
      }
   }
   return logEventEvent;
}
//---------------------------------------------------------------------------
void queueLog( LogEvent *a )
{
   if ( a )
   {
      GJV_scoped_lock lck;

      LogEventQ::logQueue.push_back( a );
      if ( logEventEvent )
         SetEvent( logEventEvent );
   }
}
LogEvent * deQueueLog()
{
   GJV_scoped_lock lck;

   if ( !LogEventQ::logQueue.empty() )
   {
      LogEvent * log = LogEventQ::logQueue[ 0 ];
      LogEventQ::logQueue.pop_front();
      return log;
   }
   else
   {
      if ( logEventEvent )
         ResetEvent( logEventEvent );
   }
   return 0;
}
//---------------------------------------------------------------------------
void logMessage( const std::string &level, const std::string &mess )
{
   // queue a log event on the event queue
   LogEvent * ev = new LogEvent;
   ev->eMessageType = level;
   ev->eMessage = mess;
   queueLog( ev );
}
//---------------------------------------------------------------------------
static HANDLE minosCloseEvent;
void createCloseEvent()
{
   if ( !minosCloseEvent )
      minosCloseEvent = CreateEvent( NULL, TRUE, FALSE, "MinosClosedownEvent" ); // manual reset, initially false
}
//---------------------------------------------------------------------------
void signalCloseEvent()
{
   createCloseEvent();
   SetEvent( minosCloseEvent );
}
//---------------------------------------------------------------------------
bool checkCloseEvent()
{
   createCloseEvent();
   return ( WaitForSingleObject( minosCloseEvent , 0 ) == WAIT_OBJECT_0 );
}
//---------------------------------------------------------------------------
void resetCloseEvent()
{
   createCloseEvent();
   ResetEvent( minosCloseEvent );
}
//---------------------------------------------------------------------------

