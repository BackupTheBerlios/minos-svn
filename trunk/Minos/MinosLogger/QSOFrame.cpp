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
#include "LogEvents.h"

#include "ForceLog.h"
#include "enqdlg.h"

#include "QSOFrame.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
__fastcall TGJVEditFrame::TGJVEditFrame( TComponent* Owner )
      : TFrame( Owner ), selectedContact( 0 ),
      updateTimeAllowed( true ), overstrike( false ),
      EL_ValidateError ( EN_ValidateError, & ValidateError_Event )
{
   Parent = ( TWinControl * ) Owner;               // This makes the JEDI splitter work!
}
__fastcall TGJVEditFrame::~TGJVEditFrame()
{
   for ( std::vector <ValidatedControl *>::iterator vcp = vcs.begin(); vcp != vcs.end(); vcp++ )
   {
      delete ( *vcp );
   }
}
void TGJVEditFrame::initialise( BaseContestLog * pcontest, bool bf )
{
   catchup = bf;
//   editScreen = edScreen;
   contest = pcontest;
   screenContact.initialise( contest ); // get ops etc correct

   dateIl = new ValidatedControl( DateEdit, vtDate );
   vcs.push_back( dateIl );
   timeIl = new ValidatedControl( TimeEdit, vtTime );
   vcs.push_back( timeIl );
   csIl = new ValidatedControl( CallsignEdit, vtCallsign );
   vcs.push_back( csIl );
   rsIl = new ValidatedControl( RSTTXEdit, vtRST );
   vcs.push_back( rsIl );
   ssIl = new ValidatedControl( SerTXEdit, vtSN );
   vcs.push_back( ssIl );
   rrIl = new ValidatedControl( RSTRXEdit, vtRST );
   vcs.push_back( rrIl );
   srIl = new ValidatedControl( SerRXEdit, vtSN );
   vcs.push_back( srIl );
   locIl = new ValidatedControl( LocEdit, vtLoc );
   vcs.push_back( locIl );
   qthIl = new ValidatedControl( QTHEdit, vtQTH );
   vcs.push_back( qthIl );
   cmntIl = new ValidatedControl( CommentsEdit, vtComments );
   vcs.push_back( cmntIl );

   BrgSt->Caption = "";
   DistSt->Caption = "";

   if ( contest->isReadOnly() )
   {
      DateEdit->Enabled = false;
      TimeEdit->Enabled = false;
      //CallsignEdit->Enabled = false;
      RSTTXEdit->Enabled = false;
      SerTXEdit->Enabled = false;
      RSTRXEdit->Enabled = false;
      SerRXEdit->Enabled = false;
      //LocEdit->Enabled = false;
      //QTHEdit->Enabled = false;
      CommentsEdit->Enabled = false;
      ModeComboBoxGJV->Enabled = false;
      NonScoreCheckBox->Enabled = false;
      DeletedCheckBox->Enabled = false;
      GJVOKButton->Enabled = false;
      GJVForceButton->Enabled = false;
   }
   QTHEdit->Enabled = ( contest->otherExchange .getValue() || contest->districtMult.getValue() );

   updateQSODisplay();
   SerTXEdit->Color = clBtnFace;
   current = 0;
   updateTimeAllowed = true;
   updateQSOTime();
}
void TGJVEditFrame::ValidateError_Event ( MinosEventBase & Event )
{
		ActionEvent1<int, EN_ValidateError> & S = dynamic_cast<ActionEvent1<int, EN_ValidateError> &> ( Event );
      int mess_no = S.getData();
      if ( mess_no == -1 )
      {
         errs.clear();
         return ;
      }

      // add the message into the error list
      errs.insert( &errDefs[ mess_no ] );
}
void TGJVEditFrame::setActiveControl( WORD *Key )
{
   switch ( *Key )
   {
      case VK_F1:
         selectField( CallsignEdit );
         *Key = 0;
         break;
      case VK_F2:
         selectField( RSTTXEdit );
         *Key = 0;
         break;
      case VK_F3:
         selectField( RSTRXEdit );
         *Key = 0;
         break;
      case VK_F4:
         selectField( SerRXEdit );
         *Key = 0;
         break;
      case VK_F5:
         selectField( LocEdit );
         *Key = 0;
         break;
      case VK_F6:
         selectField( QTHEdit );
         *Key = 0;
         break;
   }
}
//---------------------------------------------------------------------------
void TGJVEditFrame::getScreenEntry()
{
   screenContact.time.setDate( DateEdit->Text.c_str(), DTGDISP );
   screenContact.time.setTime( TimeEdit->Text.c_str(), DTGDISP );

   screenContact.cs.fullCall.setValue( CallsignEdit->Text.Trim().c_str() );
   screenContact.cs.valRes = CS_NOT_VALIDATED;
   screenContact.cs.validate( );

   screenContact.reps = RSTTXEdit->Text.Trim().c_str();
   screenContact.serials = SerTXEdit->Text.Trim().c_str();
   screenContact.repr = RSTRXEdit->Text.Trim().c_str();
   screenContact.serialr = SerRXEdit->Text.Trim().c_str();

   screenContact.loc.loc.setValue( LocEdit->Text.Trim().c_str() );
   screenContact.bearing = -1;		// force a recalc
   screenContact.loc.validate();
   screenContact.extraText = QTHEdit->Text.Trim().c_str();
   screenContact.comments = CommentsEdit->Text.Trim().c_str();

   screenContact.mode = ModeComboBoxGJV->Text.Trim().c_str();
   screenContact.contactFlags &= ~NON_SCORING;

   // op1/op2 get set when the attached combos change - I hope :)

   if ( NonScoreCheckBox->Checked )
   {
      screenContact.contactFlags |= NON_SCORING;
   }
   screenContact.contactFlags &= ~DONT_PRINT;
   if ( DeletedCheckBox->Checked )
   {
      screenContact.contactFlags |= DONT_PRINT;
   }
}
//---------------------------------------------------------------------------
void TGJVEditFrame::showScreenEntry( void )
{
   // display the contents of the contest->screenContact

   if ( contest )
   {
      // we only validate this contact up to the validation point
      contest->validationPoint = selectedContact;
      ScreenContact temp;
      temp.copyFromArg( screenContact ); // as screen contact gets corrupted by auto changes
      // op1, op2 in ScreenContact ge corrupted as well
      DateEdit->Text = temp.time.getDate( DTGDISP ).c_str();
      TimeEdit->Text = temp.time.getTime( DTGDISP ).c_str();
      CallsignEdit->Text = trim( temp.cs.fullCall.getValue() ).c_str();
      RSTTXEdit->Text = trim( temp.reps ).c_str();
      SerTXEdit->Text = trim( temp.serials ).c_str();
      RSTRXEdit->Text = trim( temp.repr ).c_str();
      SerRXEdit->Text = trim( temp.serialr ).c_str();
      LocEdit->Text = trim( temp.loc.loc.getValue() ).c_str();  // also forces update of score etc
      QTHEdit->Text = trim( temp.extraText ).c_str();
      CommentsEdit->Text = trim( temp.comments ).c_str();
      ModeComboBoxGJV->Text = trim( temp.mode ).c_str();
      NonScoreCheckBox->Checked = temp.contactFlags & NON_SCORING;
      DeletedCheckBox->Checked = temp.contactFlags & DONT_PRINT;

      // and now we want to put the selection on each at the END of the text
      for ( std::vector <ValidatedControl *>::iterator vcp = vcs.begin(); vcp != vcs.end(); vcp++ )
      {
         int selpt = ( *vcp ) ->wc->Text.Length();
         ( *vcp ) ->wc->SelStart = selpt;
         ( *vcp ) ->wc->SelLength = 0;
      }

      valid( cmCheckValid ); // make sure contact is valid - display any errors
      if ( temp.contactFlags & ( COMMENT_ONLY | LOCAL_COMMENT ) )
      {
         selectField( CommentsEdit );
      }
      else
         selectField( 0 );

      SerTXEdit->Color = clBtnFace;
      SerTXEdit->ReadOnly = true;
      MinosLoggerEvents::SendShowErrorList();
      MinosLoggerEvents::SendScreenContactChanged(&screenContact, contest);
   }
}
//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::EditControlEnter( TObject *Sender )
{
   current = dynamic_cast<TWinControl *>(Sender);
   TLabeledEdit *tle = dynamic_cast<TLabeledEdit *>( current );
   if ( tle )
   {
      tle->EditLabel->Font->Color = clGreen;

   }
   bool ovr = overstrike;
   if ( tle == DateEdit || tle == TimeEdit )
   {
      ovr = true;
   }
   MinosLoggerEvents::SendReportOverstrike(ovr, contest);
}
//---------------------------------------------------------------------------

