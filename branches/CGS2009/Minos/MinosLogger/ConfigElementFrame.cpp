/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "base_pch.h"
#pragma hdrstop

#include <inifiles.hpp>

#include "ConfigDM.h"
#include "ConfigElementFrame.h"
//#include "ConfigMain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TIniFile *config;
//---------------------------------------------------------------------------
__fastcall TConfigElementFrm::TConfigElementFrm( TComponent* Owner, TConfigElement *tce )
      : TFrame( Owner ), tce( tce )
{}
//---------------------------------------------------------------------------
bool TConfigElementFrm::initialise( String sect )
{
#warning need to check for duplicate names!
   Name = sect;
   AppNameLabel->Caption = sect;
   RunCheckBox->Checked = tce->run;
   return true;
}
//---------------------------------------------------------------------------
void __fastcall TConfigElementFrm::RunCheckBoxClick(TObject *Sender)
{
   // action it instantly?   
}
//---------------------------------------------------------------------------

