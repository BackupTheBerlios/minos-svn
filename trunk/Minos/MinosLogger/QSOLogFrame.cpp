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
#include "LoggerContest.h"

#include "SendRPCDM.h"
#include "ServerEvent.h"
#include "MatchContact.h"
#include "MatchThread.h"
#include "QSOLogFrame.h"
#include "QSOEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "QSOFrame"
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
__fastcall TGJVQSOLogFrame::TGJVQSOLogFrame( TComponent* Owner )
      : TGJVEditFrame( Owner ), partialContact( 0 )
{}
__fastcall TGJVQSOLogFrame::~TGJVQSOLogFrame()
{
   delete partialContact;
}
void TGJVQSOLogFrame::initialise( BaseContestLog * contest, QSOEditScreen *edScreen )
{
   TGJVEditFrame::initialise( contest, edScreen );
   BandMapPanel->Visible = checkServerReady();
   editScreen->reportOverstrike( overstrike );
}
//---------------------------------------------------------------------------
void TGJVQSOLogFrame::logScreenEntry( )
{
   if ( contest->isReadOnly() )
      return ;

   int ctmax = atoi( screenContact.serials.c_str() );

   if ( ctmax > contest->maxSerial )
      contest->maxSerial = ctmax;

   // NB contact will be saved - soon

   // will be used to hold the screen contact
   LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );
   if ( !ct )
   {
      return ;
   }
   DisplayContestContact *lct = ct->addContact( ctmax, 0, false );	// "current" doesn't get flag, don't save ContestLog yet

   bool setTime = ct->isPostEntry();

   dtg saved( screenContact.time );		// dtg from current contact

   bool contactmodeCW = ( screenContact.reps.size() == 3 && screenContact.repr.size() == 3 );
   bool curmodeCW = ( stricmp( screenContact.mode, "A1A" ) == 0 );

   if ( contactmodeCW != curmodeCW )
   {
      // ask if change...
      if ( !curmodeCW )
      {
         if ( MinosParameters::getMinosParameters() ->yesNoMessage( this, "Change mode to A1A?" ) )
         {
            screenContact.mode = "A1A";
         }
      }
      else
      {
         if ( MinosParameters::getMinosParameters() ->yesNoMessage( this, "Change mode to J3E?" ) )
         {
            screenContact.mode = "J3E";
         }
      }
   }
   ct->mode.setValue( screenContact.mode );
   screenContact.op1.setValue( ct->op1.getValue() );
   screenContact.op2.setValue( ct->op2.getValue() );

   lct->copyFromArg( screenContact );
   ContestContact *llct = dynamic_cast<ContestContact *>( lct );
   llct->commonSave();				// which also saves the ContestLog

   selectEntry( 0 );	// select the "next"
   if ( setTime )
   {
      updateTimeAllowed = false;
      updateQSOTime();
      screenContact.time = saved;
      DateEdit->Text = screenContact.time.getDate( DTGDISP ).c_str();
      int selpt = DateEdit->Text.Length();
      if ( selpt > 2 )
      {
         DateEdit->SelStart = 1;
         DateEdit->SelLength = 1;
      }

      TimeEdit->Text = screenContact.time.getTime( DTGDISP ).c_str();
      selpt = TimeEdit->Text.Length();
      if ( selpt > 0 )
      {
         TimeEdit->SelStart = selpt - 1;
         TimeEdit->SelLength = 1;
      }
      selectField( 0 );
   }
   editScreen->afterLogContact();
}
//==============================================================================
void TGJVQSOLogFrame::logCurrentContact( )
{
   if ( contest->isReadOnly() )
      return ;

   // copy the display into the correct log entry, etc

   getScreenEntry();

   // first check for extra lines to be added
   int ctno = atoi( screenContact.serials.c_str() );
   if ( ctno > contest->maxSerial + 1 )
   {

      if ( MessageBox( Handle, "Serial sent is too high - should I create the missing contacts?",
                       "Minos Logger", MB_ICONQUESTION | MB_OKCANCEL ) == IDOK )
      {
         int orflag = 0;

         if ( MessageBox( Handle, "Do you want to enter these contacts later?", "Minos Logger", MB_ICONQUESTION | MB_YESNO ) == IDYES )
         {
            orflag = TO_BE_ENTERED;
         }

         int nct_no = contest->maxSerial + 1;
         do
         {
            // last child is "current contact", and we need to add TO IT
            LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );
            ct->addContact( nct_no, orflag, true ); // last contact
            nct_no++;
         }
         while ( nct_no < ctno ) ;
      }
      else
      {
         // reset focus to serial sent
         //????????????? But its disabled...
         selectField( SerTXEdit );
         return ;
      }
   }

   logScreenEntry( );  // true => move on to next contact
}
//==============================================================================
void TGJVQSOLogFrame::selectEntry( BaseContact *plct )
{
   if ( plct )
   {
      // Non zero plct happens if we switch contests by double clicking on a match
      editScreen->afterSelectEntry( plct );
      return ;
   }

   else
      if ( !restorePartial() )
      {
         updateTimeAllowed = true;
         screenContact.initialise( contest );
         if ( contest->isPostEntry() )
         {
            // need to get previous contact dtg, or if not, set as now
            if ( contest->getContactCount() > 1 )
            {
               BaseContact * pct = contest->pcontactAt( contest->getContactCount() - 1 ) ;
               if ( pct )
               {
                  screenContact.time = pct->time;
                  int tne = screenContact.time.notEntered(); // partial dtg will give fe
                  // full dtg gives -ve, none gives 0
                  if ( tne != 0 )
                  {
                     updateTimeAllowed = FALSE;
                  }
               }
            }
         }
      }

   updateQSOTime();
   showScreenEntry();

   if ( !plct && contest->isPostEntry() )
   {
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

   editScreen->afterSelectEntry( plct );
}
bool TGJVQSOLogFrame::doGJVCancelButtonClick( TObject */*Sender*/ )
{
   DateEdit->ReadOnly = !contest->isPostEntry();
   TimeEdit->ReadOnly = !contest->isPostEntry();
   SerTXEdit->ReadOnly = true;

   ScreenContact *temp = 0;
   if ( !partialContact )
   {
      savePartial();
      temp = partialContact;
      partialContact = 0;
   }
   updateTimeAllowed = true;
   updateQSOTime();
   selectEntry( 0 );
   CallsignEdit->SetFocus();

   if ( temp )
      partialContact = temp;

   MinosParameters::getMinosParameters() ->showErrorList( );
   return true;
}
//==============================================================================
bool TGJVQSOLogFrame::savePartial()
{
   if ( !partialContact )
   {
      getScreenEntry();
      partialContact = new ScreenContact( );
      partialContact->copyFromArg( screenContact );
      return true;
   }
   return false;
}
bool TGJVQSOLogFrame::restorePartial( void )
{
   if ( partialContact )
   {
      screenContact.copyFromArg( *( partialContact ) );
      showScreenEntry();

      killPartial();
      return true;
   }
   return false;
}
void TGJVQSOLogFrame::killPartial( void )
{
   if ( partialContact )
   {
      delete partialContact;
      partialContact = 0;
   }
   TMatchThread::startMatch();
}
//==============================================================================
void TGJVQSOLogFrame::doGJVEditChange( TObject *Sender )
{
   // sensitive field changed - trigger match scan
   if ( contest )
   {
      getScreenEntry();

      if ( current == CallsignEdit || Sender == CallsignEdit )
      {
         // clear the error list
         contest->DupSheet.clearCurDup();	// as edited, no longer a dup(?)
      }
      TMatchThread::startMatch();
      if ( current == LocEdit || Sender == LocEdit )
      {
         // force bearing calc
         calcLoc();
      }
   }
}
//==============================================================================
void TGJVQSOLogFrame::transferDetails( const BaseContact * lct, const BaseContestLog *matct )
{
   CallsignEdit->Text = lct->cs.fullCall.getValue().c_str();
   LocEdit->Text = lct->loc.loc.getValue().c_str();  // also forces update of score etc

   // only transfer qth info if required for this ContestLog
   // and it might be valid...
   if ( contest->districtMult.getValue() || contest->otherExchange.getValue() )
   {
      if ( ( contest->districtMult.getValue() && matct->districtMult.getValue() ) ||
           ( contest->otherExchange.getValue() && matct->otherExchange.getValue() )
         )
      {
         if ( contest->QTHField.getValue() )
            QTHEdit->Text = lct->extraText.getValue().c_str();
      }
   }
   valid( cmCheckValid ); // make sure all single and cross field
   // validation has been done

   // make sure dtg gets entered if needed
   CallsignEdit->SetFocus();
   doAutofill();
   CallsignEdit->SetFocus();
}
void TGJVQSOLogFrame::transferDetails( const ListContact * lct, const ContactList */*matct*/ )
{
   CallsignEdit->Text = lct->cs.fullCall.getValue().c_str();
   LocEdit->Text = lct->loc.loc.getValue().c_str();  // also forces update of score etc

   valid( cmCheckValid ); // make sure all single and cross field
   // validation has been done

   // make sure dtg gets entered if needed
   CallsignEdit->SetFocus();
   doAutofill();
   CallsignEdit->SetFocus();
}
//==============================================================================
void TGJVQSOLogFrame::setFreq( String f )
{
   sCurrFreq = f;
   bool temp;
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpAutoBandMapTune, temp );
   if ( temp )
      BandMapButtonClick( this );
}

