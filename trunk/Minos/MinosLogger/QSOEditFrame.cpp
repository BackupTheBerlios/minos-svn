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
void TGJVQSOEditFrame::updateQSODisplay()
{
   TGJVEditFrame::updateQSODisplay();

   DateEdit->Enabled = !contest->isReadOnly();
   TimeEdit->Enabled = !contest->isReadOnly();
   CallsignEdit->Enabled = !contest->isReadOnly();
   LocEdit->Enabled = !contest->isReadOnly();
   QTHEdit->Enabled = !contest->isReadOnly();
}
//---------------------------------------------------------------------------
void __fastcall TGJVQSOEditFrame::InsertBeforeButtonClick( TObject */*Sender*/ )
{
   BaseContact *pct = getPriorContact();
   LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );
   DisplayContestContact *newct = ct->addContactBetween(pct, selectedContact);
   newct->contactFlags.setValue(newct->contactFlags.getValue()|TO_BE_ENTERED);
   selectEntry(newct);
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::InsertAfterButtonClick( TObject */*Sender*/ )
{
   BaseContact *nct = getNextContact();
   LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );
   DisplayContestContact *newct = ct->addContactBetween(selectedContact, nct);
   newct->contactFlags.setValue(newct->contactFlags.getValue()|TO_BE_ENTERED);
   selectEntry(newct);
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
BaseContact *TGJVQSOEditFrame::getNextContact()
{
   for ( LogIterator i = contest->ctList.begin(); i != contest->ctList.end(); i++ )
   {
      if ( ( *i ) ->getLogSequence() == screenContact.getLogSequence() )
      {
         i++;
         if ( i != contest->ctList.end() )
         {
            return (*i);
         }
         return 0;
      }
   }
   return 0;
}

