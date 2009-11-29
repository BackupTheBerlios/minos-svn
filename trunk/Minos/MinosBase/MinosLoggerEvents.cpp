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

