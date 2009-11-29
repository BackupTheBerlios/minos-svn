/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef MinosLoggerEventsH
#define MinosLoggerEventsH
//---------------------------------------------------------------------------
#include "MinosEventData.h"
//---------------------------------------------------------------------------
class BaseContestLog;
class BaseContact;
class MinosLoggerEvents
{
private:
public:
   static void SendContestPageChanged();
   static void SendLogColumnsChanged();
   static void SendSplittersChanged();
   static void SendValidateError(int err);
   static void SendShowErrorList();
   static void SendReportOverstrike(bool ov);
   static void SendAfterLogContact(BaseContestLog *ct);
   static void SendAfterSelectContact(BaseContact *ct);

};
//---------------------------------------------------------------------------
#endif
