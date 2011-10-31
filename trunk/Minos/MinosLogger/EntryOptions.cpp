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
#include<boost/tokenizer.hpp>

#include "LoggerContest.h"
#include "EntryOptions.h"
#include "enqdlg.h"
#include "reg1test.h"
#include "optionsframe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEntryOptionsForm *EntryOptionsForm;
//---------------------------------------------------------------------------
__fastcall TEntryOptionsForm::TEntryOptionsForm( TComponent* Owner, LoggerContestLog * cnt, bool saveMinos )
      : TForm( Owner ), ct( cnt ), minosSave( saveMinos ), opsQSOLine1(-1), opsQSOLine2(-1),
      opsEntryLine1(-1), opsEntryLine2(-1)
//      , GridHintWindow( 0 ), oldX( 0 ), oldY( 0 )
{
//   GridHintWindow = new TGridHint( this );
 //  GridHintWindow->SetHintControl( DetailGrid );
}
   // We want to treat ops1 and ops2 as CSV lists, and get all the ops
   // Then we need to go through oplist and add any that we don't
   // find in ops1/ops2
   // Finally, sort the resulting list, and split between the two lines equally
   // writing  back to ops1, ops2
void TEntryOptionsForm::getContestOperators()
{
   OperatorList operators;

   for ( OperatorIterator op = ct->oplist.begin(); op != ct->oplist.end(); op++ )
   {
      operators.insert(*op);
   }

   // now actual ops are a sorted list

   std::string ops1;
   std::string ops2;
   unsigned int ls = operators.size();
   for (unsigned int i = 0; i < ls; i++)
   {
      if (i <= ls/2)
      {
         ops1 += operators[i] + " ";
      }
      else
      {
         ops2 += operators[i] + " ";
      }
   }
   ct->opsQSO1 = trim(ops1);
   ct->opsQSO2 = trim(ops2);
}
//---------------------------------------------------------------------------
void __fastcall TEntryOptionsForm::FormShow( TObject */*Sender*/ )
{
   MinosParameters::getMinosParameters() ->applyFontChange(this);
   if ( !ct )
      return ;

   int etop = 0;
   for (int i = 0; i < 33; i++)
   {
      TOptionFrame *tcf = new TOptionFrame( this );
      options.push_back(tcf);

      tcf->Name = "entryopt" + String(i);
      tcf->Parent = OptionsScrollBox;
      tcf->Top = etop;
      tcf->Width = OptionsScrollBox->Width;
      etop += tcf->Height;
   }
   int r = 0;

   options[ r ]->OptionLabel->Caption = "Date Range (Calculated)";
   options[ r ]->OptionEdit->Text = ct->dateRange( DTGDISP ).c_str();
   options[ r ]->OptionEdit->ReadOnly = true;
   r++;

   options[ r ]->OptionLabel->Caption = "Contest Name";
   options[ r ]->OptionEdit->Text = ct->name.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Band";
   options[ r ]->OptionEdit->Text = ct->band.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Entrant name (or group)";
   options[ r ]->OptionEdit->Text = ct->entrant.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Station QTH 1";
   options[ r ]->OptionEdit->Text = ct->sqth1.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Station QTH 2";
   options[ r ]->OptionEdit->Text = ct->sqth2.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Section";
   options[ r ]->OptionEdit->Text = ct->entSect.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Callsign";
   options[ r ]->OptionEdit->Text = ct->mycall.fullCall.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Locator";
   options[ r ]->OptionEdit->Text = ct->myloc.loc.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Exchange/code/QTH sent";
   options[ r ]->OptionEdit->Text = ct->location.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Transmitter";
   options[ r ]->OptionEdit->Text = ct->entTx.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Transmit Power";
   options[ r ]->OptionEdit->Text = ct->power.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Receiver";
   options[ r ]->OptionEdit->Text = ct->entRx.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Antenna";
   options[ r ]->OptionEdit->Text = ct->entAnt.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Height above ground";
   options[ r ]->OptionEdit->Text = ct->entAGL.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Height above sea level";
   options[ r ]->OptionEdit->Text = ct->entASL.getValue().c_str();
   r++;

   getContestOperators();

   opsQSOLine1 = r;
   options[ r ]->OptionLabel->Caption = "(From QSOs) Operators Line 1";
   options[ r ]->OptionEdit->Text = ct->opsQSO1.c_str();
   options[ r ]->OptionEdit->ReadOnly = true;
   r++;

   opsQSOLine2 = r;
   options[ r ]->OptionLabel->Caption = "(From QSOs) Operators Line 2";
   options[ r ]->OptionEdit->Text = ct->opsQSO2.c_str();
   options[ r ]->OptionEdit->ReadOnly = true;
   r++;

   opsEntryLine1 = r;
   options[ r ]->OptionLabel->Caption = "(Entry)Operators Line 1";
   options[ r ]->OptionEdit->Text = ct->ops1.getValue().c_str();
   r++;

   opsEntryLine2 = r;
   options[ r ]->OptionLabel->Caption = "(Entry)Operators Line 2";
   options[ r ]->OptionEdit->Text = ct->ops2.getValue().c_str();
   r++;


   options[ r ]->OptionLabel->Caption = "Conditions/Comments";
   options[ r ]->OptionEdit->Text = ct->entCondx1.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Conditions/Comments";
   options[ r ]->OptionEdit->Text = ct->entCondx2.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Conditions/Comments";
   options[ r ]->OptionEdit->Text = ct->entCondx3.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Conditions/Comments";
   options[ r ]->OptionEdit->Text = ct->entCondx4.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Name for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entName.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Callsign for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entCall.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Address 1 for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entAddr1.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Address 2 for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entAddr2.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "City for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entCity.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Country for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entCountry.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Postcode for Correspondence";
   options[ r ]->OptionEdit->Text = ct->entPostCode.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "Phone number for queries";
   options[ r ]->OptionEdit->Text = ct->entPhone.getValue().c_str();
   r++;

   options[ r ]->OptionLabel->Caption = "email address for queries";            //31
   options[ r ]->OptionEdit->Text = ct->entEMail.getValue().c_str();
   r++;


   if ( minosSave )
   {
      EntryGroup->ItemIndex = EMINOS;
      EntryGroup->Visible = false;
   }
   else
   {
      EntryGroup->ItemIndex = EREG1TEST;
   }
}
//---------------------------------------------------------------------------
void __fastcall TEntryOptionsForm::CloseButtonClick( TObject */*Sender*/ )
{
   int r = 0;
   r++;  // date range not editable
   ct->name.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->band.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entrant.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->sqth1.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->sqth2.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entSect.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->mycall.fullCall.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->myloc.loc.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->location.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entTx.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->power.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entRx.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entAnt.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entAGL.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entASL.setValue( options[ r++ ]->OptionEdit->Text.c_str() );

   ct->opsQSO1 = options[ r++ ]->OptionEdit->Text.c_str() ;
   ct->opsQSO2 = options[ r++ ]->OptionEdit->Text.c_str() ;

   ct->ops1.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->ops2.setValue( options[ r++ ]->OptionEdit->Text.c_str() );

   ct->entCondx1.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entCondx2.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entCondx3.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entCondx4.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entName.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entCall.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entAddr1.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entAddr2.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entCity.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entCountry.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entPostCode.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entPhone.setValue( options[ r++ ]->OptionEdit->Text.c_str() );
   ct->entEMail.setValue( options[ r++ ]->OptionEdit->Text.c_str() );

   //enum ExportType {EREG1TEST, EADIF, EG0GJV, EMINOS, EKML};
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

   //enum ExportType {EREG1TEST, EADIF, EG0GJV, EMINOS, EKML, EPRINTFILE };
   String defext;
   String filter;
   switch ( expformat )
   {
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
            ct->mycall.valRes = CS_NOT_VALIDATED;
            ct->mycall.validate( );
            filter = "Region 1 EDI files (*.edi)|*.EDI|All Files (*.*)|*.*" ;
         }
         break;
      case EPRINTFILE:
      {
         defext = "TXT";
         filter = "Text output (*.txt)|*.TXT|All Files (*.*)|*.*" ;
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







