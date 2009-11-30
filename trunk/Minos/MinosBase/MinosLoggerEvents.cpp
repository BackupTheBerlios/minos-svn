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
#include "MinosEventData.h"
#include "MinosEventManager.h"
#include "MinosLoggerEvents.h"
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendContestPageChanged()
{
	ActionEvent<int ,EN_ContestPageChanged>::Send(0); // as we cannot have void
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendLogColumnsChanged()
{
	ActionEvent<int ,EN_LogColumnsChanged>::Send(0); // as we cannot have void
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSplittersChanged()
{
	ActionEvent<int ,EN_SplittersChanged>::Send(0); // as we cannot have void
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendValidateError(int err)
{
	ActionEvent<int ,EN_ValidateError>::Send(err);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendShowErrorList()
{
	ActionEvent<int ,EN_ShowErrorList>::Send(0);
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
	ActionEvent<int ,EN_NextContactDetailsOnLeft>::Send(0);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendContestDetails()
{
	ActionEvent<int ,EN_ContestDetails>::Send(0);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendGoToSerial()
{
	ActionEvent<int ,EN_GoToSerial>::Send(0);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendMakeEntry()
{
	ActionEvent<int ,EN_MakeEntry>::Send(0);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendSetTimeNow()
{
	ActionEvent<int ,EN_SetTimeNow>::Send(0);
}
//---------------------------------------------------------------------------
void MinosLoggerEvents::SendNextUnfilled()
{
	ActionEvent<int ,EN_NextUnfilled>::Send(0);
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
	ActionEvent<String ,EN_EditMatchContact>::Send(0);
}
//---------------------------------------------------------------------------

