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
   static void SendReportOverstrike(bool ov, BaseContestLog *c);
   static void SendAfterLogContact(BaseContestLog *ct);
   static void SendAfterSelectContact(BaseContact *ct, BaseContestLog *);
   static void SendNextContactDetailsOnLeft();
   static void SendContestDetails(BaseContestLog *);
   static void SendGoToSerial(BaseContestLog *);
   static void SendMakeEntry(BaseContestLog *);
   static void SendNextUnfilled(BaseContestLog *);
   static void SendFormKey(WORD *, BaseContestLog *);
   static void SendSetMode(String, BaseContestLog *);
   static void SendSetFreq(String, BaseContestLog *);
   static void SendEditMatchContact(BaseContestLog *);
   static void SendScreenContactChanged(ScreenContact *, BaseContestLog *);
   static void SendReplaceLogList(TMatchCollection *matchCollection, BaseContestLog *);
   static void SendReplaceListList(TMatchCollection *matchCollection, BaseContestLog *);
   static void SendScrollToCountry(std::string prefix, BaseContestLog *);
   static void SendScrollToDistrict(std::string prefix, BaseContestLog *);
   static void SendMatchStarting(BaseContestLog *);
   static void SendOp1Change(std::string op1, BaseContestLog *);
   static void SendOp2Change(std::string op2, BaseContestLog *);
   static void SendShowOperators();
};
//---------------------------------------------------------------------------
#endif
