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
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Hstcbo"
#pragma link "BundleFrame"
#pragma link "JvDateTimePicker"
#pragma link "JvExComCtrls"
#pragma link "JvCheckedMaskEdit"
#pragma link "JvDatePickerEdit"
#pragma link "JvExMask"
#pragma link "JvMaskEdit"
#pragma link "JvToolEdit"
#pragma resource "*.dfm"

#define nSections 13
std::string DefaultSectionList[ nSections ] =
   {
      std::string( "SF" ),
      std::string( "SO" ),
      std::string( "M" ),
      std::string( "6S" ),
      std::string( "6O" ),
      std::string( "O" ),
      std::string( "OPEN" ),
      std::string( "RESTRICTED" ),
      std::string( "LOW POWER" ),
      std::string( "3W SINGLE" ),
      std::string( "10W SINGLE" ),
      std::string( "3W MULTI" ),
      std::string( "10W MULTI" )
   };

//---------------------------------------------------------------------------
__fastcall TContestEntryDetails::TContestEntryDetails( TComponent* Owner)
      : TForm( Owner ), CalendarDlg( 0 ), contest(0), inputcontest(0)
{
}
//---------------------------------------------------------------------------
__fastcall TContestEntryDetails::~TContestEntryDetails( )
{
   delete CalendarDlg;
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

   if ( contest->sections.size() )
   {
      TStringList * sl = new TStringList;
      // need to get legal sections from ContestLog
      sl->CommaText = contest->sections.c_str();
      SectionComboBox->Items = sl;
      delete sl;
   }
   else
   {
      for ( int i = 0; i < nSections; i++ )
      {
         SectionComboBox->Items->Add( DefaultSectionList[ i ].c_str() );
      }
   }

   int s = SectionComboBox->Items->IndexOf( contest->entSect.getValue().c_str() );        // contest

   if ( s >= 0 )
   {
      SectionComboBox->ItemIndex = s;
   }
   else
   {
      //SectionComboBox->ItemIndex = 0;
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
      StartDateEdit->Date = CanonicalToTDT( contest->DTGStart.getValue().c_str() ).DateString();
      StartTimeCombo->Text = CanonicalToTDT( contest->DTGStart.getValue().c_str() ).FormatString( "hh:mm" );
   }
   else
   {
      //         StartDateEdit->Date = "";
      StartTimeCombo->Text = "";
   }
   if ( contest->DTGEnd.getValue().size() )
   {
      EndDateEdit->Date = CanonicalToTDT( contest->DTGEnd.getValue().c_str() ).DateString(); // short date format, hours:minutes
      EndTimeCombo->Text = CanonicalToTDT( contest->DTGEnd.getValue().c_str() ).FormatString( "hh:mm" ); // short date format, hours:minutes
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
   DXCCMult->Checked = contest->countryMult.getValue() ;   // bool               // contest
   LocatorMult->Checked = contest->locMult.getValue() ;   // bool                // contest

   PowerEdit->Text = contest->power.getValue().c_str();

   if ( contest->isGJVFile() )
   {
      PostEventOption->Checked = false ;   // bool       // Free!
      ReadOnlyOption->Checked = true ;   // bool         // Free!

      PostEventOption->Enabled = false;
      ReadOnlyOption->Enabled = false;
      OptionsGroupBox->Enabled = false;
   }
   else
   {
      PostEventOption->Checked = contest->isPostEntry() ;   // bool       // Free!
      ReadOnlyOption->Checked = contest->isReadOnly() ;   // bool         // Free!
   }
   RSTField->Checked = contest->RSTField.getValue() ;   // bool                   // contest
   SerialField->Checked = contest->serialField.getValue() ;   // bool             // contest
   LocatorField->Checked = contest->locatorField.getValue() ;   // bool         // contest
   QTHField->Checked = contest->QTHField.getValue() ;   // bool                   // contest

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

   TStringList *sl = new TStringList;
   // need to get legal sections from ContestLog
   sl->CommaText = ic.sections.c_str();
   SectionComboBox->Items = sl;
   SectionComboBox->ItemIndex = 0;


   // start/end of ContestLog
   // if attempt to log QSO BEFORE, complain (but allow) (give set time offset option)
   // if AFTER at load time, set "post entry"
   // if attempt to log QSO AFTER and NOT post entry, complain (give set time offset option, post entry option)
   //      std::string contest->DTGStart;  //ccccmmsshhmm
   //      std::string contest->DTGEnd;    //ccccmmsshhmm

   StartDateEdit->Date = ic.start.DateString();
   StartTimeCombo->Text = ic.start.FormatString( "hh:mm" );

   EndDateEdit->Date = ic.finish.DateString(); // short date format, hours:minutes
   EndTimeCombo->Text = ic.finish.FormatString( "hh:mm" ); // short date format, hours:minutes

   ScoreOptions->ItemIndex = ( int ) contest->scoreMode.getValue();  // combo     // contest

   if ( ic.mults == "M1" )
   {
      // PC, DXCC
      contest->districtMult.setValue( true );
      contest->countryMult.setValue( true );
      contest->locMult.setValue( false );
   }
   if ( ic.mults == "M2" )
   {
      // Loc
      contest->districtMult.setValue( false );
      contest->countryMult.setValue( false );
      contest->locMult.setValue( true );
   }
   if ( ic.mults == "M3" )
   {
      // PC, DXCC, LOC
      contest->districtMult.setValue( true );
      contest->countryMult.setValue( true );
      contest->locMult.setValue( true );
   }
   if ( ic.mults == "M4" )
   {
      // DXCC, LOC
      contest->districtMult.setValue( false );
      contest->countryMult.setValue( true );
      contest->locMult.setValue( true );

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

   RSTField->Checked = true ;
   SerialField->Checked = true ;
   LocatorField->Checked = true ;
   QTHField->Checked = true ;

   ScoreOptions->ItemIndex = ( ic.ppKmScoring ? 0 : 1 );
   contest->scoreMode.setValue( ( SCOREMODE ) ScoreOptions->ItemIndex );  // combo

}
//---------------------------------------------------------------------------
bool TContestEntryDetails::getDetails( )
{
	contest->name.setValue( ContestNameEdit->Text.t_str() );
	contest->band.setValue( BandComboBox->Text.t_str() );
	contest->entSect.setValue( SectionComboBox->Text.t_str() );


   try
   {
      if ( ( int ) StartDateEdit->Date > 0 )
         contest->DTGStart.setValue(
				TDTToCanonical( StartDateEdit->Date.DateString() + " " + StartTimeCombo->Text ).t_str() );
      else
         contest->DTGStart.setValue( "" );
   }
   catch ( EConvertError & )
   {
      MinosParameters::getMinosParameters() ->mshowMessage
      (
         StartDateEdit->Date.DateString() + " " + StartTimeCombo->Text +
         " is not a good date/time (DD/HH/CCYY HH:MM)"
         , this
      );
      StartDateEdit->SetFocus();
      return false;
   }

   try
   {
      if ( ( int ) EndDateEdit->Date > 0 )
			contest->DTGEnd.setValue( TDTToCanonical( EndDateEdit->Date.DateString() + " " + EndTimeCombo->Text ).t_str() );
      else
         contest->DTGEnd.setValue( "" );
   }
   catch ( EConvertError & )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( EndDateEdit->Date.DateString() + " " + EndTimeCombo->Text + " is not a good date/time (DD/HH/CCYY HH:MM)", this );
      EndDateEdit->SetFocus();
      return false;
   }


	contest->mycall.fullCall.setValue( CallsignEdit->Text.t_str() );
	contest->myloc.loc.setValue( LocatorEdit->Text.t_str() );
   contest->myloc.validate();
   if ( contest->myloc.valRes != LOC_OK )
   {
      LocatorEdit->SetFocus();
      return false;
   }
   contest->allowLoc4.setValue( AllowLoc4CB->Checked );    // bool
   contest->allowLoc8.setValue( AllowLoc8CB->Checked );    // bool
	contest->location.setValue( ExchangeEdit->Text.t_str() );
   contest->scoreMode.setValue( ( SCOREMODE ) ScoreOptions->ItemIndex );  // combo
   contest->countryMult.setValue( DXCCMult->Checked );   // bool

   contest->locMult.setValue( LocatorMult->Checked ) ;   // bool
   contest->setPostEntry( PostEventOption->Checked ) ;   // bool
   contest->setReadOnly( ReadOnlyOption->Checked ) ;   // bool  - NB this doesn't do very much useful - doesn't convert it to RO

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

   contest->power.setValue( PowerEdit->Text.t_str() );

   contest->validateLoc();

   return true;
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
   if ( getDetails( ) )
   {
      TWinControl * next = getNextFocus();
      if ( next )
      {
         next->SetFocus();
         return ;
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
   //   ScaleBy( TContestApp::getContestApp() ->sysfont->Size, Font->Size );
   //   Font->Assign( TContestApp::getContestApp() ->sysfont );

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
   if ( getDetails( ) )
   {
      TWinControl *next = getNextFocus();
      if ( next )
      {
         next->SetFocus();
      }
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
void __fastcall TContestEntryDetails::BundleOverrideButtonClick( TObject * /*Sender*/ )
{
   getDetails( );   // override from the window

   contest->setINIDetails();
   setDetails( );
   if ( bool( CalendarDlg ) )
   {
      setDetails( CalendarDlg->ic );
   }
}
//---------------------------------------------------------------------------
String BSHelpText =
   "A \"Settings Bundle\" is a group (or bundle) of settings that can "
   "be applied to a contest all in one go. Each of the individual settings "
   "in a bundle is a \"bundled setting\"\r\n"
   "\r\n"
   "There are four basic bundles - \r\n"
   "Contest - for the description, bands, multipliers and time of a contest\r\n"
   "Station - Rig details, antenna.\r\n"
   "QTH - where the station is, Locator.\r\n"
   "Entry - all the extra bits for a real entry - callsign, group, contact details.\r\n"
   "\r\n"
   "To use them select from the drop down lists, or for Contest, use the "
   "\"VHFCalendar\" button.\r\n"
   "Any bundles set to \"<none>\" will be ignored.\r\n"
   "\r\n"
   "If the setting you want isn't there, press the \"Edit\" "
   "button for the bundle.\r\n"
   "\r\n"
   "This brings up a dialog where you can define a new setting, "
   "copy an existing setting, or delete an existing setting\r\n"
   "\r\n"
   "Click on the setting name on the left to select an existing setting "
   "and then its comp[onents are shown in the right hand pane, and "
   "can be editted individually.\r\n"
   "\r\n"
   "Move between components using the mouse or up/down arrow keys.\r\n"
   ;

void __fastcall TContestEntryDetails::BSHelpButtonClick( TObject */*Sender*/ )
{
   // Put up the help text on bundled settings
   std::auto_ptr <TMinosHelpForm> HelpForm( new TMinosHelpForm( this ) );
   HelpForm->Caption = "Bundled Settings";
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
   if ( !CalendarDlg )
   {
      CalendarDlg = new TCalendarForm( this ) ;
      CalendarDlg->Caption = "VHF Calendar";
      CalendarDlg->description = ContestNameSelected->Text;
   }
   if ( CalendarDlg->ShowModal() == mrOk )
   {
      // set up all the details that we can from the calendar
      ContestNameSelected->Text = CalendarDlg->ic.description.c_str();
      setDetails( CalendarDlg->ic );
   }
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::BundleFrameBundleSectionChange(
      TObject *Sender)
{
   TComboBox *cb = dynamic_cast<TComboBox *>(Sender);
   TSettingBundleFrame *bf = dynamic_cast<TSettingBundleFrame *>(cb->Parent);
   if (bf)
   {
      bf->BundleSectionChange(Sender);
   }
   getDetails( );   // override from the window

   contest->setINIDetails();
   setDetails( );
}
//---------------------------------------------------------------------------

void __fastcall TContestEntryDetails::StationBundleFrameBundleEditClick(
      TObject *Sender)
{
   TButton *cb = dynamic_cast<TButton *>(Sender );
   TSettingBundleFrame *bf = dynamic_cast<TSettingBundleFrame *>(cb->Parent);
   if (bf)
   {
      bf->BundleEditClick(Sender);
   }
   getDetails( );   // override from the window

   contest->setINIDetails();
   setDetails( );
}
//---------------------------------------------------------------------------

