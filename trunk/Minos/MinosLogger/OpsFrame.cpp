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

#include "OpsFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "BundleFrame"
#pragma resource "*.dfm"
TOperatorFrame *OperatorFrame;
//---------------------------------------------------------------------------
__fastcall TOperatorFrame::TOperatorFrame( TComponent* Owner )
      : TFrame( Owner )
{}
//---------------------------------------------------------------------------

void TOperatorFrame::refreshOps()
{
   MainOpComboBox->Clear();
   SecondOpComboBox->Clear();
   // refill the op combo boxes from the current contest, and select the correct op
   BaseContestLog * contest = TContestApp::getContestApp() ->getCurrentContest();

   for ( OperatorIterator i = contest->oplist.begin(); i != contest->oplist.end(); i++ )
   {
      if ( ( *i ).size() )
      {
         MainOpComboBox->Items->Add( ( *i ).c_str() );
         SecondOpComboBox->Items->Add( ( *i ).c_str() );
      }
   }
   MainOpComboBox->Text = contest->currentOp1.getValue().c_str();
   SecondOpComboBox->Text = contest->currentOp2.getValue().c_str();
}
//---------------------------------------------------------------------------

void __fastcall TOperatorFrame::MainOpComboBoxKeyPress( TObject */*Sender*/,
      char &Key )
{
   Key = toupper( Key );
}
//---------------------------------------------------------------------------


void __fastcall TOperatorFrame::MainOpComboBoxExit( TObject *Sender )
{
   MinosLoggerEvents::SendOp1Change(Trim(MainOpComboBox->Text).c_str());
   refreshOps();
}
//---------------------------------------------------------------------------

void __fastcall TOperatorFrame::SecondOpComboBoxExit(TObject *Sender)
{
   MinosLoggerEvents::SendOp2Change(Trim(MainOpComboBox->Text).c_str());
   refreshOps();
}
//---------------------------------------------------------------------------


