//--------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MonitoredContestH
#define MonitoredContestH 
//---------------------------------------------------------------------------
class MonitoredContestLog: public BaseContestLog
{
   public:
      MonitoredContestLog();
      virtual ~MonitoredContestLog();

      virtual void makeContact( bool time_now, BaseContact *& );

};
#endif
