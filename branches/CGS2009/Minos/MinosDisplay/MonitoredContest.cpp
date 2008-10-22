//--------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "display_pch.h"
#pragma hdrstop

#include "MonitoredContest.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

MonitoredContestLog::MonitoredContestLog() : BaseContestLog()
{
}
MonitoredContestLog::~MonitoredContestLog()
{
}

void MonitoredContestLog::makeContact( bool timeNow, BaseContact *&lct )
{
   lct = new DisplayContestContact( this, timeNow );
}
