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
class ScreenContact;
class TMatchCollection;
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
   static void SendNextContactDetailsOnLeft();
   static void SendContestDetails();
   static void SendGoToSerial();
   static void SendMakeEntry();
   static void SendSetTimeNow();
   static void SendNextUnfilled();
   static void SendFormKey(WORD *);
   static void SendSetMode(String);
   static void SendSetFreq(String);
   static void SendEditMatchContact();
   static void SendScreenContactChanged(ScreenContact *);
   static void SendReplaceLogList(TMatchCollection *matchCollection);
   static void SendReplaceListList(TMatchCollection *matchCollection);
   static void SendScrollToCountry(std::string prefix);
   static void SendScrollToDistrict(std::string prefix);
};
//---------------------------------------------------------------------------
#endif
