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
#include "EntryOptions.h"
#include "enqdlg.h"
#include "reg1test.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEntryOptionsForm *EntryOptionsForm;
//---------------------------------------------------------------------------
__fastcall TEntryOptionsForm::TEntryOptionsForm( TComponent* Owner, LoggerContestLog * cnt, bool saveMinos )
      : TForm( Owner ), ct( cnt ), minosSave( saveMinos )
{}
//---------------------------------------------------------------------------
void __fastcall TEntryOptionsForm::FormShow( TObject */*Sender*/ )
{
   if ( !ct )
      return ;
   DetailGrid->RowCount = 32;
   DetailGrid->ColCount = 2;

   int r = 0;
   DetailGrid->Cells[ 0 ][ r ] = "Field";
   DetailGrid->Cells[ 1 ][ r ] = "Value";
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Date Range (Calculated)";
   DetailGrid->Cells[ 1 ][ r ] = ct->dateRange( DTGDISP ).c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Contest Name";
   DetailGrid->Cells[ 1 ][ r ] = ct->name.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Band";
   DetailGrid->Cells[ 1 ][ r ] = ct->band.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Entrant name (or group)";
   DetailGrid->Cells[ 1 ][ r ] = ct->entrant.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Station QTH 1";
   DetailGrid->Cells[ 1 ][ r ] = ct->sqth1.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Station QTH 2";
   DetailGrid->Cells[ 1 ][ r ] = ct->sqth2.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Section";
   DetailGrid->Cells[ 1 ][ r ] = ct->entSect.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Callsign";
   DetailGrid->Cells[ 1 ][ r ] = ct->mycall.fullCall.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Locator";
   DetailGrid->Cells[ 1 ][ r ] = ct->myloc.loc.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Exchange/code/QTH sent";
   DetailGrid->Cells[ 1 ][ r ] = ct->location.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Transmitter";
   DetailGrid->Cells[ 1 ][ r ] = ct->entTx.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Transmit Power";
   DetailGrid->Cells[ 1 ][ r ] = ct->power.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Receiver";
   DetailGrid->Cells[ 1 ][ r ] = ct->entRx.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Antenna";
   DetailGrid->Cells[ 1 ][ r ] = ct->entAnt.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Height above ground";
   DetailGrid->Cells[ 1 ][ r ] = ct->entAGL.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Height above sea level";
   DetailGrid->Cells[ 1 ][ r ] = ct->entASL.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Operators Line 1";
   DetailGrid->Cells[ 1 ][ r ] = ct->ops1.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Operators Line 2";
   DetailGrid->Cells[ 1 ][ r ] = ct->ops2.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Conditions/Comments";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCondx1.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Conditions/Comments";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCondx2.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Conditions/Comments";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCondx3.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Conditions/Comments";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCondx4.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Name for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entName.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Callsign for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCall.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Address 1 for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entAddr1.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Address 2 for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entAddr2.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "City for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCity.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Country for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entCountry.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Postcode for Correspondence";
   DetailGrid->Cells[ 1 ][ r ] = ct->entPostCode.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "Phone number for queries";
   DetailGrid->Cells[ 1 ][ r ] = ct->entPhone.getValue().c_str();
   r++;

   DetailGrid->Cells[ 0 ][ r ] = "email address for queries";            //31
   DetailGrid->Cells[ 1 ][ r ] = ct->entEMail.getValue().c_str();
   r++;


   if ( DetailGrid->RowCount != r )
   {
      ShowMessage( "Sizing of DetailGrid is wrong!" );
   }
   if ( minosSave )
   {
      EntryGroup->ItemIndex = EMINOS;
      EntryGroup->Visible = false;
   }
   else
   {
      EntryGroup->ItemIndex = EREG1TEST;
   }
   DetailGrid->Row = 2;
}
//---------------------------------------------------------------------------
void __fastcall TEntryOptionsForm::CloseButtonClick( TObject */*Sender*/ )
{
   int r = 1;
   r++;  // date range not editable
   ct->name.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->band.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entrant.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->sqth1.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->sqth2.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entSect.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->mycall.fullCall.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->myloc.loc.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->location.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entTx.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->power.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entRx.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entAnt.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entAGL.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entASL.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->ops1.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->ops2.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCondx1.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCondx2.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCondx3.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCondx4.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entName.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCall.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entAddr1.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entAddr2.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCity.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entCountry.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entPostCode.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entPhone.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );
   ct->entEMail.setValue( DetailGrid->Cells[ 1 ][ r++ ].c_str() );

   //enum ExportType {EREG1TEST, EADIF, EG0GJV, EMINOS, EKML, ESUMMARY };
   expformat = ( ExportType ) EntryGroup->ItemIndex;

   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TEntryOptionsForm::CancelButtonClick( TObject */*Sender*/ )
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
String TEntryOptionsForm::doFileSave( )
{
   SaveDialog->InitialDir = ExtractFilePath( ct->cfileName.c_str() );

   String expName;
   String fname = ExtractFileName( ct->cfileName.c_str() );
   String ext = ExtractFileExt( ct->cfileName.c_str() );
   fname = fname.SubString( 1, fname.Length() - ext.Length() );

   //enum ExportType {EREG1TEST, EADIF, EG0GJV, EMINOS, EKML, ESUMMARY };
   String defext;
   String filter;
   switch ( expformat )
   {
      case ESUMMARY:
         defext = "TXT";
         filter = "Summary file (*.txt)|*.TXT|All Files (*.*)|*.*" ;
         break;
      case EG0GJV:
         defext = "GJV";
         filter = "GJV ContestLog files (*.gjv)|*.GJV|All Files (*.*)|*.*" ;
         break;
      case EMINOS:
         defext = "Minos";
         filter = "Minos ContestLog files (*.minos)|*.MINOS|All Files (*.*)|*.*" ;
         break;
      case EADIF:
         defext = "ADI";
         filter = "ADIF files (*.adi)|*.ADI|All Files (*.*)|*.*" ;
         break;
      case EKML:
         defext = "KML";
         filter = "KML(GoogleEarth) files (*.kml)|*.KML|All Files (*.*)|*.*" ;
         break;
      case EREG1TEST:
         {
            defext = "EDI";
            // default file name as call
            ct->mycall.valRes = CS_NOT_VALIDATED;
            ct->mycall.validate( );
            fname = ( ct->mycall.prefix2 + ct->mycall.number + ct->mycall.body ).c_str();
            filter = "Region 1 EDI files (*.edi)|*.EDI|All Files (*.*)|*.*" ;
         }
         break;
   }
   SaveDialog->FileName = fname;
   SaveDialog->DefaultExt = defext;
   SaveDialog->Filter = filter;
   while ( SaveDialog->Execute() )
   {
      TWaitCursor fred;
      expName = SaveDialog->FileName;

      // open the export file
      struct stat sbuf;
      if ( stat( expName.c_str(), &sbuf ) == 0 )
      {
         if ( !( sbuf.st_mode & S_IWRITE ) )
         {
            MinosParameters::getMinosParameters() ->mshowMessage( "File is Read Only" );
            return "";
         }
      }
      if ( MinosParameters::getMinosParameters() ->isContestOpen( expName.c_str() ) )
      {
         // then try again...
         continue;
      }

      HANDLE expfd = CreateFile( expName.c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ,
                                 0,                  // security
                                 CREATE_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL,
                                 0 );               // template handle
      if ( expfd == INVALID_HANDLE_VALUE )
      {
         char * emess = _strerror( "Failed to create export file" );
         MinosParameters::getMinosParameters() ->mshowMessage( emess );
         return "";
      }

      int ret = ct->export_contest( expfd, expformat );
      CloseHandle( expfd );

      if ( ret == -1 )
      {
         unlink( expName.c_str() );		// failure response, so delete file again
         expName = "";
      }
      break;
   }
   return expName;
}
//---------------------------------------------------------------------------

void __fastcall TEntryOptionsForm::DetailGridSelectCell( TObject */*Sender*/,
      int ACol, int ARow, bool &CanSelect )
{
   if ( ACol == 0 || ARow == 0 || ARow == 1 )    // label column/row or date range cell
   {
      CanSelect = false;
   }
   else
      CanSelect = true;
}
//---------------------------------------------------------------------------