void __fastcall TGJVQSOEditFrame::NextButtonClick( TObject */*Sender*/ )
{
   current = 0;            // make sure the focus moves off this button
   if ( !checkLogEntry(true) )
   {
      return ;
   }
   BaseContact *lct = getNextContact();
   if ( lct )
   {
      selectEntry( lct );
   }
   else
   {
      ShowMessage( "End of QSOs" );
   }
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

   int ctmax = atoi( screenContact.serials.c_str() );

   if ( ctmax > contest->maxSerial )
      contest->maxSerial = ctmax;

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
void TGJVQSOEditFrame::selectEntry( BaseContact *slct )
{
   selectedContact = slct;   // contact from log list selected

   if (contest->unfilledCount <= 0 || contest->isReadOnly())
   {
      FirstUnfilledButton->Visible = false;
   }
   else
   {
      FirstUnfilledButton->Visible = true;
   }
//   updateTimeAllowed = false;   // whatever the time says, leave it alone

   screenContact.copyFromArg( *slct );
   showScreenEntry();

   PriorButton->Enabled = getPriorContact();
   NextButton->Enabled = getNextContact();
   InsertAfterButton->Enabled = getNextContact();  // dont allow insert after last contact

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
         BaseContact * pct = getPriorContact();
         if ( pct )
         {
            screenContact.time = pct->time;
            DateEdit->Text = screenContact.time.getDate( DTGDISP ).c_str();
            TimeEdit->Text = screenContact.time.getTime( DTGDISP ).c_str();
         }
         else
         {
            // use contest start time
            TDateTime DTGStart = CanonicalToTDT(contest->DTGStart.getValue().c_str());
            DateEdit->Text = DTGStart.FormatString("dd/mm/yyyy");
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

   MinosLoggerEvents::SendAfterSelectContact(slct, contest);
   selectField( 0 );
}
//---------------------------------------------------------------------------
bool TGJVQSOEditFrame::doGJVOKButtonClick( TObject *Sender )
{
   getScreenEntry();
   screenContact.op1 = MainOpComboBox->Text.c_str();
   screenContact.op2 = SecondOpComboBox->Text.c_str();
   bool unfilled = screenContact.contactFlags & TO_BE_ENTERED ;

   DateEdit->ReadOnly = !catchup;
   TimeEdit->ReadOnly = !catchup;

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
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::getScreenContactTime()
{
   screenContact.time.setDate( DateEdit->Text.c_str(), DTGDISP );
   screenContact.time.setTime( TimeEdit->Text.c_str(), DTGDISP );
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::showScreenContactTime( ScreenContact &temp )
{
   // display the contents of the contest->screenContact

   DateEdit->Text = temp.time.getDate( DTGDISP ).c_str();
   TimeEdit->Text = temp.time.getTime( DTGDISP ).c_str();
}
//==============================================================================
void TGJVQSOEditFrame::checkTimeEditEnter(TLabeledEdit *tle, bool &ovr)
{
   if ( tle == DateEdit || tle == TimeEdit )
   {
      ovr = true;
   }

}
//==============================================================================
void TGJVQSOEditFrame::checkTimeEditExit()
{
   bool tbe = screenContact.contactFlags & TO_BE_ENTERED;
   DateEdit->ReadOnly = !catchup && !tbe;
   TimeEdit->ReadOnly = !catchup && !tbe;
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
   dateIl = new ValidatedControl( DateEdit, vtDate );
   vcs.push_back( dateIl );
   timeIl = new ValidatedControl( TimeEdit, vtTime );
   vcs.push_back( timeIl );
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::MainOpComboBoxExit(TObject */*Sender*/)
{
   std::string op1 = MainOpComboBox->Text.c_str();
   if ( op1.size() )
   {
      contest->oplist.insert( op1 );
   }
   if (catchup)
   {
      contest->currentOp1.setValue(op1);
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
   if (catchup)
   {
      contest->currentOp2.setValue(op2);
   }
   contest->commonSave(false);
   refreshOps();
}
//---------------------------------------------------------------------------


void __fastcall TGJVQSOEditFrame::TimeEditDblClick(TObject */*Sender*/)
{
   if ( contest->isReadOnly() )
   {
      return ;
   }
   updateTimeAllowed = false;
   TimeEdit->ReadOnly = false;
   updateQSOTime();
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::DateEditDblClick(TObject */*Sender*/)
{
   if ( contest->isReadOnly() )
   {
      return ;
   }
   updateTimeAllowed = false;
   DateEdit->ReadOnly = false;
   updateQSOTime();
}
//---------------------------------------------------------------------------
bool TGJVQSOEditFrame::isTimeEdit(TLabeledEdit */*tle*/)
{
   return false;
}
//---------------------------------------------------------------------------
void __fastcall TGJVQSOEditFrame::TimeEditKeyPress(TObject *Sender, char &Key)
{
   if ( Key == '+' || Key == '-' )
   {
      dtg thisdtg( false );
      thisdtg.setDate( DateEdit->Text.c_str(), DTGDISP );
      thisdtg.setTime( TimeEdit->Text.c_str(), DTGDISP );
      time_t contactTime;
      thisdtg.getDtg( contactTime );
      if ( Key == '+' )
      {
         contactTime += 60;
      }
      else
         if ( Key == '-' )
         {
            contactTime -= 60;
         }
      thisdtg.setDtg( contactTime );
      DateEdit->Text = thisdtg.getDate( DTGDISP ).c_str();
      TimeEdit->Text = thisdtg.getTime( DTGDISP ).c_str();
   }
   GJVEditKeyPress( Sender, Key );
   if ( Key == '+' || Key == '-' )
   {
      Key = 0;
   }

}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::DateEditChange(TObject */*Sender*/)
{
   if ( updateTimeAllowed || contest->isReadOnly() )
   {
      return ;
   }
   DateEdit->ReadOnly = false;
}
//---------------------------------------------------------------------------

void __fastcall TGJVQSOEditFrame::TimeEditChange(TObject */*Sender*/)
{
   if ( updateTimeAllowed || contest->isReadOnly() )
   {
      return ;
   }
   TimeEdit->ReadOnly = false;
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::selectField( TWinControl *v )
{
   int dtgne = -1;

   if ( catchup )
   {
      dtgne = screenContact.time.notEntered();
   }

   if ( v == 0 )
   {
      if ( contest->isReadOnly() )
      {
         v = CallsignEdit;
      }
      else
         if ( catchup )
         {
            v = ( ( dtgne != -1 ) && ( dtgne <= 1 ) ) ? DateEdit : TimeEdit;
         }
         else
            if ( screenContact.contactFlags & TO_BE_ENTERED )
               v = TimeEdit;
            else
               v = CallsignEdit;

   }
   if ( !v || ( current == v ) )
   {
      if (v)
         v->SetFocus();
      return ;
   }

   if ( ( current == CallsignEdit ) || ( current == LocEdit ) )
   {
      valid( cmCheckValid ); // make sure all single and cross field
      doAutofill();
   }

   if ( v == TimeEdit )
   {
      ( ( TLabeledEdit * ) v ) ->ReadOnly = false;
      ( ( TLabeledEdit * ) v ) ->Color = clWindow;
      if (dtgne == 0 || dtgne == 1)
      {
         TimeEdit->SelStart = 0;
         TimeEdit->SelLength = 1;
      }
   }
   if ( v == DateEdit )
   {
      ( ( TLabeledEdit * ) v ) ->ReadOnly = false;
      ( ( TLabeledEdit * ) v ) ->Color = clWindow;
      if (dtgne == 0 || dtgne == 2)
      {
         DateEdit->SelStart = 0;
         DateEdit->SelLength = 1;
      }
   }
   if ( v == SerTXEdit )
   {
      ( ( TLabeledEdit * ) v ) ->ReadOnly = false;
      ( ( TLabeledEdit * ) v ) ->Color = clWindow;
   }
   if ( v->Enabled )
   {
      v->SetFocus();
      current = v;
   }
   MinosLoggerEvents::SendShowErrorList();
}
//---------------------------------------------------------------------------
void TGJVQSOEditFrame::setDTGNotValid(ScreenContact *vcct)
{
   // date
   // time
   int tne = vcct->time.notEntered(); // partial dtg will give fe
   // full dtg gives -ve, none gives 0
   if ( tne == 1 )
      dateIl->tIfValid = false;
   if ( tne == 2 )
      timeIl->tIfValid = false;
}
//---------------------------------------------------------------------------

