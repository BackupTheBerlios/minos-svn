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

#ifndef LogEventsH
#define LogEventsH 
//---------------------------------------------------------------------------
extern void createCloseEvent();
extern void signalCloseEvent();
extern bool checkCloseEvent();
extern void resetCloseEvent();
//---------------------------------------------------------------------------
class LogEvent
{
   public:
      std::string eMessageType;
      std::string eMessage;

      LogEvent()
      {}
}
;
//---------------------------------------------------------------------------
HANDLE makeLogEventEvent( bool create );
//---------------------------------------------------------------------------
void queueLog( LogEvent *a );
LogEvent * deQueueLog();
//---------------------------------------------------------------------------
extern void logMessage( const std::string &level, const std::string &mess );
//---------------------------------------------------------------------------
#endif
