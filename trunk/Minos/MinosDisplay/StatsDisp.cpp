/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "display_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
__fastcall TStatsDispFrame::TStatsDispFrame( TComponent* Owner )
      : TFrame( Owner ), ct( 0 )
{}
void TStatsDispFrame::setContest( BaseContestLog *pct )
{
   ct = pct;
}
//---------------------------------------------------------------------------
void TStatsDispFrame::reInitialise()
{
   // we need to set the operator bundle to the current contest ops
   if ( !P1Edit->Focused() && !P2Edit->Focused() )
   {
      P1Edit->Text = String( MinosParameters::getMinosParameters() ->getStatsPeriod1() );
      P2Edit->Text = String( MinosParameters::getMinosParameters() ->getStatsPeriod2() );
   }

   SLabel1->Caption = "";

   if ( !ct )
   {
      // not there...
      return ;
   }

   BaseContact *bestdx = ct->getBestDX();

   if ( !bestdx || ct->maxSerial <= 0 )
      return ;

   int ltot = 0;
   if ( ct->countryMult.getValue() )
      ltot += ct->nctry ;
   if ( ct->districtMult.getValue() )
      ltot += ct->ndistrict;
   if ( ct->locMult.getValue() )
      ltot += ct->nlocs;

   ct->updateStats();
   int nvalid = ct->getValidQSOs();
   if (nvalid)
   {
      double avpts = ( double ) ct->contestScore / nvalid;
      std::string tempcs;
      tempcs = trim( bestdx->cs.fullCall.getValue() ) ;

      std::string buff = ( boost::format( "Best DX %s Sn %s %s(%d kms). Average QSO is %.1f Points" )
                           //#define MAX_DISPLAY_TEST 1
   #ifndef MAX_DISPLAY_TEST
                           % tempcs % bestdx->serials.getValue() %
                           bestdx->loc.loc.getValue() % bestdx->contactScore.getValue() %
                           avpts
   #else
                           % "PE0/LX5ABC/P" % "9999" % "XX99XX" % 29999 % 9999.9
   #endif
                         ).str();
      int sp1 = MinosParameters::getMinosParameters() ->getStatsPeriod1();
      int sp2 = MinosParameters::getMinosParameters() ->getStatsPeriod2();
      TDateTime  contestStart = CanonicalToTDT(ct->DTGStart.getValue().c_str());
      TDateTime diff = TDateTime::CurrentDateTime() - contestStart;
      int fromContestStart = (double(diff)) * 1440;
      if (sp1 > fromContestStart/2)
      {
         sp1 = fromContestStart/2;
      }
      if (sp2 > fromContestStart/2)
      {
         sp2 = fromContestStart/2;
      }
      if ( ltot )
      {
         double qmult = ( double ) nvalid / ltot;
         std::string temp;

         long Qs = ct->QSO1 ? ct->QSO1 : ct->QSO2;
         if ( Qs )
         {
            // Why the selection of period in this way?  kms1 is the distance for the first period; use that unless it is zero
            int period = ct->kms1 ? sp1 : sp2;
            double qmins = ( qmult * period ) / Qs;
            temp = ( boost::format( " (%.1f Mins)" ) % qmins ).str();
         }

         std::string lbuff = ( boost::format( "%s\nOne New Multiplier is equivalent to %.1f Average QSOs%s" ) % buff
   #ifndef MAX_DISPLAY_TEST
                               % qmult % temp
   #else
                               % 9999.9 % " (9999.9 Mins)"
   #endif
                             ).str();
         buff = lbuff ;
      }

      std::string lbuff = ( boost::format( "Last %d Mins: %d QSO %ld pts %d mults\r\n(Previous %d; %ld; %d)"
                                           "\nLast %d Mins: %d QSO %ld pts %d mults\r\n(Previous %d; %ld; %d )" )
   #ifndef MAX_DISPLAY_TEST
                            % sp1 % ct->QSO1 % ct->kms1 % ct->mults1
                            % ct->QSO1p % ct->kms1p % ct->mults1p
                            % sp2 % ct->QSO2 % ct->kms2 % ct->mults2
                            % ct->QSO2p % ct->kms2p % ct->mults2p
   #else

                            % 999 % 1999 % 999999L % 999 % 1999 % 999999L % 999
                            % 999 % 1999 % 999999L % 999 % 1999 % 999999L % 999
   #endif
                          ).str();
      SLabel1->Caption = String( ( buff + "\r\n\r\n" + lbuff ).c_str() );
   }
}
void __fastcall TStatsDispFrame::RecheckTimerTimer( TObject */*Sender*/ )
{
   if ( !Visible )
      return ;

   reInitialise();
}
//---------------------------------------------------------------------------
void __fastcall TStatsDispFrame::P2EditKeyPress( TObject */*Sender*/, char &Key )
{
   if ( Key == '\b' || ( Key >= '0' && Key <= '9' ) )
      return ;

   Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TStatsDispFrame::P1EditExit(TObject */*Sender*/)
{
   MinosParameters::getMinosParameters() ->setStatsPeriod1( P1Edit->Text.ToIntDef( 10 ) );
   reInitialise();
}
//---------------------------------------------------------------------------

void __fastcall TStatsDispFrame::P2EditExit(TObject */*Sender*/)
{
   MinosParameters::getMinosParameters() ->setStatsPeriod2( P2Edit->Text.ToIntDef( 60 ) );
   reInitialise();
}
//---------------------------------------------------------------------------

