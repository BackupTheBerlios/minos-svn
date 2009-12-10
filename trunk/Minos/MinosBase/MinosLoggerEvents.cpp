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
	ActionEvent<int ,EN_ValidateError>::Send(err);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendShowErrorList()
{
	ActionEventV<EN_ShowErrorList>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendReportOverstrike(bool ov)
{
	ActionEvent<bool ,EN_ReportOverstrike>::Send(ov);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendAfterLogContact(BaseContestLog *ct)
{
	ActionEvent<BaseContestLog * ,EN_AfterLogContact>::Send(ct);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendAfterSelectContact(BaseContact *lct)
{
	ActionEvent<BaseContact * ,EN_AfterSelectContact>::Send(lct);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendNextContactDetailsOnLeft()
{
	ActionEventV<EN_NextContactDetailsOnLeft>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendContestDetails()
{
	ActionEventV<EN_ContestDetails>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendGoToSerial()
{
	ActionEventV<EN_GoToSerial>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendMakeEntry()
{
	ActionEventV<EN_MakeEntry>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetTimeNow()
{
	ActionEventV<EN_SetTimeNow>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendNextUnfilled()
{
	ActionEventV<EN_NextUnfilled>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendFormKey(WORD *key)
{
	ActionEvent<WORD * ,EN_FormKey>::Send(key);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetMode(String mode)
{
	ActionEvent<String ,EN_SetMode>::Send(mode);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetFreq(String freq)
{
	ActionEvent<String ,EN_SetFreq>::Send(freq);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendEditMatchContact()
{
	ActionEventV<EN_EditMatchContact>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendScreenContactChanged(ScreenContact *sct)
{
	ActionEvent<ScreenContact * ,EN_ScreenContactChanged>::Send(sct);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendReplaceLogList(TMatchCollection *matchCollection)
{
	ActionEvent<TMatchCollection * ,EN_ReplaceLogList>::Send(matchCollection);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendReplaceListList(TMatchCollection *matchCollection)
{
	ActionEvent<TMatchCollection * ,EN_ReplaceListList>::Send(matchCollection);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendScrollToCountry(std::string prefix)
{
	ActionEvent<std::string ,EN_ScrollToCountry>::Send(prefix);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendScrollToDistrict(std::string prefix)
{
	ActionEvent<std::string ,EN_ScrollToDistrict>::Send(prefix);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendMatchStarting()
{
	ActionEventV<EN_MatchStarting>::Send();
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendOp1Change(std::string op)
{
	ActionEvent<std::string ,EN_Op1Change>::Send(op);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendOp2Change(std::string op)
{
	ActionEvent<std::string ,EN_Op2Change>::Send(op);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendShowOperators()
{
	ActionEventV<EN_ShowOperators>::Send();
}
//---------------------------------------------------------------------------

