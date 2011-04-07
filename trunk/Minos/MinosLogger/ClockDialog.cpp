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

#include "ClockDialog.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TClockDlg *ClockDlg;
//---------------------------------------------------------------------------
__fastcall TClockDlg::TClockDlg( TComponent* Owner )
      : TForm( Owner ), initialCorrection( bigClockCorr ), initialised( false )
{}

//---------------------------------------------------------------------------
void __fastcall TClockDlg::FormShow( TObject */*Sender*/ )
{
   MinosParameters::getMinosParameters() ->applyFontMultiplier(this);
   //   ScaleBy( TContestApp::getContestApp() ->sysfont->Size, Font->Size );
   //   Font->Assign( TContestApp::getContestApp() ->sysfont );

   setEdits();
}
//---------------------------------------------------------------------------
void TClockDlg::setEdits()
{
   initialised = false;
   TDateTime corr = dtg::getCorrectedUTC( );
   DayEdit->Text = corr.FormatString( "dd" );
   MonthEdit->Text = corr.FormatString( "mm" );
   YearEdit->Text = corr.FormatString( "yyyy" );

   TDateTime dtt = TDateTime( bigClockCorr );
   HoursEdit->Text = dtt.FormatString( "h" );
   MinutesEdit->Text = dtt.FormatString( "n" );
   SecondsEdit->Text = dtt.FormatString( "s" );
   initialised = true;
}
//---------------------------------------------------------------------------
void __fastcall TClockDlg::OKButtonClick( TObject */*Sender*/ )
{
   // and save the correction to the correction file
   std::ofstream strm( ".\\Configuration\\time.correction", std::ios::trunc | std::ios::binary );
   if ( strm )
   {
      // date_correction is already in seconds

      strm << long( bigClockCorr * dtg::daySecs ) << std::endl;
   }
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TClockDlg::ClearButtonClick( TObject *Sender )
{
   bigClockCorr = 0;
   setEdits();
   Timer1Timer( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TClockDlg::ApplyButtonClick( TObject *Sender )
{
   setEdits();
   Timer1Timer( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TClockDlg::Timer1Timer( TObject */*Sender*/ )
{
   String rawdisp = dtg::getRawUTC( ).FormatString( "dd/mm/yyyy hh:nn:ss" ) + " UTC";
   String disp = dtg::getCorrectedUTC( ).FormatString( "dd/mm/yyyy hh:nn:ss" ) + " UTC";

   RawTime->Caption = rawdisp;
   CorrectedTime->Caption = disp;
}
//---------------------------------------------------------------------------
void __fastcall TClockDlg::ClockEditChange( TObject *Sender )
{
   if ( initialised )
   {
      TEMPBUFF( instr, 20 );

      time_t cttime = time( 0 );			// computer time
      struct tm *tms = localtime( &cttime );  // NB static data area!

      struct tm tmstr;
      tmstr = *tms;	// overwrite it

      tmstr.tm_mday = DayEdit->Text.ToIntDef( 0 );
      tmstr.tm_mon = MonthEdit->Text.ToIntDef( 0 ) - 1;
      tmstr.tm_year = YearEdit->Text.ToIntDef( 1900 ) - 1900;

      time_t cttime2 = mktime( &tmstr );	// time value for the day

      // now we need to make that into a correction value

      long date_correction = cttime2 - cttime;	// I hope sign is right!

      long clock_correction = HoursEdit->Text.ToIntDef( 0 ) * 3600;
      clock_correction += MinutesEdit->Text.ToIntDef( 0 ) * 60;
      clock_correction += SecondsEdit->Text.ToIntDef( 0 );

      bigClockCorr = ( clock_correction + date_correction ) / dtg::daySecs;

      Timer1Timer( Sender );
   }
}
//---------------------------------------------------------------------------
void __fastcall TClockDlg::CancelButtonClick( TObject */*Sender*/ )
{
   bigClockCorr = initialCorrection;
   Close();
}
//---------------------------------------------------------------------------
