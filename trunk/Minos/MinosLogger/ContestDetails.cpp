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
#include "ContestDetails.h"
#include "gridhint.h"
#include "EntryOptions.h"
#include "SettingsEditor.h"
#include "MinosHelp.h"
#include "reg1test.h"
#include "VHFList.h"
#include "BandList.h"
#include "TCalendarForm.h"
#include "MDateSelectForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BundleFrame"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TContestEntryDetails::TContestEntryDetails( TComponent* Owner)
      : TForm( Owner ), /*CalendarDlg( 0 ),*/ contest(0), inputcontest(0),
      saveContestOK(false), suppressProtectedOnClick(false)
{
}
//---------------------------------------------------------------------------
__fastcall TContestEntryDetails::~TContestEntryDetails( )
{
//   delete CalendarDlg;
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::StartDateButtonClick(TObject */*Sender*/)
{
   std::auto_ptr <TDateSelectForm> DateSelectDlg( new TDateSelectForm( this ) );
   TDateTime test;
   try
   {
	test = TDateTime( StartDateEdit->Text, TDateTime::Date );
   }
   catch(...)
   {
	   test = TDateTime::CurrentDate();
   }
   DateSelectDlg->CalendarDate = test;
   if ( DateSelectDlg->ShowModal() == mrOk )
   {
	   StartDateEdit->Text = DateSelectDlg->CalendarDate.FormatString( "dd/mm/yyyy" );
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::EndDateButtonClick(TObject */*Sender*/)
{
   std::auto_ptr <TDateSelectForm> DateSelectDlg( new TDateSelectForm( this ) );
   TDateTime test;
   try
   {
	test = TDateTime( EndDateEdit->Text, TDateTime::Date );
   }
   catch(...)
   {
	   test = TDateTime::CurrentDate();
   }
   DateSelectDlg->CalendarDate = test;
   if ( DateSelectDlg->ShowModal() == mrOk )
   {
	   EndDateEdit->Text = DateSelectDlg->CalendarDate.FormatString( "dd/mm/yyyy" );
   }
}
//---------------------------------------------------------------------------
/*
      onShow - needs to produce a copy of the contest structure containing relevant
               fields
      setDetails (both) to populate display from those details

      getDetails to get from screen into copy

      Then OK Button to actually copy the detail over - then cancel
      can just discard the details
*/
void TContestEntryDetails::setDetails( LoggerContestLog * pcont )
{
   if ( !pcont )
      return ;
   inputcontest = pcont;
   contest = new LoggerContestLog();
   *contest = *pcont;                // is this safe? not with the QSO vector... although it won't get changed!
   sectionList = contest->sectionList.getValue().c_str(); // the combo will then be properly set up in setDetails()
   setDetails();
}
void TContestEntryDetails::setDetails(  )
{
   Caption = ( "Details of Contest Entry - " + contest->cfileName ).c_str();

   ContestNameEdit->Text = contest->name.getValue().c_str();                      // contest

   // need to get legal bands from ContestLog
   if ( contest->bands.size() )
   {
      BandComboBox->Style = Stdctrls::csDropDownList;
      TStringList *sl = new TStringList;

      sl->CommaText = contest->bands.c_str();
      BandComboBox->Items = sl;

      delete sl;
   }
   else
   {
      BandList &blist = BandList::getBandList();
      for (std::vector<BandInfo>::iterator i = blist.bandList.begin(); i != blist.bandList.end(); i++)
      {
         if ((*i).type != "HF")
         {
            BandComboBox->Items->Add( (*i).uk.c_str() );
         }
      }
   }
   int b = BandComboBox->Items->IndexOf( contest->band.getValue().c_str() );        // contest

   if ( b >= 0 )
   {
      BandComboBox->ItemIndex = b;
   }
   else
   {
      //BandComboBox->ItemIndex = 0;
	  BandComboBox->Style = Stdctrls::csDropDown;    // was csDropDownList
      BandComboBox->Text = contest->band.getValue().c_str();
   }

   if ( sectionList.Length() )
   {
      TStringList * sl = new TStringList;
      sl->CommaText = sectionList.c_str();
      SectionComboBox->Items = sl;
      delete sl;
   }

   int s = SectionComboBox->Items->IndexOf( contest->entSect.getValue().c_str() );        // contest

   if ( s >= 0 )
   {
      SectionComboBox->ItemIndex = s;
   }
   else
   {
      SectionComboBox->Style = Stdctrls::csDropDown;    // was csDropDownList
      SectionComboBox->Text = contest->entSect.getValue().c_str();
   }

   // start/end of ContestLog
   // if attempt to log QSO BEFORE, complain (but allow) (give set time offset option)
   // if AFTER at load time, set "post entry"
   // if attempt to log QSO AFTER and NOT post entry, complain (give set time offset option, post entry option)
   //      std::string contest->DTGStart;  //ccccmmsshhmm
   //      std::string contest->DTGEnd;    //ccccmmsshhmm

   if ( contest->DTGStart.getValue().size() )
   {
      StartDateEdit->Text = CanonicalToTDT( contest->DTGStart.getValue().c_str() ).FormatString("dd/mm/yyyy");
      StartTimeCombo->Text = CanonicalToTDT( contest->DTGStart.getValue().c_str() ).FormatString( "hh:nn" );
   }
   else
   {
      //         StartDateEdit->Date = "";
      StartTimeCombo->Text = "";
   }
   if ( contest->DTGEnd.getValue().size() )
   {
	  EndDateEdit->Text = CanonicalToTDT( contest->DTGEnd.getValue().c_str() ).FormatString("dd/mm/yyyy"); // short date format, hours:minutes
      EndTimeCombo->Text = CanonicalToTDT( contest->DTGEnd.getValue().c_str() ).FormatString( "hh:nn" ); // short date format, hours:minutes
   }
   else
   {
      //         EndDateEdit->Date = "";
      EndTimeCombo->Text = "";
   }

   if ( !contest->mycall.fullCall.getValue().size() )                                       // Entry
   {
      std::string temp;
      contest->entryBundle.getStringProfile( eepCall, temp );

      // STL version of strupr
      std::transform( temp.begin(), temp.end(),  	// source
                      temp.begin(),  				// destination
                      toupper );				// Function to use
      contest->mycall.fullCall.setValue( temp );
   }
   CallsignEdit->Text = contest->mycall.fullCall.getValue().c_str();
   if (contest->currentOp1.getValue().size()== 0)
   {
      contest->currentOp1.setValue( CallsignEdit->Text.c_str());
   }

   contest->validateLoc();
   if ( !contest->locValid && contest->myloc.loc.getValue().size() == 0 )
   {
      std::string temp;
      contest->QTHBundle.getStringProfile( eqpLocator, temp );
      contest->myloc.loc.setValue( temp );
      contest->validateLoc();
   }
   LocatorEdit->Text = contest->myloc.loc.getValue().c_str();

   AllowLoc4CB->Checked = contest->allowLoc4.getValue();    // bool               // ?? contest
   AllowLoc8CB->Checked = contest->allowLoc8.getValue();    // bool               // ?? contest

   ExchangeEdit->Text = contest->location.getValue().c_str(); // QTH/if contest specifies - but disp anyway

   ScoreOptions->ItemIndex = ( int ) contest->scoreMode.getValue();  // combo     // contest

   /*
      ExchangeComboBox:

      No Exchange Required
      PostCode Multipliers
      Other Exchange Multiplier
      Exchange Required (no multiplier)
   */

   if ( contest->districtMult.getValue() )
   {
      ExchangeComboBox->ItemIndex = 1;
   }
   else
      if ( contest->otherExchange.getValue() )
      {
         ExchangeComboBox->ItemIndex = 3;
      }
      else
      {
         ExchangeComboBox->ItemIndex = 0;
      }
   DXCCMult->Checked = contest->countryMult.getValue() ;
   LocatorMult->Checked = contest->locMult.getValue() ;
   GLocMult->Checked = contest->GLocMult.getValue();

   PowerEdit->Text = contest->power.getValue().c_str();

   if ( contest->isMinosFile() )
   {
      suppressProtectedOnClick = true;
      ProtectedOption->Checked = contest->isProtected() && !contest->isProtectedSuppressed();
      suppressProtectedOnClick = false;
   }
   else
   {
      ProtectedOption->Enabled = false;
   }
   RSTField->Checked = contest->RSTField.getValue() ;   // bool                   // contest
   SerialField->Checked = contest->serialField.getValue() ;   // bool             // contest
   LocatorField->Checked = contest->locatorField.getValue() ;   // bool         // contest
   QTHField->Checked = contest->QTHField.getValue() ;   // bool                   // contest

   AntOffsetEdit->Text = contest->bearingOffset.getValue();	// int
   refreshOps();

   enableControls();
}
void TContestEntryDetails::refreshOps()
{
   // refill the op combo boxes from the current contest, and select the correct op
   if (contest)
   {
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
      MainOpComboBox->Text = contest->currentOp1.getValue().c_str();
      SecondOpComboBox->Text = contest->currentOp2.getValue().c_str();
   }
}
void TContestEntryDetails::setDetails( const IndividualContest &ic )
{
   Caption = ( "Details of Contest Entry - " + contest->cfileName ).c_str();

   ContestNameEdit->Text = ic.description.c_str();                      // contest
   contest->VHFContestName.setValue(ic.description);

   // need to get legal bands from ContestLog
   BandComboBox->Items->Clear();
   BandComboBox->Style = Stdctrls::csDropDownList;

   BandComboBox->Items->Add( ic.reg1band.c_str() );
   BandComboBox->ItemIndex = 0;

   sectionList = ic.sections.c_str(); // the combo will then be properly set up in setDetails()
   if ( sectionList.Length() )
   {
      TStringList * sl = new TStringList;
      sl->CommaText = sectionList.c_str();
      SectionComboBox->Items = sl;
      delete sl;
   }

   int s = SectionComboBox->Items->IndexOf( contest->entSect.getValue().c_str() );        // contest

   if ( s >= 0 )
   {
      SectionComboBox->ItemIndex = s;
   }
   else
   {
      SectionComboBox->Style = Stdctrls::csDropDown;    // was csDropDownList
      SectionComboBox->Text = contest->entSect.getValue().c_str();
   }

   StartDateEdit->Text = ic.start.DateString();
   StartTimeCombo->Text = ic.start.FormatString( "hh:nn" );

   EndDateEdit->Text = ic.finish.DateString(); // short date format, hours:minutes
   EndTimeCombo->Text = ic.finish.FormatString( "hh:nn" ); // short date format, hours:minutes

   ScoreOptions->ItemIndex = ( int ) contest->scoreMode.getValue();  // combo     // contest

   if ( ic.mults == "M1" )
   {
      // PC, DXCC
      contest->districtMult.setValue( true );
      contest->countryMult.setValue( true );
      contest->locMult.setValue( false );
      contest->GLocMult.setValue( false );
   }
   else if ( ic.mults == "M2" )
   {
      // Loc
      contest->districtMult.setValue( false );
      contest->countryMult.setValue( false );
      contest->locMult.setValue( true );
      contest->GLocMult.setValue( false );
   }
   else if ( ic.mults == "M3" )
   {
      // PC, DXCC, LOC
      contest->districtMult.setValue( true );
      contest->countryMult.setValue( true );
      contest->locMult.setValue( true );
      contest->GLocMult.setValue( false );
   }
   else if ( ic.mults == "M4" )
   {
      // DXCC, LOC
      contest->districtMult.setValue( false );
      contest->countryMult.setValue( true );
      contest->locMult.setValue( true );
      contest->GLocMult.setValue( false );
   }
   else if ( ic.mults == "M5" )
   {
      // G Locs only
      contest->districtMult.setValue( false );
      contest->countryMult.setValue( false );
      contest->locMult.setValue( true );
      contest->GLocMult.setValue( true );
   }
   else
   {
      contest->districtMult.setValue( false );
      contest->countryMult.setValue( false );
      contest->locMult.setValue( false );
      contest->GLocMult.setValue( false );
   }
   /*
      ExchangeComboBox:

      No Exchange Required
      PostCode Multipliers
      Other Exchange Multiplier
      Exchange Required (no multiplier)
   */

   if ( contest->districtMult.getValue() )
   {
      ExchangeComboBox->ItemIndex = 1;
   }
   else
      if ( contest->otherExchange.getValue() )
      {
         ExchangeComboBox->ItemIndex = 3;
      }
      else
      {
         ExchangeComboBox->ItemIndex = 0;
      }
   DXCCMult->Checked = contest->countryMult.getValue() ;
   LocatorMult->Checked = contest->locMult.getValue() ;
   GLocMult->Checked = contest->GLocMult.getValue() ;

   RSTField->Checked = true ;
   SerialField->Checked = true ;
   LocatorField->Checked = true ;
   QTHField->Checked = true ;

   ScoreOptions->ItemIndex = ( ic.ppKmScoring ? 0 : 1 );
   contest->scoreMode.setValue( ( SCOREMODE ) ScoreOptions->ItemIndex );  // combo

//   setDetails();
}
//---------------------------------------------------------------------------
TWinControl * TContestEntryDetails::getDetails( )
{
   TWinControl *nextD = getNextFocus();

   contest->name.setValue( ContestNameEdit->Text.c_str() );
   contest->band.setValue( BandComboBox->Text.c_str() );
   contest->entSect.setValue( SectionComboBox->Text.c_str() );
   contest->sectionList.setValue( sectionList.c_str() );


   try
   {
      if (StartDateEdit->Text.IsEmpty())
      {
         StartDateEdit->Text = TDateTime::CurrentDate();
      }
   	TDateTime test = TDateTime( StartDateEdit->Text, TDateTime::Date );
      if ( ( int ) test > 0 )
         contest->DTGStart.setValue(
            TDTToCanonical( StartDateEdit->Text + " " + StartTimeCombo->Text ).c_str() );
      else
         contest->DTGStart.setValue( "" );
   }
   catch ( EConvertError & )
   {
      MinosParameters::getMinosParameters() ->mshowMessage
      (
         StartDateEdit->Text + " " + StartTimeCombo->Text +
                     " is not a good date/time (DD/MM/CCYY HH:NN)"
         , this
      );
      nextD = (nextD?nextD:StartDateEdit);
   }

   try
   {
      if (EndDateEdit->Text.IsEmpty())
      {
         EndDateEdit->Text = TDateTime::CurrentDate();
      }
   	TDateTime test = TDateTime( EndDateEdit->Text, TDateTime::Date );
      if ( ( int ) test > 0 )
         contest->DTGEnd.setValue( TDTToCanonical( EndDateEdit->Text + " " + EndTimeCombo->Text ).c_str() );
      else
         contest->DTGEnd.setValue( "" );
   }
   catch ( EConvertError & )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( EndDateEdit->Text + " " + EndTimeCombo->Text + " is not a good date/time (DD/HH/CCYY HH:NN)", this );
      nextD = (nextD?nextD:EndDateEdit);
   }

   contest->mycall.fullCall.setValue( CallsignEdit->Text.c_str() );
   contest->mycall.valRes = CS_NOT_VALIDATED;
   contest->mycall.validate();
   if ( contest->mycall.valRes != CS_OK )
   {
      nextD = (nextD?nextD:CallsignEdit);
   }
   contest->myloc.loc.setValue( LocatorEdit->Text.c_str() );
   contest->myloc.validate();
   if ( contest->myloc.valRes != LOC_OK )
   {
      nextD = (nextD?nextD:LocatorEdit);
   }
   if ( contest->currentOp1.getValue().size() == 0 )
   {
      nextD = (nextD?nextD:MainOpComboBox);
   }
   contest->allowLoc4.setValue( AllowLoc4CB->Checked );    // bool
   contest->allowLoc8.setValue( AllowLoc8CB->Checked );    // bool
   contest->location.setValue( ExchangeEdit->Text.c_str() );
   contest->scoreMode.setValue( ( SCOREMODE ) ScoreOptions->ItemIndex );  // combo
   contest->countryMult.setValue( DXCCMult->Checked );   // bool

   contest->locMult.setValue( LocatorMult->Checked ) ;   // bool
   contest->GLocMult.setValue( GLocMult->Checked ) ;   // bool

   if (ProtectedOption->Checked && contest->isProtected() && contest->isProtectedSuppressed())
   {
      contest->setProtectedSuppressed(false);
   }
   else
   {
      if (ProtectedOption->Checked && !contest->isProtected())
      {
         contest->setProtected( true ) ;
         saveContestOK  = true;
      }
      else if (!ProtectedOption->Checked && contest->isProtected())
	  {
         contest->setProtected( false ) ;
         saveContestOK  = true;
      }
      else if (!contest->isReadOnly()) // not protected, not unwriteable, protected but suppressed
      {
         saveContestOK  = true;
      }
   }
   /*
      ExchangeComboBox:

      No Exchange Required
      PostCode Multipliers
      Other Exchange Multiplier
      Exchange Required (no multiplier)
   */
   switch ( ExchangeComboBox->ItemIndex )
   {
      case 0:
         contest->otherExchange.setValue( false );
         contest->districtMult.setValue( false );
         break;

      case 1:
         contest->otherExchange.setValue( true );
         contest->districtMult.setValue( true );
         break;

      case 2:
         contest->otherExchange.setValue( true );
         contest->districtMult.setValue( false );
         break;

      case 3:
         contest->otherExchange.setValue( true );
         contest->districtMult.setValue( false );
         break;

   }
   contest->RSTField.setValue( RSTField->Checked ) ;   // bool
   contest->serialField.setValue( SerialField->Checked ) ;   // bool
   contest->locatorField.setValue( LocatorField->Checked ) ;   // bool
   contest->QTHField.setValue( QTHField->Checked ) ;   // bool

   contest->power.setValue( PowerEdit->Text.c_str() );
   contest->bearingOffset.setValue(atoi(AntOffsetEdit->Text .c_str()));	// int

   contest->currentOp1.setValue(MainOpComboBox->Text.c_str());
   contest->currentOp2.setValue(SecondOpComboBox->Text.c_str());
   contest->oplist.insert(contest->currentOp1.getValue());
   contest->oplist.insert(contest->currentOp2.getValue());

   contest->validateLoc();

   return nextD;
}
//---------------------------------------------------------------------------
TWinControl * TContestEntryDetails::getNextFocus()
{
   if ( ContestNameEdit->Text.Trim().Length() == 0 )
   {
      return ContestNameEdit;
   }
   if ( BandComboBox->Text.Trim().Length() == 0 )
   {
      return BandComboBox;
   }
   if ( CallsignEdit->Text.Trim().Length() == 0 )
   {
      return CallsignEdit;
   }
   if ( LocatorEdit->Text.Trim().Length() == 0 )
   {
      return LocatorEdit;
   }
   if ( PowerEdit->Text.Trim().Length() == 0 )
   {
      return PowerEdit;
   }
   return 0;
}
void __fastcall TContestEntryDetails::OKButtonClick( TObject * /*Sender*/ )
{
   // make sure we have the minimum required information

   if (ProtectedOption->Checked && ! contest->isProtected())
   {
      if (!mShowYesNoMessage(this, "This contest will be marked as protected.\r\n"
                                    "This is a permanent change that may be temporarily overridden.\r\n"
                                    "Please confirm this change by pressing \"Ok\"." ))
      {
         return;
      }
   }

   TWinControl *nextD = getDetails( );
   if ( nextD )
   {
      nextD->SetFocus();
   }
   else
   {
      if (saveContestOK)
      {
         bool temp = contest->isProtectedSuppressed();
         contest->setProtectedSuppressed(true);
         contest->commonSave( false );
         contest->setProtectedSuppressed(temp);
      }
      *inputcontest = *contest;
      ModalResult = mrOk;
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::CancelButtonClick( TObject * /*Sender*/ )
{
   //
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::FormShow( TObject * /*Sender*/ )
{
   MinosParameters::getMinosParameters() ->applyFontChange(this);

   QTHBundleFrame->initialise( "QTH", &contest->QTHBundle, &contest->QTHBundleName );
   StationBundleFrame->initialise( "Station", &contest->stationBundle, &contest->stationBundleName );
   EntryBundleFrame->initialise( "Entry", &contest->entryBundle, &contest->entryBundleName );
   ContestNameSelected->Text = contest->VHFContestName.getValue().c_str();

   contest->initialiseINI();

   for ( int i = 0; i < 24; i++ )
   {
      std::string cbText = ( boost::format( "%02.2d:" ) % i ).str();
      std::string hour = cbText + "00";
      std::string halfhour = cbText + "30";
      StartTimeCombo->Items->Add( hour.c_str() );
      StartTimeCombo->Items->Add( halfhour.c_str() );
      EndTimeCombo->Items->Add( hour.c_str() );
      EndTimeCombo->Items->Add( halfhour.c_str() );
   }
   TWinControl *nextD = getDetails( );
   if ( nextD )
   {
      nextD->SetFocus();
   }
}
//---------------------------------------------------------------------------
void __fastcall TContestEntryDetails::EntDetailButtonClick( TObject * /*Sender*/ )
{
   getDetails( );   // override from the window
   std::auto_ptr <TEntryOptionsForm> EntryDlg( new TEntryOptionsForm( this, contest, false ) );
   EntryDlg->EntryGroup->Visible = false;
   EntryDlg->Caption = "Entry Details";
   if ( EntryDlg->ShowModal() == mrOk )
      setDetails( );

}
//---------------------------------------------------------------------------
String BSHelpText =
   "These settings are groups of settings that can "
   "be applied to a contest all in one go."
   "\r\n\r\n"
   "There are four basic groups: - \r\n\r\n"
   "Contest - for the description, bands, multipliers and time of a contest\r\n"
   "Entry - all the extra bits for a real entry - callsign, group, contact details.\r\n"
   "Station - Rig details, antenna, antenna height.\r\n"
   "QTH - where the station is, height above sea level, Locator.\r\n"
   "\r\n"
   "To use them select from the drop down lists, or for Contest, use the "
   "\"VHF Calendar\" button.\r\n"
   "Any group set to \"<none>\" will be ignored.\r\n"
   "\r\n"
   "If the setting you want isn't there, press the \"Edit\" "
   "button for the group.\r\n"
   "\r\n"
   "This brings up a dialog where you can define a new setting, "
   "copy an existing setting, or delete an existing setting\r\n"
   "\r\n"
   "Click on the setting name on the left to select an existing setting "
   "and then its components are shown in the right hand pane, and "
   "can be edited individually.\r\n"
   "\r\n"
   "Move between components of a group using the mouse or up/down arrow keys.\r\n"
   ;

void __fastcall TContestEntryDetails::BSHelpButtonClick( TObject */*Sender*/ )
{
   // Put up the help text on bundled settings
   std::auto_ptr <TMinosHelpForm> HelpForm( new TMinosHelpForm( this ) );
   HelpForm->Caption = "Settings";
   HelpForm->HelpMemo->Text = BSHelpText;
   HelpForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::DateEditKeyPress( TObject * /*Sender*/,
      char &Key )
{
   if ( Key == VK_ESCAPE || Key == VK_RETURN )
   {
      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::VHFCalendarButtonClick( TObject * /*Sender*/ )
{
   std::auto_ptr <TCalendarForm> CalendarDlg(new TCalendarForm(this));

   CalendarDlg->Caption = "VHF Calendar";
   CalendarDlg->description = ContestNameSelected->Text;

   if ( CalendarDlg->ShowModal() == mrOk )
   {
      // set up all the details that we can from the calendar
      ContestNameSelected->Text = CalendarDlg->ic.description.c_str();
      setDetails( CalendarDlg->ic );
   }
   TWinControl *next = getNextFocus();
   if (next)
   {
      next->SetFocus();
   }
   else
   {
      OKButton->SetFocus();
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::BundleFrameBundleSectionChange(
      TObject *Sender)
{
   TComboBox *cb = dynamic_cast<TComboBox *>(Sender);
   TSettingBundleFrame *bf = dynamic_cast<TSettingBundleFrame *>(cb->Parent);
   if (!bf && cb->Parent && cb->Parent->Parent)
   {
      bf = dynamic_cast<TSettingBundleFrame *>(cb->Parent->Parent);
   }
   if (bf)
   {
      bf->BundleSectionChange(Sender);
   }
   getDetails( );   // override from the window

   contest->setINIDetails();
   setDetails( );
   TWinControl *next = getNextFocus();
   if (next)
   {
      next->SetFocus();
   }
   else
   {
      OKButton->SetFocus();
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::BundleFrameBundleEditClick(
      TObject *Sender)
{
   TButton *cb = dynamic_cast<TButton *>(Sender );
   TSettingBundleFrame *bf = dynamic_cast<TSettingBundleFrame *>(cb->Parent);
   if (!bf && cb->Parent && cb->Parent->Parent)
   {
      bf = dynamic_cast<TSettingBundleFrame *>(cb->Parent->Parent);
   }
   if (bf)
   {
      bf->BundleEditClick(Sender);
   }
   getDetails( );   // override from the window

   contest->setINIDetails();
   setDetails( );
   TWinControl *next = getNextFocus();
   if (next)
   {
      next->SetFocus();
   }
   else
   {
      OKButton->SetFocus();
   }
}
//---------------------------------------------------------------------------


void __fastcall TContestEntryDetails::CallsignEditExit(TObject */*Sender*/)
{
   if (MainOpComboBox->Text.IsEmpty())
   {
      MainOpComboBox->Text = CallsignEdit->Text;
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::ProtectedOptionClick(TObject */*Sender*/)
{
   if (!suppressProtectedOnClick)
   {
      if (ProtectedOption->Checked )
      {
         // move to protected
         if (!mShowYesNoMessage(this, "Are you sure you want to protect this contest?" ))
         {
            ProtectedOption->Checked = contest->isProtected() && !contest->isProtectedSuppressed();
         }
      }
      else // unchecked
      {
         if (!mShowYesNoMessage(this, "Are you sure you want to disable protection for this contest?" ))
         {
            ProtectedOption->Checked = contest->isProtected() && !contest->isProtectedSuppressed();
         }
      }
      enableControls();
   }
}
//---------------------------------------------------------------------------
void TContestEntryDetails::enableControls()
{
// Should protected be disabled if the contest is unwriteable?
   bool protectedChecked = ProtectedOption->Checked;
// enable/disable relevant fields based on protected
   ContestNameEdit->Enabled = !protectedChecked;
   BandComboBox->Enabled = !protectedChecked;
   CallsignEdit->Enabled = !protectedChecked;
   LocatorEdit->Enabled = !protectedChecked;
   ExchangeEdit->Enabled = !protectedChecked;
   ScoreOptions->Enabled = !protectedChecked;
   MultGroupBox->Enabled = !protectedChecked;
   DXCCMult->Enabled = !protectedChecked;
   LocatorMult->Enabled = !protectedChecked;
   FieldsGroupBox->Enabled = !protectedChecked;
   RSTField->Enabled = !protectedChecked;
   SerialField->Enabled = !protectedChecked;
   LocatorField->Enabled = !protectedChecked;
   QTHField->Enabled = !protectedChecked;

   QTHBundleFrame->enableBundle(!protectedChecked);
   StationBundleFrame->enableBundle(!protectedChecked);
   EntryBundleFrame->enableBundle(!protectedChecked);

   SectionComboBox->Enabled = !protectedChecked;
   StartTimeCombo->Enabled = !protectedChecked;
   EndTimeCombo->Enabled = !protectedChecked;
   ExchangeComboBox->Enabled = !protectedChecked;
   VHFCalendarButton->Enabled = !protectedChecked;
   ContestNameSelected->Enabled = !protectedChecked;
   LocatorGroupBox->Enabled = !protectedChecked;
   AllowLoc8CB->Enabled = !protectedChecked;
   AllowLoc4CB->Enabled = !protectedChecked;
   StartDateEdit->Enabled = !protectedChecked;
   EndDateEdit->Enabled = !protectedChecked;
   StartDateButton->Enabled = !protectedChecked;
   EndDateButton->Enabled = !protectedChecked;
   MainOpComboBox->Enabled = !protectedChecked;
   SecondOpComboBox->Enabled = !protectedChecked;
   AntOffsetEdit->Enabled = !protectedChecked;
}

