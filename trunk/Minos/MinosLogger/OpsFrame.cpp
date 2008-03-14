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
      : TFrame( Owner ), op1Value( 0 ), op2Value( 0 ), lastFocused( false )
{}
//---------------------------------------------------------------------------
void __fastcall TOperatorFrame::MainOpComboBoxChange( TObject */*Sender*/ )
{
   if ( op1Value->getValue() != MainOpComboBox->Text.c_str() )
   {
      op1Value->setValue( MainOpComboBox->Text.c_str() );
   }
}
//---------------------------------------------------------------------------

void __fastcall TOperatorFrame::SecondOpComboBoxChange( TObject */*Sender*/ )
{
   if ( op2Value->getValue() != SecondOpComboBox->Text.c_str() )
   {
      op2Value->setValue( SecondOpComboBox->Text.c_str() );
   }
}
//---------------------------------------------------------------------------
void TOperatorFrame::refreshOps()
{
   MainOpComboBox->Clear();
   SecondOpComboBox->Clear();
   // refill the op combo boxes from the curent contest, and select the correct op
   BaseContestLog * contest = TContestApp::getContestApp() ->getCurrentContest();

   for ( OperatorIterator i = contest->oplist.begin(); i != contest->oplist.end(); i++ )
   {
      if ( ( *i ).size() )
      {
         MainOpComboBox->Items->Add( ( *i ).c_str() );
         SecondOpComboBox->Items->Add( ( *i ).c_str() );
      }
   }
   MainOpComboBox->Text = contest->op1.getValue().c_str();
   SecondOpComboBox->Text = contest->op2.getValue().c_str();
}
//---------------------------------------------------------------------------

void __fastcall TOperatorFrame::MainOpComboBoxKeyPress( TObject */*Sender*/,
      char &Key )
{
   Key = toupper( Key );
}
//---------------------------------------------------------------------------

void __fastcall TOperatorFrame::OpChangeTimerTimer( TObject */*Sender*/ )
{
   //   TWinControl *active = LogContainer->ActiveControl;
   // Check if we have lost focus - if so update any changes
   if ( lastFocused && !MainOpComboBox->Focused() && !SecondOpComboBox->Focused() )
   {
      // Need to update main contest
      BaseContestLog * contest = TContestApp::getContestApp() ->getCurrentContest();

      std::string op1 = trim( MainOpComboBox->Text.c_str() );
      contest->op1.setValue( op1 );
      if ( op1.size() )
      {
         contest->oplist.insert( op1 );
      }
      std::string op2 = trim( SecondOpComboBox->Text.c_str() );
      contest->op2.setValue( op2 );
      if ( op2.size() )
      {
         contest->oplist.insert( op2 );
      }
      refreshOps();
      lastFocused = false;
   }
   if ( MainOpComboBox->Focused() || SecondOpComboBox->Focused() )
   {
      lastFocused = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TOperatorFrame::OpComboBoxExit( TObject *Sender )
{
   OpChangeTimerTimer( Sender );
}
//---------------------------------------------------------------------------