void __fastcall TGJVEditFrame::EditControlExit( TObject *Sender )
{
   if (!contest)
   {
      return;
   }
   bool tbe = screenContact.contactFlags & TO_BE_ENTERED;
   DateEdit->ReadOnly = !catchup && !tbe;
   TimeEdit->ReadOnly = !catchup && !tbe;
   SerTXEdit->ReadOnly = true;
   SerTXEdit->Color = clBtnFace;
   TLabeledEdit *tle = dynamic_cast<TLabeledEdit *>( current );
   if ( tle )
   {
      tle->EditLabel->Font->Color = clWindowText;
   }
   if ( current == LocEdit )
   {
      // do any required character substitutions, but only when we have a full
      // locator

      // This does something to correct sloppy typing

      String loc = LocEdit->Text.Trim();
      if (loc.Length() == 6 || loc.Length() == 8)
      {
         if (loc[1] == '1')
         {
            loc[1] = 'I';
         }
         if (loc[1] == '0')
         {
            loc[1] = 'O';
         }
         if (loc[2] == '1')
         {
            loc[2] = 'I';
         }
         if (loc[2] == '0')
         {
            loc[2] = 'O';
         }

         if (loc[5] == '1')
         {
            loc[5] = 'I';
         }
         if (loc[5] == '0')
         {
            loc[5] = 'O';
         }
         if (loc[6] == '1')
         {
            loc[6] = 'I';
         }
         if (loc[6] == '0')
         {
            loc[6] = 'O';
         }
         if (loc != LocEdit->Text.Trim())
         {
            LocEdit->Text = loc;
            doGJVEditChange( LocEdit );
         }
      }
   }
   if ( ( current == CallsignEdit ) || ( current == LocEdit ) )
   {
      GJVEditChange( Sender );   // start the match thread
      valid( cmCheckValid ); // make sure all single and cross field
      doAutofill();
   }
   MinosLoggerEvents::SendShowErrorList();
   MinosLoggerEvents::SendReportOverstrike(overstrike, contest);
   if (ModeComboBoxGJV->Text == "A1A")
   {
      ModeButton->Caption = "J3E";
   }
   else
   {
      ModeButton->Caption = "A1A";
   }
}
//---------------------------------------------------------------------------

