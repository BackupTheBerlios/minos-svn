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

#include "VHFList.h"
#include "TCalendarForm.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvExMask"
#pragma link "JvSpin"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TCalendarForm::TCalendarForm( TComponent* Owner )
      : TForm( Owner ), vhf(2000)
{
   TDateTime tnow = TDateTime::CurrentDateTime();
   YearSpinEdit->Text = tnow.FormatString("yyyy");
}
//---------------------------------------------------------------------------
void __fastcall TCalendarForm::Button1Click( TObject * /*Sender*/ )
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TCalendarForm::FormShow( TObject * /*Sender*/ )
{
   vhf = Calendar(YearSpinEdit->Text.ToIntDef(2000));
   bool loaded = vhf.parseFile( ".\\configuration\\vhfContests.xml" );
   if ( !loaded )
   {
      ShowMessage( "Failed to load the calendar file" );
      CalendarVersionLabel->Caption = "No file loaded";
      return ; // don't close - they need a chance to download
   }
   CalendarVersionLabel->Caption = ("File Version " + vhf.version).c_str();
   CalendarGrid->RowCount = vhf.calendar.size() + 1;
   CalendarGrid->ColCount = 8;
   CalendarGrid->ColWidths[ 0 ] = CalendarGrid->Canvas->TextWidth( "144MHz UK Activity Contest and Club ChampionshipXXX" );
   CalendarGrid->ColWidths[ 1 ] = CalendarGrid->Canvas->TextWidth( "248GHZZZ" );
   CalendarGrid->ColWidths[ 2 ] = CalendarGrid->Canvas->TextWidth( "99/99/9999 99:99XXX" );
   CalendarGrid->ColWidths[ 3 ] = CalendarGrid->Canvas->TextWidth( "99/99/9999 99:99XXX" );
   CalendarGrid->ColWidths[ 4 ] = CalendarGrid->Canvas->TextWidth( "1PT/QSOXX" );
   CalendarGrid->ColWidths[ 5 ] = CalendarGrid->Canvas->TextWidth( "MULTSXX" );
   CalendarGrid->ColWidths[ 6 ] = CalendarGrid->Canvas->TextWidth( "3S, 3M, 10S, 10M, XX" );
   CalendarGrid->ColWidths[ 7 ] = CalendarGrid->Canvas->TextWidth( "LOW POWER CONTEST" );

   CalendarGrid->Cells[ 0 ][ 0 ] = "Contest Name";
   CalendarGrid->Cells[ 1 ][ 0 ] = "Band";
   CalendarGrid->Cells[ 2 ][ 0 ] = "Start (UTC)";
   CalendarGrid->Cells[ 3 ][ 0 ] = "Finish (UTC)";
   CalendarGrid->Cells[ 4 ][ 0 ] = "Scoring";
   CalendarGrid->Cells[ 5 ][ 0 ] = "Mults";
   CalendarGrid->Cells[ 6 ][ 0 ] = "Sections";
   CalendarGrid->Cells[ 7 ][ 0 ] = "Special Rules";
   int row = 1;
   int nextContest = 0;
   TDateTime now = TDateTime::CurrentDateTime();
   for ( std::vector<IndividualContest>::iterator i = vhf.calendar.begin(); i != vhf.calendar.end(); i++ )
   {
      if (!description.IsEmpty())
      {
         if (nextContest == 0 && description == (*i).description.c_str())
         {
            nextContest = row;
         }
      }
      else
      {
         if (nextContest == 0 && now <= (*i).start)
         {
            nextContest = row;
         }
      }
      CalendarGrid->Cells[ 0 ][ row ] = ( *i ).description.c_str();
      CalendarGrid->Cells[ 1 ][ row ] = ( *i ).bands.c_str();
      CalendarGrid->Cells[ 2 ][ row ] = ( *i ).start.FormatString( "dd/mm/yyyy hh:mm" );
      CalendarGrid->Cells[ 3 ][ row ] = ( *i ).finish.FormatString( "dd/mm/yyyy hh:mm" );
      CalendarGrid->Cells[ 4 ][ row ] = ( *i ).ppKmScoring ? "1Pt/Km" : "1Pt/QSO";
      CalendarGrid->Cells[ 5 ][ row ] = ( *i ).mults.c_str();
      CalendarGrid->Cells[ 6 ][ row ] = ( *i ).sections.c_str();
      CalendarGrid->Cells[ 7 ][ row ] = ( *i ).specialRules.c_str();
      row++;
   }
   if (nextContest)
   {
      CalendarGrid->Row = nextContest;   
   }
}
//---------------------------------------------------------------------------
void __fastcall TCalendarForm::GetCalendarButtonClick( TObject *Sender )
{
   TWaitCursor fred;
   AnsiString calendarURL = "http://www.vhfcc.org/vhfcontests.xml";
   try
   {
      String cal = IdHTTP1->Get( calendarURL );
      // and write it out

      char appFName[ 255 ];
      int nLen = 0;
      nLen = ::GetModuleFileName( HInstance, appFName, 255 );
      appFName[ nLen ] = '\0';

      String fpath = ExtractFilePath( appFName );

      TStringList *sl = new TStringList();
      sl->Text = cal;
      sl->SaveToFile( fpath + "Configuration\\vhfcontests.xml" );
      delete sl;
      FormShow( Sender );
   }
   catch ( Exception & e )
   {
      ShowMessage( "HTPP Get of " + calendarURL + " failed: " + e.Message );
   }
}
//---------------------------------------------------------------------------

void __fastcall TCalendarForm::SelectButtonClick( TObject * /*Sender*/ )
{
   ic = vhf.calendar[ CalendarGrid->Row - 1 ];
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TCalendarForm::CalendarGridDblClick( TObject * /*Sender*/ )
{
   ic = vhf.calendar[ CalendarGrid->Row - 1 ];
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TCalendarForm::YearSpinEditBottomClick(TObject *Sender)
{
   FormShow( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TCalendarForm::YearSpinEditTopClick(TObject *Sender)
{
   FormShow( Sender );
}
//---------------------------------------------------------------------------

