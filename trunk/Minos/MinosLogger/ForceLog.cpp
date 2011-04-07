/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop

#include "ForceLog.h"
#include "loccalc.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForceLogDlg *ForceLogDlg;
//---------------------------------------------------------------------------
__fastcall TForceLogDlg::TForceLogDlg( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TForceLogDlg::OKButtonClick( TObject */*Sender*/ )
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TForceLogDlg::LocCalcButtonClick( TObject */*Sender*/ )
{
   std::auto_ptr <TLocCalcForm> loccalc( new TLocCalcForm( this ) );
   loccalc->S1Loc->Text = trim( TContestApp::getContestApp() ->getCurrentContest() ->myloc.loc.getValue() ).c_str();
   if ( loccalc->ShowModal() == mrOk )
   {
      CheckBox4->Checked = false;	// Scored - so not non-scoring!
      ScoreIl->Text = loccalc->Distance->Text.Trim();
   }
   return ;
}
//---------------------------------------------------------------------------
void __fastcall TForceLogDlg::CancelButtonClick( TObject */*Sender*/ )
{
   ModalResult = mrCancel;
}

void __fastcall TForceLogDlg::FormShow( TObject */*Sender*/ )
{
   MinosParameters::getMinosParameters() ->applyFontMultiplier(this);
   //   ScaleBy( TContestApp::getContestApp() ->sysfont->Size, Font->Size );
   //   Font->Assign( TContestApp::getContestApp() ->sysfont );
   CheckBox3->SetFocus();
}
//---------------------------------------------------------------------------

