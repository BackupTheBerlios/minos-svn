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

#include "LocCalc.h"
#include "latlong.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLocCalcForm *LocCalcForm = 0;
//---------------------------------------------------------------------------
__fastcall TLocCalcForm::TLocCalcForm( TComponent* Owner )
      : TForm( Owner ), Modal( false )
{}
//---------------------------------------------------------------------------
__fastcall TLocCalcForm::~TLocCalcForm()
{
   if ( !Modal )
      LocCalcForm = 0;
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::S1LocExit( TObject */*Sender*/ )
{
   handleExit( S1Loc );
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::S1LatLongExit( TObject */*Sender*/ )
{
   handleExit( S1LatLong );
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::S1NGRExit( TObject */*Sender*/ )
{
   handleExit( S1NGR );
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::S2LocExit( TObject */*Sender*/ )
{
   handleExit( S2Loc );
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::S2LatLongExit( TObject */*Sender*/ )
{
   handleExit( S2LatLong );
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::S2NGRExit( TObject */*Sender*/ )
{
   handleExit( S2NGR );
}
//---------------------------------------------------------------------------
bool __fastcall TLocCalcForm::handleExit( TEdit *Edit )
{
   Location l1, l2;
   String inputbuff;
   gridstyle gstyle = NUL;

   TEdit *locIl;
   TEdit *geoIl;
   TEdit *ngrIl;
   switch ( Edit->Tag )
   {
      case 0:
         gstyle = LOC;
         locIl = S1Loc;
         geoIl = S1LatLong;
         ngrIl = S1NGR;
         break;
      case 1:
         gstyle = GEO;
         locIl = S1Loc;
         geoIl = S1LatLong;
         ngrIl = S1NGR;
         break;
      case 2:
         gstyle = NGR;
         l1.centremeridian = degrad( -2.0 );
         locIl = S1Loc;
         geoIl = S1LatLong;
         ngrIl = S1NGR;
         break;

      case 10:
         gstyle = LOC;
         locIl = S2Loc;
         geoIl = S2LatLong;
         ngrIl = S2NGR;
         break;
      case 11:
         gstyle = GEO;
         locIl = S2Loc;
         geoIl = S2LatLong;
         ngrIl = S2NGR;
         break;
      case 12:
         gstyle = NGR;
         locIl = S2Loc;
         geoIl = S2LatLong;
         ngrIl = S2NGR;
         l1.centremeridian = degrad( -2.0 );
         break;

      default:
         return false;
   }
   inputbuff = Edit->Text.Trim().UpperCase();

   l1.gridstyle = gstyle;
   l1.datastring = inputbuff.c_str();
   l1.datalength = inputbuff.Length() + 1;
   l1.dataoffset = 0;

   if ( gstyle != NGR )
   {
      char outputbuff[ 100 ];
      l2.datastring = outputbuff;
      l2.gridstyle = NGR;
      l2.datalength = 20;
      l2.dataoffset = 0;
      l2.centremeridian = degrad( -2.0 );

      ngrIl->Text = ( ( transform( &l1, &l2 ) == GRIDOK ) ? outputbuff : "" );
   }

   if ( gstyle != LOC )
   {
      char outputbuff[ 100 ];
      l2.datastring = outputbuff;
      l2.gridstyle = LOC;
      l2.datalength = 20;
      l2.dataoffset = 0;

      locIl->Text = ( ( transform( &l1, &l2 ) == GRIDOK ) ? outputbuff : "" );
   }

   if ( gstyle != GEO )
   {
      char outputbuff[ 100 ];
      l2.datastring = outputbuff;
      l2.gridstyle = GEO;
      l2.datalength = 40;
      l2.dataoffset = 0;

      geoIl->Text = ( ( transform( &l1, &l2 ) == GRIDOK ) ? outputbuff : "" );
   }
   return true;
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::CalcButtonClick( TObject */*Sender*/ )
{
   // Now calculate distance to the current contest locator

   double dist = 0.0;
   int brg;
   double longitude = 0.0;
   double latitude = 0.0;

   if ( handleExit( S1Loc ) && handleExit( S2Loc ) )
   {
      BaseContestLog cnt;
      cnt.myloc.loc.setValue( S1Loc->Text.UpperCase().c_str() );
      cnt.validateLoc();

      if ( lonlat( S2Loc->Text.UpperCase().c_str(), longitude, latitude ) == LOC_OK )
      {
         cnt.disbear( longitude, latitude, dist, brg );
         if ( Modal )
            Distance->Text = String( ( int ) dist );
         else
            Distance->Text = String( "Dist " ) + ( int ) dist + " km " + brg + " degrees";
      }
   }
}

void __fastcall TLocCalcForm::ExitButtonClick( TObject *Sender )
{
   if ( Modal )
   {
      if ( Distance->Text.Trim().Length() == 0 )
      {
         CalcButtonClick( Sender );
      }
      ModalResult = mrOk;
   }
   else
      Close();
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::CancelButtonClick( TObject */*Sender*/ )
{
   if ( Modal )
      ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TLocCalcForm::FormShow( TObject *Sender )
{
   MinosParameters::getMinosParameters() ->applyFontChange(this);
   Modal = FormState.Contains( fsModal );
   CalcButtonClick(Sender);
}
//---------------------------------------------------------------------------

