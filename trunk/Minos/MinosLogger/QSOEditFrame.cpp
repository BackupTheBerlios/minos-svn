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

#include "gridhint.h"
#include "LogFrame.h"
#include "LogMain.h"
#include "QSOEditFrame.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "QSOFrame"
#pragma link "BundleFrame"
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
__fastcall TGJVQSOEditFrame::TGJVQSOEditFrame( TComponent* Owner )
      : TGJVEditFrame( Owner ), op1Value( 0 ), op2Value( 0 )
{}
//---------------------------------------------------------------------------
void __fastcall TGJVQSOEditFrame::InsertBeforeButtonClick( TObject */*Sender*/ )
{
#warning Insert Before not yet implemented
   ShowMessage( "Insert Before not yet implemented" );
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::InsertAfterButtonClick( TObject */*Sender*/ )
{
#warning Insert After not yet implemented
   ShowMessage( "Insert After not yet implemented" );
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::FirstUnfilledButtonClick( TObject */*Sender*/ )
{
   current = 0;            // make sure the focus moves off this button
   if ( !checkLogEntry() )
   {
      return ;
   }
   for ( LogIterator i = contest->ctList.begin(); i != contest->ctList.end(); i++ )
   {
      if ( ( *i ) ->contactFlags.getValue() & TO_BE_ENTERED )
      {
         selectEntry( ( *i ) );
         return ;
      }
   }
   ShowMessage( "No unfilled contacts found" );
}
//---------------------------------------------------------------------------
BaseContact *TGJVQSOEditFrame::getPriorContact()
{
   for ( LogIterator i = contest->ctList.begin(); i != contest->ctList.end(); i++ )
   {
      if ( ( *i ) ->getLogSequence() == screenContact.getLogSequence() )
      {
         if ( i != contest->ctList.begin() )
         {
            i--;
            return ( *i ) ;
         }
         return 0;
      }
   }
   return 0;
}


void __fastcall TGJVQSOEditFrame::PriorButtonClick( TObject */*Sender*/ )
{
   current = 0;            // make sure the focus moves off this button
   if ( !checkLogEntry() )
   {
      return ;
   }
   BaseContact *lct = getPriorContact();
   if ( lct )
   {
      selectEntry( lct );
   }
   else
   {
      ShowMessage( "Start of QSOs" );
   }
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::NextButtonClick( TObject */*Sender*/ )
{
   current = 0;            // make sure the focus moves off this button
   if ( !checkLogEntry() )
   {
      return ;
   }
   for ( LogIterator i = contest->ctList.begin(); i != contest->ctList.end(); i++ )
   {
      if ( ( *i ) ->getLogSequence() == screenContact.getLogSequence() )
      {
         i++;
         if ( i == contest->ctList.end() )
         {
            ShowMessage( "End of QSOs" );
            i--;
         }
         selectEntry( ( *i ) );
         return ;
      }
   }
   ShowMessage( "End of QSOs" );
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::killPartial( void )
{
   // no partials on edit screen
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::logScreenEntry( )
{
   if ( contest->isReadOnly() )
      return ;

   selectedContact->copyFromArg( screenContact );
   selectedContact->commonSave();
   editScreen->afterLogContact();
}
//==============================================================================
void TGJVQSOEditFrame::logCurrentContact( )
{
   if ( contest->isReadOnly() )
      return ;

   getScreenEntry();

   screenContact.contactFlags &= ~TO_BE_ENTERED;

   logScreenEntry( );
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::selectEntry( BaseContact *lct )
{
   selectedContact = lct;   // contact from log list selected

   updateTimeAllowed = false;   // whatever the time says, leave it alone

   screenContact.copyFromArg( *lct );
   showScreenEntry();
   if ( !contest->isReadOnly() && screenContact.contactFlags & TO_BE_ENTERED )
   {
      // Uri Mode - backfilling QSOs from paper while logging current QSOs
      // and we need to set the date/time from the previous contact
      TimeEdit->ReadOnly = false;
      DateEdit->ReadOnly = false;

      int tne = screenContact.time.notEntered(); // partial dtg will give +fe
      // full dtg gives -ve, none gives 0
      if ( tne == 0 )
      {
         BaseContact * lct = getPriorContact();
         if ( lct )
         {
            screenContact.time = lct->time;
            DateEdit->Text = screenContact.time.getDate( DTGDISP ).c_str();
            TimeEdit->Text = screenContact.time.getTime( DTGDISP ).c_str();
         }
         else
         {
            // use contest start time
            TDateTime DTGStart = CanonicalToTDT(contest->DTGStart.getValue().c_str());
            DateEdit->Text = DTGStart.FormatString("dd/mm/yy");
            TimeEdit->Text = DTGStart.FormatString("hh:nn");
            dtg time(false);
            time.setDate( DateEdit->Text.c_str(), DTGDISP );
            time.setTime( TimeEdit->Text.SubString(1, 5).c_str(), DTGDISP );
            screenContact.time = time;
         }
         int selpt = DateEdit->Text.Length();
         if ( selpt > 2 )
         {
            DateEdit->SelStart = 1;
            DateEdit->SelLength = 1;
         }

         selpt = TimeEdit->Text.Length();
         if ( selpt > 0 )
         {
            TimeEdit->SelStart = selpt - 1;
            TimeEdit->SelLength = 1;
         }
      }
   }
   else
   {
      TimeEdit->ReadOnly = true;
      DateEdit->ReadOnly = true;
   }
   SerTXEdit->ReadOnly = true;

   bool timeOK = false;
   int tne = screenContact.time.notEntered(); // partial dtg will give +fe
   // full dtg gives -ve, none gives 0

   if (tne < 0)
   {
      dtg time(false);
      time.setDate( DateEdit->Text.c_str(), DTGDISP );
      time.setTime( TimeEdit->Text.SubString(1, 5).c_str(), DTGDISP );

      timeOK = contest->checkTime(time);
   }

   if (timeOK)
   {
      DateEdit->Font->Color = clWindowText;
      TimeEdit->Font->Color = clWindowText;
   }
   else
   {
      DateEdit->Font->Color = clRed;
      TimeEdit->Font->Color = clRed;
   }

   editScreen->afterSelectEntry( lct );
   selectField( 0 );
}
//---------------------------------------------------------------------------
bool TGJVQSOEditFrame::doGJVOKButtonClick( TObject *Sender )
{
   getScreenEntry();
   bool unfilled = screenContact.contactFlags & TO_BE_ENTERED ;
   bool OKok = TGJVEditFrame::doGJVOKButtonClick( Sender );
   if ( OKok && unfilled )
   {
      // If Uri mode then continue to the next...
      FirstUnfilledButtonClick( Sender );
      unfilled = screenContact.contactFlags & TO_BE_ENTERED;

      // somehow we need to update the log screen tree - how do we find it?
      TSingleLogFrame *frame = LogContainer->findCurrentLogFrame();
      if ( frame )
      {
         frame->updateTrees();
      }
      return !unfilled;   // so we don't close the dialog
   }
   return OKok;
}
//---------------------------------------------------------------------------

bool TGJVQSOEditFrame::doGJVCancelButtonClick( TObject */*Sender*/ )
{
   if ( checkLogEntry() )
   {
      DateEdit->ReadOnly = !contest->isPostEntry();
      TimeEdit->ReadOnly = !contest->isPostEntry();
      SerTXEdit->ReadOnly = true;

      return true;
   }

   return false;
}

//==============================================================================

void TGJVQSOEditFrame::doGJVEditChange( TObject *Sender )
{

   if ( current == LocEdit || Sender == LocEdit )
   {
      getScreenEntry();
      // force bearing calc
      calcLoc();
   }
}
//==============================================================================

void TGJVQSOEditFrame::updateQSOTime()
{
   if ( DateEdit->ReadOnly )
   {
      DateEdit->Color = clBtnFace;
   }
   else
   {
      DateEdit->Color = clWindow;
   }
   if ( TimeEdit->ReadOnly )
   {
      TimeEdit->Color = clBtnFace;
   }
   else
   {
      TimeEdit->Color = clWindow;
   }
   if ( SerTXEdit->ReadOnly )
   {
      SerTXEdit->Color = clBtnFace;
   }
   else
   {
      SerTXEdit->Color = clWindow;
   }
}
//---------------------------------------------------------------------------


void __fastcall TGJVQSOEditFrame::ROFieldTimerTimer( TObject */*Sender*/ )
{
   updateQSOTime();
}
//---------------------------------------------------------------------------


void __fastcall TGJVQSOEditFrame::MainOpComboBoxChange( TObject */*Sender*/ )
{
   if ( op1Value )
   {
      op1Value->setValue( MainOpComboBox->Text.c_str() );
   }
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::SecondOpComboBoxChange( TObject */*Sender*/ )
{
   if ( op2Value )
   {
      op2Value->setValue( SecondOpComboBox->Text.c_str() );
   }
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::MainOpComboBoxKeyPress( TObject */*Sender*/,
      char &Key )
{
   Key = toupper( Key );
}
//---------------------------------------------------------------------------