void __fastcall TGJVEditFrame::EditKeyDown( TObject */*Sender*/, WORD &Key,
      TShiftState Shift )
{
   if (!contest)
   {
      return;
   }
   if ( ( Key == VK_INSERT ) && ( !Shift.Contains( ssShift ) ) )
      overstrike = !overstrike;

   // DTG need to be overstrike ALWAYS as they are formatted
   bool ovr = overstrike;

   TLabeledEdit *ed = dynamic_cast<TLabeledEdit *>( current );
   if ( ed == DateEdit || ed == TimeEdit )
   {
      ovr = true;
   }

   MinosLoggerEvents::SendReportOverstrike(ovr, contest);

   if ( ed->ReadOnly )
   {
      // want to pop up the hint
      MinosParameters::getMinosParameters() ->mshowMessage( ed->Hint, ed );
      return ;
   }

   if ( ed && ovr && Key == VK_DELETE )
   {
      // need to overstrike with space, but move back one
      if ( ed->SelLength == 0 )
      {
         ed->SelLength = 1;
      }

      String repText;
      int len = ed->SelLength;
      for ( int i = 0; i < len; i++ )
      {
         repText += ' ';
      }
      ed->SelText = repText;
      ed->SelStart = ed->SelStart - len ;

      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGJVEditFrame::DateEditDblClick( TObject */*Sender*/ )
{
   if ( contest->isReadOnly() )
   {
      return ;
   }
   updateTimeAllowed = false;
   updateQSOTime();
   DateEdit->ReadOnly = false;
}
//---------------------------------------------------------------------------

void __fastcall TGJVEditFrame::TimeEditDblClick( TObject */*Sender*/ )
{
   if ( contest->isReadOnly() )
   {
      return ;
   }
   updateTimeAllowed = false;
   updateQSOTime();
   TimeEdit->ReadOnly = false;
}

//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::SerTXEditDblClick( TObject */*Sender*/ )
{
   if ( contest->isReadOnly() )
   {
      return ;
   }
   SerTXEdit->ReadOnly = false;
   SerTXEdit->Color = clWindow;
}
//---------------------------------------------------------------------------

void __fastcall TGJVEditFrame::TimeEditKeyPress( TObject *Sender, char &Key )
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

void __fastcall TGJVEditFrame::GJVEditKeyPress( TObject *Sender, char &Key )
{
   if ( !iscntrl( Key ) )
   {
      killPartial(); // if a normal character, then lose
      // any partial data preserved.
   }

   TLabeledEdit *ed = dynamic_cast<TLabeledEdit *>( Sender );
   if ( !ed || ed->ReadOnly )
   {
      return ;
   }
   bool ovr = overstrike;
   if ( ed == DateEdit || ed == TimeEdit )
   {
      ovr = true;
   }
   if ( ovr && Key != 8 )
   {
      if ( ed->SelLength == 0 )
         ed->SelLength = 1;
   }
   else
      if ( ed && ovr && Key == 8 )
      {
         // need to overstrike with space, but move back one
         if ( ed->SelStart > 0 )
         {
            ed->SelStart = ed->SelStart - 1;
            ed->SelLength = 1;
            ed->SelText = " ";
            ed->SelStart = ed->SelStart - 1;
         }
         Key = 0;
      }
}
//---------------------------------------------------------------------------
bool TGJVEditFrame::doGJVOKButtonClick( TObject *Sender )
{
   DateEdit->ReadOnly = !catchup;
   TimeEdit->ReadOnly = !catchup;
   SerTXEdit->ReadOnly = true;
   SerTXEdit->Color = clBtnFace;

   if ( contest->isReadOnly() )
   {
      return true;
   }
   GJVEditChange( Sender );   // start the match thread

   getScreenEntry(); // make sure it is saved

   if ( screenContact.contactFlags & ( LOCAL_COMMENT | DONT_PRINT | COMMENT_ONLY ) )
   {
      logCurrentContact( );
      return true;
   }

   // validate the entry; if still invalid, spin round the invalid
   // controls (this should really be the job of tab, but...)

   TWinControl *currn = current;
   if ( !valid( cmCheckValid ) || ( currn == RSTTXEdit ) || ( currn == RSTRXEdit ) )
      // make sure all single and cross field
      // validation has been done
   {
      doAutofill();
   }
   bool was_unfilled = screenContact.contactFlags & TO_BE_ENTERED;
   if ( !valid( cmCheckValid ) )   // make sure all single and cross field
      // validation has been done
   {
      TWinControl * firstInvalid = 0;
      TWinControl *nextInvalid = 0;
      bool onCurrent = false;
      bool pastCurrent = false;
      for ( std::vector <ValidatedControl *>::iterator vcp = vcs.begin(); vcp != vcs.end(); vcp++ )
      {
         if ( ( *vcp ) ->wc->ReadOnly || !( *vcp ) ->wc->Visible )
         {
            continue;
         }
         if ( onCurrent )
            pastCurrent = true;
         if ( ( *vcp ) ->wc == current )
            onCurrent = true;
         if ( !( *vcp ) ->valid( cmValidStatus ) )
         {
            if ( !firstInvalid )
               firstInvalid = ( *vcp ) ->wc;
            if ( pastCurrent )
            {
               if ( !nextInvalid )
               {
                  nextInvalid = ( *vcp ) ->wc;
                  break;
               }
            }
         }
      }

      // make sure we go to the invalid field

      TWinControl *nextf = ( nextInvalid ) ? nextInvalid : firstInvalid;

      // but if it is DTG, probably want CS instead (Unless post entry)

      if ( ( nextf == TimeEdit ) || ( nextf == DateEdit ) )
      {
         if ( !catchup && screenContact.time.notEntered() == 0 )
         {
            selectField( CallsignEdit );
            nextf = 0;			// dont show silly errors!
            if ( screenContact.cs.validate( ) == CS_OK )
            {
               doAutofill();
               //??????????????????????
               return doGJVOKButtonClick( Sender );
            }
         }
      }

      if ( nextf )
      {
         if ( nextf == current )
         {
            if ( firstInvalid != nextf )
               selectField( firstInvalid );
            else
               dlgForced();   // repeated attack on same faulty field
         }
         else
            selectField( nextf );
      }
      // Show on errList on multdisp frame
      MinosLoggerEvents::SendShowErrorList();
      return false ;
   }
   else
   {
      screenContact.contactFlags &= ~( TO_BE_ENTERED | FORCE_LOG );
   }

   // all is OK (or we will have executed a return statement)
   // so do it!

   // we have to check if we need to save it
   // checkLogEntry does the log action as well

   if ( !was_unfilled && !catchup && selectedContact )  // AND if we are logging "current" then we don't want to do this
   {
      if ( !checkLogEntry(true) )  // if it is the same, then don't log
      {
         return false;
      }
   }
   else
   {
      logCurrentContact( );
   }
   selectField( 0 );             // make sure we move off the "Log" default button
   return true;
}
//---------------------------------------------------------------------------
bool TGJVEditFrame::dlgForced()
{
   std::auto_ptr <TForceLogDlg> ForceDlg( new TForceLogDlg( this ) );

   getScreenEntry();
   valid( cmCheckValid );       // This adds errors to the MAIN dialog error list, not our own

   for ( ErrorIterator i = errs.begin(); i != errs.end(); i++ )
   {
      ForceDlg->ErrList->Items->Add( ( *i ) ->errStr.c_str() );
   }
   ForceDlg->ErrList->ItemIndex = 0;

   int s = screenContact.contactScore;
   if ( s < 0 )
      s = 0;
   AnsiString temp;
   temp.printf( "%d", s );
   ForceDlg->ScoreIl->Text = temp;

   ForceDlg->CheckBox1->Checked = screenContact.contactFlags & TO_BE_ENTERED;
   ForceDlg->CheckBox2->Checked = screenContact.contactFlags & VALID_DUPLICATE;
   ForceDlg->CheckBox3->Checked = screenContact.contactFlags & MANUAL_SCORE;
   ForceDlg->CheckBox4->Checked = screenContact.contactFlags & NON_SCORING;
   ForceDlg->CheckBox5->Checked = screenContact.contactFlags & DONT_PRINT;
   ForceDlg->CheckBox6->Checked = screenContact.contactFlags & COUNTRY_FORCED;
   ForceDlg->CheckBox7->Checked = screenContact.contactFlags & VALID_DISTRICT;
   ForceDlg->CheckBox8->Checked = screenContact.contactFlags & XBAND;

   if ((screenContact.cs.valRes == ERR_DUPCS) ||
        ( screenContact.contactFlags & ( NON_SCORING | MANUAL_SCORE | DONT_PRINT | VALID_DUPLICATE | TO_BE_ENTERED | XBAND ) ) )

   {
      // set nothing! DUPs are dealt with!
   }
   else
      if ( errs.size() != 0 )  				// no errors -> OK
         ForceDlg->CheckBox4->Checked = screenContact.contactFlags | NON_SCORING;

   if ( screenContact.contactFlags & COUNTRY_FORCED )
   {
      ForceDlg->CtryMultIl->Text = screenContact.forcedMult.c_str();
   }
   else
      if ( contest->countryMult.getValue() && screenContact.ctryMult )
      {
         ForceDlg->CtryMultIl->Text = screenContact.ctryMult->basePrefix.c_str();
      }

   bool tryagain = true;
   int res = mrCancel;
   while ( tryagain && ( res = ForceDlg->ShowModal() ) == mrOk )
   {
      if ( contest->countryMult.getValue() )
      {
         temp = ForceDlg->CtryMultIl->Text;
      }
      else
         break;

      if ( !ForceDlg->CheckBox6->Checked )
      {
         tryagain = false;
         screenContact.contactFlags &= ~ COUNTRY_FORCED;
         screenContact.ctryMult = 0;
         screenContact.forcedMult = "";
         break;
      }

      temp = temp.Trim();

      CountryEntry *ctryMult = MultLists::getMultLists() ->getCtryForPrefix( temp.c_str() );
      if ( ctryMult )
      {
         tryagain = false;
         if ( screenContact.ctryMult != ctryMult )
         {
            screenContact.ctryMult = ctryMult;
            screenContact.contactFlags |= COUNTRY_FORCED;
            screenContact.forcedMult = temp.c_str();
         }
      }
      else
      {
         if ( MessageBox( Handle, "Country not in CTY.DAT. Leave for now?", "Force log contact", MB_ICONQUESTION | MB_OKCANCEL ) == mbOK )
         {
            tryagain = false;
            screenContact.contactFlags &= ~COUNTRY_FORCED;
            screenContact.forcedMult = "";
         }
      }
   }
   if ( res == mrOk )
   {
      // save contact...
      screenContact.contactFlags |= FORCE_LOG;
      // here read it all off the dialog

      screenContact.contactFlags &= ~( NON_SCORING | MANUAL_SCORE | DONT_PRINT | VALID_DUPLICATE | TO_BE_ENTERED | VALID_DISTRICT | XBAND );

      if ( ForceDlg->CheckBox1->Checked )
      {
         screenContact.contactFlags |= TO_BE_ENTERED;
      }
      if ( ForceDlg->CheckBox2->Checked )
      {
         screenContact.contactFlags |= VALID_DUPLICATE;
      }
      if ( ForceDlg->CheckBox3->Checked )
      {
         screenContact.contactFlags |= MANUAL_SCORE;
         temp = ForceDlg->ScoreIl->Text.Trim();
         screenContact.contactScore = atoi( temp.c_str() );
      }
      if ( screenContact.contactFlags & ( TO_BE_ENTERED | VALID_DUPLICATE | MANUAL_SCORE ) )
         ForceDlg->CheckBox4->Checked = false;

      if ( ForceDlg->CheckBox4->Checked )
         screenContact.contactFlags |= NON_SCORING;
      if ( ForceDlg->CheckBox5->Checked )
      {
         screenContact.contactFlags |= ( DONT_PRINT | NON_SCORING );
      }
      if ( ForceDlg->CheckBox7->Checked )
         screenContact.contactFlags |= VALID_DISTRICT;
      if ( ForceDlg->CheckBox8->Checked )
         screenContact.contactFlags |= XBAND;

      // make sure marked on main screen

      NonScoreCheckBox->Checked = screenContact.contactFlags & NON_SCORING;
      DeletedCheckBox->Checked = screenContact.contactFlags & DONT_PRINT;
      if ( screenContact.contactFlags & NON_SCORING )
         screenContact.multCount = 0;

      // if no dtg then autofill dtg

      if ( !catchup && !( screenContact.contactFlags & TO_BE_ENTERED ) )
      {
         int tne = screenContact.time.notEntered(); // partial dtg will give fe
         // full dtg gives -ve, none gives 0
         if ( tne == 0 )
         {
            screenContact.time = dtg( true );
            DateEdit->Text = screenContact.time.getDate( DTGDISP ).c_str();
            TimeEdit->Text = screenContact.time.getTime( DTGDISP ).c_str();
         }
      }

      logCurrentContact( );
      return true;

   }
   return false;
}
//---------------------------------------------------------------------------
bool TGJVEditFrame::doGJVForceButtonClick( TObject */*Sender*/ )
{
   if ( contest->isReadOnly() )
   {
      return true;
   }
   DateEdit->ReadOnly = !catchup;
   TimeEdit->ReadOnly = !catchup;
   SerTXEdit->ReadOnly = true;
   SerTXEdit->Color = clBtnFace;
   MinosLoggerEvents::SendShowErrorList();
   return dlgForced();
}
//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::GJVEditChange( TObject *Sender )
{
   doGJVEditChange( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::DateEditChange( TObject */*Sender*/ )
{
   if ( updateTimeAllowed || contest->isReadOnly() )
   {
      return ;
   }
   DateEdit->ReadOnly = false;
}
//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::TimeEditChange( TObject */*Sender*/ )
{
   if ( updateTimeAllowed || contest->isReadOnly() )
   {
      return ;
   }
   TimeEdit->ReadOnly = false;
}
//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::SerTXEditChange( TObject */*Sender*/ )
{
   if ( contest->isReadOnly() )
   {
      return ;
   }
   SerTXEdit->ReadOnly = false;
   SerTXEdit->Color = clWindow;
}
//---------------------------------------------------------------------------
void TGJVEditFrame::setScoreText( int dist, bool partial, bool xband )
{
   AnsiString b;
   AnsiString s;
   s.printf( "%d", dist );

   if ( partial )
      b += "(";
   b += s;
   if ( partial )
      b += ")";
   if ( xband )
      b += "X";
   DistSt->Caption = b;
}
//---------------------------------------------------------------------------
void TGJVEditFrame::calcLoc( )
{
   String gridref = LocEdit->Text.Trim();

   if ( stricmp( gridref.c_str(), oldloc.c_str() ) != 0 )
   {
      oldloc = gridref;
      locValid = true;
      double latitude;
      double longitude;
      double dist;
      int brg;

      int locValres = lonlat( gridref.c_str(), longitude, latitude );
      if ( ( locValres ) != LOC_OK )
         locValid = false;

      ScreenContact &sct = screenContact;
      if ( ( sct.contactFlags & MANUAL_SCORE ) &&
           !( sct.contactFlags & DONT_PRINT ) )
      {
         int thisscore = sct.contactScore;
         setScoreText( thisscore, false, sct.contactFlags & XBAND );
         BrgSt->Caption = "MANUAL";

      }
      else
         if ( ( locValid
                || ( locValres == LOC_PARTIAL ) )
              && contest->locValid )
         {
            contest->disbear( longitude, latitude, dist, brg );

            if ( locValid )  	// just 4CHAR not enough
            {
               sct.bearing = brg;

               if ( !( sct.contactFlags & ( MANUAL_SCORE | NON_SCORING | LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) ) )
               {
                  sct.contactScore = dist;
               }

            }

            TEMPBUFF( rev, 10 );
            strcpy( rev, ( MinosParameters::getMinosParameters() ->getMagneticVariation() ) ? "M" : "T" );
            int vb = varBrg( brg );
            if ( TContestApp::getContestApp() ->reverseBearing )
            {
               vb = normBrg( vb - 180 );
               strcat( rev, "R" );
            }
            setScoreText( ( int ) dist, ( locValres == LOC_PARTIAL ), sct.contactFlags & XBAND );
			AnsiString brgbuff;
            if ( locValres == LOC_PARTIAL )
            {
			   brgbuff.printf( "(%d\xB0%s)", vb, rev );
            }
            else
            {
               brgbuff.printf( "%d\xB0%s", vb, rev );
            }
            BrgSt->Caption = brgbuff;
         }
         else
         {
            DistSt->Caption = "";
            BrgSt->Caption = "";
            sct.contactScore = -1;
            sct.bearing = -1;
         }
   }
}
//---------------------------------------------------------------------------
void __fastcall TGJVEditFrame::LocEditChange( TObject *Sender )
{
   if ( current == LocEdit || Sender == LocEdit )
   {
      // force bearing calc
      getScreenEntry();
      calcLoc();
      doGJVEditChange( Sender );
   }
}
//---------------------------------------------------------------------------
bool TGJVEditFrame::validateControls( validTypes command )   // do control validation
{
   // spin round all controls, and validate them
   // return true if all valid

   for ( std::vector <ValidatedControl *>::iterator vcp = vcs.begin(); vcp != vcs.end(); vcp++ )
   {
      if ( updateTimeAllowed && ( ( *vcp ) == dateIl || ( *vcp ) == timeIl ) )
      {
         continue;
      }
      if ( !( *vcp ) ->valid( command ) )
         return false;
   }
   return true;
}
//---------------------------------------------------------------------------
bool TGJVEditFrame::valid( validTypes command )
{
   lgTraceerr(-1); // clear the error list

   if ( contest->isReadOnly() )
      return true;

   bool pvalid = validateControls( command ); // do control validation

   if ( command == cmCheckValid )   // our own command!
      contactValid();

   // re-validate, as we may have changed things
   pvalid = validateControls( cmValidStatus ); // look at current validity

   return pvalid;
}
//---------------------------------------------------------------------------
void TGJVEditFrame::selectField( TWinControl *v )
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
//==============================================================================
// check for embedded space or empty number

void TGJVEditFrame::fillRst( TLabeledEdit *rIl, std::string &rep, const std::string &fmode )
{
   std::string newrep;
   if ( current == rIl )
   {
      if ( !Validator::validateRST( rIl->Text.Trim() ) )
      {
         if ( ( rep.size() == 1 ) && isdigit( rep[ 0 ] ) )
         {
            newrep = rep[ 0 ];
            newrep += "9";
            rIl->Text = newrep.c_str();
            rep = newrep;
         }
      }
      if ( ( stricmp( fmode, "A1A" ) == 0 ) && Validator::validateRST( rIl->Text.Trim() )
           && ( rep.size() == 2 ) )
      {
         newrep = rep[ 0 ];
         newrep += rep[ 1 ];
         newrep += "9";
         rIl->Text = newrep.c_str();
         rep = newrep;
      }
   }
}
//==============================================================================
void TGJVEditFrame::doAutofill( void )
{
   if ( contest->isReadOnly() )
      return ;
   ScreenContact *vcct = &screenContact;
   if ( ( vcct->contactFlags & TO_BE_ENTERED ) ||  catchup)
   {
      // don't think we do anything here
   }
   else
      if ( ( current == CallsignEdit )
           && ( vcct->cs.validate( ) != ERR_NOCS ) )
      {
         // if no dtg and we are on cs, then autofill dtg
         int tne = vcct->time.notEntered(); // partial dtg will give fe
         // full dtg gives -ve, none gives 0
         if ( updateTimeAllowed || tne == 0 )
         {
            updateTimeAllowed = false;
            updateQSOTime();
            vcct->time = dtg( true );
            DateEdit->Text = vcct->time.getDate( DTGDISP ).c_str();
            TimeEdit->Text = vcct->time.getTime( DTGDISP ).c_str();
         }
      }

   //rst sent (autofill S9)

   fillRst( RSTTXEdit, vcct->reps, vcct->mode );
   fillRst( RSTRXEdit, vcct->repr, vcct->mode );
}
//==============================================================================
void TGJVEditFrame::lgTraceerr( int err )
{
   MinosLoggerEvents::SendValidateError(err);
}
//==============================================================================
void TGJVEditFrame::contactValid( void )
{
   // this is where we need to do all of our cross field validation

   getScreenEntry();
   ScreenContact *vcct = &screenContact;
   // date
   // time
   int tne = vcct->time.notEntered(); // partial dtg will give fe
   // full dtg gives -ve, none gives 0
   if ( tne == 1 )
      dateIl->tIfValid = false;
   if ( tne == 2 )
      timeIl->tIfValid = false;

   if ( vcct->contactFlags & DONT_PRINT )
   {
      lgTraceerr(-1);
      lgTraceerr( ERR_26 );
   }
   else
   {
      if ( vcct->contactFlags & ( LOCAL_COMMENT | COMMENT_ONLY ) )
         lgTraceerr( ERR_25 );
      if ( vcct->contactFlags & NON_SCORING )
         lgTraceerr( ERR_24 );
   }
   if ( vcct->contactFlags & ( LOCAL_COMMENT | COMMENT_ONLY | NON_SCORING | DONT_PRINT ) )
   {
      return ;
   }
   // cs worked

   vcct->cs.valRes = CS_NOT_VALIDATED;

   // we only validate this contact up to the validation point of the contest
   contest->validationPoint = selectedContact ;

   int csret = vcct->cs.validate( );
   contest->DupSheet.clearCurDup();
   if ( csret == CS_OK )
   {
      if ( contest->DupSheet.checkCurDup( vcct, contest->validationPoint, false ) )
      {
         if ( contest->DupSheet.isCurDup( vcct ) )      // But vcct is screen contact... so it won't be curdup
         {
            csret = vcct->cs.valRes = ERR_DUPCS;
         }
      }
   }
   if ( csret != CS_OK )
   {
      csIl->tIfValid = false;
      switch ( csret )
      {
         case CS_NOT_VALIDATED:
            lgTraceerr( ERR_10 );
            break;

         case ERR_NOCS:
            lgTraceerr( ERR_11 );
            break;

         case ERR_DUPCS:
            lgTraceerr( ERR_12 );
            break;

         default:
            lgTraceerr( ERR_13 );
            break;
      }
   }

   // locator received

   int locrep = vcct->loc.validate();
   if ( locrep != 0 )
   {
      if ( contest->locatorField.getValue() )
         locIl->tIfValid = false;
      if ( contest->locatorField.getValue() && ( locrep == ERR_NOLOC ) )
      {
         lgTraceerr( ERR_18 );
      }
      else
         if ( locrep == ERR_LOC_RANGE )
         {
            lgTraceerr( ERR_19 );
         }
   }

   // If multiplier ContestLog, multiplier (e.g. district if needed)
   // check anyway, this keeps country and loc charts up to date

   vcct->check( );  // check multiplier, don't log it yet!

   if ( contest->districtMult.getValue() && !vcct->QSOValid )
   {
      // no district when required
      // No CS means we should go to QTH, as its likely to be needed
      if ( csret == ERR_NOCS || ( vcct->ctryMult && vcct->ctryMult->hasDistricts() && !vcct->districtMult ) )
      {
         lgTraceerr( ERR_20 );    // "Invalid district multiplier"
         qthIl->tIfValid = false;
      }
      #warning we also need to check for district out of country
      // What do we allow as valid? Should we error wrong postcode/country combination?
   }
   else
      if ( contest->otherExchange .getValue() && !contest->districtMult.getValue() && ( trim( vcct->extraText ).size() == 0 ) )
      {
         // no QTH info if required

         lgTraceerr( ERR_21 );            // QTH required
         qthIl->tIfValid = false;
      }
}

//---------------------------------------------------------------------------
bool TGJVEditFrame::checkLogEntry(bool checkDTG)
{

   if ( contest->isReadOnly() )
      return true;

   // check if the screen contact and selected log contact differ
   bool retval = true;
   getScreenEntry();
   BaseContact *sct = selectedContact ;
   if ( sct->ne( screenContact, checkDTG ) )
   {
      int mresp = IDYES;

      // Dont check with op if not entered, and e.g. ESC pressed
      // Also allows for partial saving when in Uri mode
      if ( !( screenContact.contactFlags & TO_BE_ENTERED ) && !catchup )
      {
         mresp = MessageBox( Handle,
                             "This Contact has changed: Shall I log the changes?\n"
                             "\n"
                             "Yes         - Log as shown\n"
                             "No          - Discard changes",
                             "Minos Logger", MB_ICONQUESTION | MB_YESNO );
      }
      if ( mresp == IDYES )
      {
         //Yes - log and continue
         logScreenEntry( );
         retval = true;
      }
      else
         if ( mresp == IDNO )
         {
            //Cancel - Discard changes, continue action
            screenContact.copyFromArg( *selectedContact );  // we have to ACTUALLY revert, as the action may not conmplete
            showScreenEntry();
            retval = false;	// stay where we are
         }
   }
   return retval;
}

void TGJVEditFrame::setMode( String m )
{
   ModeComboBoxGJV->Text = m;
}

void TGJVEditFrame::clearCurrentField()
{
   current = 0;
}
void TGJVEditFrame::setTimeNow()
{
   updateTimeAllowed = false;
   updateQSOTime();
   dtg tnow( true );
   DateEdit->Text = tnow.getDate( DTGDISP ).c_str();
   TimeEdit->Text = tnow.getTime( DTGDISP ).c_str();
}
//---------------------------------------------------------------------------
void TGJVEditFrame::updateQSODisplay()
{
   if ( contest->districtMult.getValue() )
   {
      QTHEdit->CharCase = ecUpperCase;
   }
   else
   {
      QTHEdit->CharCase = ecNormal;
   }
}

void __fastcall TGJVEditFrame::TimeNowButtonClick(TObject *Sender)
{
   setTimeNow();
   EditControlExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TGJVEditFrame::ModeButtonClick(TObject *Sender)
{
   ModeComboBoxGJV->Text = ModeButton->Caption;
   EditControlExit(Sender);
}
//---------------------------------------------------------------------------



void __fastcall TGJVEditFrame::OpComboBoxKeyPress(TObject */*Sender*/,
      char &Key)
{
   Key = toupper( Key );
}
//---------------------------------------------------------------------------
void TGJVEditFrame::refreshOps()
{
   // refill the op combo boxes from the current contest, and select the correct op
//   BaseContestLog * contest = TContestApp::getContestApp() ->getCurrentContest();
   if (contest)
   {
      //if we are being closed then we are not the current contest - and current may be null
      String mainOp = MainOpComboBox->Text;
      String secondOp = SecondOpComboBox->Text;
      MainOpComboBox->Clear();
      SecondOpComboBox->Clear();
      for ( OperatorIterator i = contest->oplist.begin(); i != contest->oplist.end(); i++ )
      {
         if ( ( *i ).size() )
         {
            MainOpComboBox->Items->Add( ( *i ).c_str() );
            SecondOpComboBox->Items->Add( ( *i ).c_str() );
         }
      }
      MainOpComboBox->Text = mainOp;
      SecondOpComboBox->Text = secondOp;
   }
}

//---------------------------------------------------------------------------

