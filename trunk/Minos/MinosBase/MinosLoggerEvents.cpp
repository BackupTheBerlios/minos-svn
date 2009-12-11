/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <string>
#include "MinosEventData.h"
#include "MinosEventManager.h"
#include "MinosLoggerEvents.h"
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendContestPageChanged()
{
	ActionEventV<EN_ContestPageChanged>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendLogColumnsChanged()
{
	ActionEventV<EN_LogColumnsChanged>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSplittersChanged()
{
	ActionEventV<EN_SplittersChanged>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendValidateError(int err)
{
	ActionEvent1<int , EN_ValidateError>::Send(err);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendShowErrorList()
{
	ActionEventV<EN_ShowErrorList>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendReportOverstrike(bool ov, BaseContestLog *c)
{
	ActionEvent2<bool , BaseContestLog *, EN_ReportOverstrike>::Send(ov, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendAfterLogContact(BaseContestLog *ct)
{
	ActionEvent1<BaseContestLog * ,EN_AfterLogContact>::Send(ct);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendAfterSelectContact(BaseContact *lct, BaseContestLog *c)
{
	ActionEvent2<BaseContact *, BaseContestLog * ,EN_AfterSelectContact>::Send(lct, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendNextContactDetailsOnLeft()
{
	ActionEventV<EN_NextContactDetailsOnLeft>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendContestDetails(BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *, EN_ContestDetails>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendGoToSerial(BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *, EN_GoToSerial>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendMakeEntry(BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *, EN_MakeEntry>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetTimeNow(BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *,EN_SetTimeNow>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendNextUnfilled(BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *, EN_NextUnfilled>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendFormKey(WORD *key, BaseContestLog *c)
{
	ActionEvent2<WORD *, BaseContestLog * ,EN_FormKey>::Send(key, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetMode(String mode, BaseContestLog *c)
{
	ActionEvent2<String, BaseContestLog * ,EN_SetMode>::Send(mode, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetFreq(String freq, BaseContestLog *c)
{
	ActionEvent2<String, BaseContestLog * ,EN_SetFreq>::Send(freq, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendEditMatchContact(BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *, EN_EditMatchContact>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendScreenContactChanged(ScreenContact *sct, BaseContestLog *c)
{
	ActionEvent2<ScreenContact *, BaseContestLog * ,EN_ScreenContactChanged>::Send(sct, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendReplaceLogList(TMatchCollection *matchCollection, BaseContestLog *c)
{
	ActionEvent2<TMatchCollection *, BaseContestLog * ,EN_ReplaceLogList>::Send(matchCollection, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendReplaceListList(TMatchCollection *matchCollection, BaseContestLog *c)
{
	ActionEvent2<TMatchCollection *, BaseContestLog * ,EN_ReplaceListList>::Send(matchCollection, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendScrollToCountry(std::string prefix, BaseContestLog *c)
{
	ActionEvent2<std::string, BaseContestLog * ,EN_ScrollToCountry>::Send(prefix, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendScrollToDistrict(std::string prefix, BaseContestLog *c)
{
	ActionEvent2<std::string, BaseContestLog * ,EN_ScrollToDistrict>::Send(prefix, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendMatchStarting( BaseContestLog *c)
{
	ActionEvent1<BaseContestLog *, EN_MatchStarting>::Send(c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendOp1Change(std::string op, BaseContestLog *c)
{
	ActionEvent2<std::string, BaseContestLog * ,EN_Op1Change>::Send(op, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendOp2Change(std::string op, BaseContestLog *c)
{
	ActionEvent2<std::string, BaseContestLog * ,EN_Op2Change>::Send(op, c);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendShowOperators()
{
	ActionEventV<EN_ShowOperators>::Send();
}
//---------------------------------------------------------------------------

