/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop

#include "tinyxml.h"
#include "controlport.h"
#include "portconf.h"
#include "CommonMonitor.h"

#include "WindowsMonitor.h"
#include "MinosControlMain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWindowsMonitorForm *WindowsMonitorForm = 0;
//---------------------------------------------------------------------------
__fastcall TWindowsMonitorForm::TWindowsMonitorForm( TComponent* Owner )
      : TForm( Owner )
{}
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
   Top = MinosControlForm->Top + MinosControlForm->Height;
   Left = MinosControlForm->Left;
}
//---------------------------------------------------------------------------

