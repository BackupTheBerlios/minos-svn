/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "MinosVK_pch.h"
#pragma hdrstop

#include "keyctrl.h"
#include "keyercontrol.h"
#include "WindowsMonitor.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWindowsMonitorForm *WindowsMonitorForm;
//---------------------------------------------------------------------------
__fastcall TWindowsMonitorForm::TWindowsMonitorForm( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TWindowsMonitorForm::PTTInCheckBoxClick( TObject */*Sender*/ )
{
   setLines( PTTInCheckBox->Checked, L1CheckBox->Checked, L2CheckBox->Checked );
}
//---------------------------------------------------------------------------
void __fastcall TWindowsMonitorForm::FormCloseQuery( TObject */*Sender*/,
      bool &CanClose )
{
   CanClose = false;   // stop Alt/F4 closing it
}
//---------------------------------------------------------------------------


void __fastcall TWindowsMonitorForm::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;
   Top = KeyControlForm->Top + KeyControlForm->Height;
   Left = KeyControlForm->Left;
}
//---------------------------------------------------------------------------