void __fastcall TGJVQSOLogFrame::BandMapButtonClick( TObject *Sender )
{
   getScreenEntry();

   std::string call = screenContact.cs.fullCall.getValue();
   std::string loc = screenContact.loc.loc.getValue();
   std::string utc = dtg( true ).getIsoDTG();
   std::string qth = screenContact.extraText;
   std::string freq = sCurrFreq.c_str();

   if ( call.size() || loc.size() || qth.size() )
   {
      // Transfer to band map app
      TSendDM::sendBandMap( freq, call, utc, loc, qth );
      doGJVCancelButtonClick( Sender ); // equivalent of hitting ESC
   }

}
//---------------------------------------------------------------------------

void TGJVQSOLogFrame::updateQSOTime()
{
   // Check if dtg is within the contest time
   // If not we wish to show as red
   // We need to do this in log displays as well

   if ( !contest->isPostEntry() && updateTimeAllowed )
   {
      static bool tick = true;
      dtg tnow( true );
      DateEdit->Color = clBtnFace;
      TimeEdit->Color = clBtnFace;
      DateEdit->Text = tnow.getDate( DTGDISP ).c_str();
      std::string t = tnow.getTime( DTGDISP ) + ( tick ? ':' : ' ' );
      TimeEdit->Text = t.c_str();
      tick = !tick;
   }
   else
   {
      TimeEdit->Text = TimeEdit->Text.SubString( 1, 5 );   // take off any :
      DateEdit->Color = clWindow;
      TimeEdit->Color = clWindow;
   }
   dtg time(false);
   time.setDate( DateEdit->Text.c_str(), DTGDISP );
   time.setTime( TimeEdit->Text.SubString(1, 5).c_str(), DTGDISP );

   bool timeOK = contest->checkTime(time);

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
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOLogFrame::BackfillButtonClick(TObject */*Sender*/)
{
// Kick off Post Entry/Uri/Backfill
// We need to create a new contact, and set the "post entry" flag
// and then trigger the qso edit dialog on it

   LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );

   #warning we aren't getting a correct maxSerial 
   int ctmax = contest->maxSerial;

   DisplayContestContact *lct = ct->addContact( ctmax, 0, false );	// "current" doesn't get flag, don't save ContestLog yet

   std::auto_ptr <TQSOEditDlg> qdlg( new TQSOEditDlg( this ) );
   ContestContact *dct = dynamic_cast<ContestContact *>( lct );
   qdlg->selectContact( ct, dct );

   qdlg->ShowModal();
   editScreen->afterLogContact();

   ct->startScan();
//   LogMonitor->Invalidate();
//   MultDispFrame->refreshMults();
//   OperatorFrame->refreshOps();
//   LogMonitor->Repaint();
   selectField( 0 );
}
//---------------------------------------------------------------------------

