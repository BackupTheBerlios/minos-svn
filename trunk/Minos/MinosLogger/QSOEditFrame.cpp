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

//---------------------------------------------------------------------------
__fastcall TGJVQSOEditFrame::TGJVQSOEditFrame( TComponent* Owner )
      : TGJVEditFrame( Owner )
{
   CatchupButton->Visible = false;
}
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
   if ( !checkLogEntry(true) )
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
   ShowMessage( "No more unfilled contacts found" );
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
   if ( !checkLogEntry(true) )
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
   if ( !checkLogEntry(true) )
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
   MinosLoggerEvents::SendAfterLogContact(contest);
}
//==============================================================================
void TGJVQSOEditFrame::logCurrentContact( )
{
   if ( contest->isReadOnly() )
      return ;

   getScreenEntry();
   screenContact.op1 = MainOpComboBox->Text.c_str();
   screenContact.op2 = SecondOpComboBox->Text.c_str();

   screenContact.contactFlags &= ~TO_BE_ENTERED;

   logScreenEntry( );
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::selectEntry( BaseContact *lct )
{
   selectedContact = lct;   // contact from log list selected

   if (contest->unfilledCount <= 0)
   {
      FirstUnfilledButton->Visible = false;
   }
   else
   {
      FirstUnfilledButton->Visible = true;
   }
   updateTimeAllowed = false;   // whatever the time says, leave it alone

   screenContact.copyFromArg( *lct );
   showScreenEntry();
   MainOpComboBox->Text = screenContact.op1.c_str();
   SecondOpComboBox->Text = screenContact.op2.c_str();
   if ( !contest->isReadOnly() && (screenContact.contactFlags & TO_BE_ENTERED || catchup))
   {
      // Uri Mode - catchuping QSOs from paper while logging current QSOs
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

   MinosLoggerEvents::SendAfterSelectContact(lct, contest);
   selectField( 0 );
}
//---------------------------------------------------------------------------
bool TGJVQSOEditFrame::doGJVOKButtonClick( TObject *Sender )
{
   getScreenEntry();
   screenContact.op1 = MainOpComboBox->Text.c_str();
   screenContact.op2 = SecondOpComboBox->Text.c_str();
   bool unfilled = screenContact.contactFlags & TO_BE_ENTERED ;
   bool OKok = TGJVEditFrame::doGJVOKButtonClick( Sender );
   if ( OKok && unfilled )
   {
      // If Uri mode then continue to the next...
      FirstUnfilledButtonClick( Sender );
      unfilled = screenContact.contactFlags & TO_BE_ENTERED;

      return !unfilled;   // so we don't close the dialog
   }
   return OKok;
}
//---------------------------------------------------------------------------

void TGJVQSOEditFrame::doGJVCancelButtonClick( TObject */*Sender*/ )
{
   catchup = false;
   checkLogEntry(false);
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
void TGJVQSOEditFrame::initialise( BaseContestLog * contest, bool bf )
{
   TGJVEditFrame::initialise(contest, /*edScreen,*/ bf);
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::MainOpComboBoxExit(TObject */*Sender*/)
{
   std::string op1 = MainOpComboBox->Text.c_str();
   if ( op1.size() )
   {
      contest->oplist.insert( op1 );
   }
   contest->commonSave(false);
   refreshOps();
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::SecondOpComboBoxExit(TObject */*Sender*/)
{
   std::string op2 = SecondOpComboBox->Text.c_str();
   if ( op2.size() )
   {
      contest->oplist.insert( op2 );
   }
   contest->commonSave(false);
   refreshOps();
}
//---------------------------------------------------------------------------

