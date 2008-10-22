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

#include <deque>
#include <string>
#include <vector>
#include <inifiles.hpp>

#define _MSC_VER 2300
#include "sorted_vector.h"
#include "WaitCursor.h"
#include "mtrace.h"

#include "ServerEvent.h"
#include "GJVThreads.h"
#include "LogEvents.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"
#include "XMPPRPCObj.h"
#include "RPCPubSub.h"
#include "BandMapRPCObj.h"
#include "TreeUtils.h"

#include "BandMap.h" 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
TBandMapForm *BandMapForm = 0;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*static*/
void TBandMapForm::makeRPCObjects()
{
   RPCPubSub::initialisePubSub( new TRPCCallback <TBandMapForm> ( BandMapForm, &TBandMapForm::notifyCallback ) );
   MinosRPCObj::addObj( new RPCBandMapClient( new TRPCCallback <TBandMapForm> ( BandMapForm, &TBandMapForm::bandMapClientCallback ) ) );
   MinosRPCObj::addObj( new RPCBandMapServer( new TRPCCallback <TBandMapForm> ( BandMapForm, &TBandMapForm::bandMapServerCallback ) ) );
}
//---------------------------------------------------------------------------
__fastcall TBandMapForm::TBandMapForm( TComponent* Owner )
      : TForm( Owner ), connected( false ), subscribed( false ), saveResize( false )
{
   enableTrace( ".\\TraceLog\\MinosBandMap_" );
}
//---------------------------------------------------------------------------
void __fastcall TBandMapForm::Timer1Timer( TObject */*Sender*/ )
{
   if ( checkCloseEvent() )
   {
      Close();
   }
   else
      if ( !connected && checkServerReady() )
      {
         makeRPCObjects();
         XMPPInitialise( "BandMap" );
         connected = true;

      }
      else
         if ( connected && !subscribed )
         {
            // Subscribe to rig frequency
            RPCPubSub::subscribe( "RigControl" );
            subscribed = true;

            // and publish that we are up and running
            RPCPubSub::publish( "BandMap", "Loaded", "true", psPublished );
         }

}
//---------------------------------------------------------------------------

void __fastcall TBandMapForm::LogTimerTimer( TObject */*Sender*/ )
{
   // check log queue; if anything on it then log to main window
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         trace( "Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   FreqLabel->Caption = FreqLabelStr.c_str();
   LogLabel->Caption = LogLabelStr.c_str();
}
//---------------------------------------------------------------------------
void TBandMapForm::bandMapServerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "BandMap client callback from " + from + ( err ? ":Error" : ":Normal" ) );
   // transfer from contact to bandmap

   // look at what we have been sent
   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) )
      {
         std::string Name;

         if ( psName->getString( Name ) )
         {
            if ( Name == "BandMap" )
            {
               boost::shared_ptr<RPCParam> psFreq;
               std::string Freq;
               boost::shared_ptr<RPCParam>psUTC;
               std::string UTC;
               boost::shared_ptr<RPCParam>psCall;
               std::string Call;
               boost::shared_ptr<RPCParam>psLoc;
               std::string Loc;
               boost::shared_ptr<RPCParam>psQTH;
               std::string QTH;

               bool res;
               // dtg
               res = args->getStructArgMember( 0, "Freq", psFreq );
               res &= args->getStructArgMember( 0, "UTC", psUTC );
               res &= args->getStructArgMember( 0, "Callsign", psCall );
               res &= args->getStructArgMember( 0, "Locator", psLoc );
               res &= args->getStructArgMember( 0, "QTH", psQTH );
               // comments
               if ( res )
               {
                  res &= psFreq->getString( Freq );
                  res &= psUTC->getDtg( UTC );
                  res &= psCall->getString( Call );
                  res &= psLoc->getString( Loc );
                  res &= psQTH->getString( QTH );
                  if ( res )
                  {
                     // Need to match up against same call already mapped
                     // ?? just add new entries, and hide old ones, or what?

                     BMEntry * bm = new BMEntry;
                     bm->freq = Freq;
                     bm->UTC = UTC;
                     bm->csCs = Call;
                     bm->loc = Loc;
                     bm->qth = QTH;
                     bmlist.insert( bm );

                     LogLabelStr = Call + " | " + Loc + " | " + QTH;
                     //                     BMTree->Clear();
                     BMTree->RootNodeCount = bmlist.size();  // The magic of virtual displays...
                  }
                  else
                     LogLabelStr = "Failure!";
               }
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void TBandMapForm::bandMapClientCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   trace( "BandMap server callback from " + from + ( err ? ":Error" : ":Normal" ) );
   // transfer from bandmap to new contact
}
//---------------------------------------------------------------------------
void TBandMapForm::notifyCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "Notify callback from " + from + ( err ? ":Error" : ":Normal" ) );
   AnalysePubSubNotify an( err, mro );

   // called whenever frequency changes
   if ( an.getOK() )
   {
      if ( an.getCategory() == "RigControl" && an.getKey() == "Frequency" )
      {
         FreqLabelStr = an.getValue();
      }
   }
}
//---------------------------------------------------------------------------
enum eBMGridCols {
   ebmFreq,
   ebmTime,
   ebmCall,
   ebmLoc,
   ebmQTH, ebmComments,
   ebmMaxCol
};
static GridColumn BMTreeColumns[ ebmMaxCol ] =
   {
      GridColumn( ebmFreq, "XX1296.200000XX", "Freq", taLeftJustify ),               // time
      GridColumn( ebmTime, "XXXXXXX", "UTC", taLeftJustify ),               // time
      GridColumn( ebmCall, "MMMMMMMMM", "Callsign", taLeftJustify ),         // call
      GridColumn( ebmLoc, "MM00MM00X", "Loc", taLeftJustify ),            // LOC
      GridColumn( ebmQTH, "XXXXXXXXXXXXXXXX", "QTH", taLeftJustify ),    // QTH
      GridColumn( ebmComments, "XXXXXXXXXXXXXXXX", "Comments", taLeftJustify )     // comments
   };
void TBandMapForm::showBM()
{
   BMTree->Canvas->Font->Assign( Font );
   BMTree->Header->Options = ( BMTree->Header->Options << hoVisible );

   BMTree->Header->Columns->Clear();
   BMTree->Margin = 0;
   BMTree->TextMargin = 4;
   for ( int i = 0; i < ebmMaxCol; i++ )
   {
      TVirtualTreeColumn *NewColumn = BMTree->Header->Columns->Add();
      NewColumn->Alignment = BMTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      NewColumn->Width = BMTree->Canvas->TextWidth( BMTreeColumns[ i ].width );

      NewColumn->Text = BMTreeColumns[ i ].title;
   }

}

void __fastcall TBandMapForm::BMTreeGetText( TBaseVirtualTree */*Sender*/,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType /*TextType*/,
	  UnicodeString &CellText )
{
   if ( Node->Index >= bmlist.size() )
   {
      if ( Node->Index == 1 )
         CellText = "No calls";
      else
         CellText = "";
      return ;
   }
   BMEntry *bm = bmlist[ Node->Index ];
   if ( bm && Column >= 0 )
   {
      switch ( BMTreeColumns[ Column ].fieldId )
      {
         case ebmFreq:
            CellText = bm->freq.c_str();
            break;
         case ebmTime:
            {
               // This is a nasty ISO format time...
               String utc = bm->UTC.c_str();
               int pos = utc.Pos( 'T' );
               if ( pos > 0 )
               {
                  utc = utc.SubString( pos + 1, 5 );
               }
               CellText = utc;
            }
            break;
         case ebmCall:
            CellText = bm->csCs.c_str();
            break;
         case ebmLoc:
            CellText = bm->loc.c_str();
            break;
         case ebmQTH:
            CellText = bm->qth.c_str();
            break;
         case ebmComments:
            CellText = bm->comments.c_str();
            break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TBandMapForm::BMTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = 0;
}
//---------------------------------------------------------------------------

void __fastcall TBandMapForm::FormShow( TObject */*Sender*/ )
{
   StartupTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TBandMapForm::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;

   ForceDirectories( ".\\Configuration" );
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosBandMap.ini" ) );

   int L = Left, T = Top, W = Width, H = Height;
   L = iniFile->ReadInteger( "Display", "Left", L );
   T = iniFile->ReadInteger( "Display", "Top", T );
   W = iniFile->ReadInteger( "Display", "Width", W );
   H = iniFile->ReadInteger( "Display", "Height", H );
   SetBounds( L, T, W, H );
   Repaint();
   saveResize = true;

   showBM();
}
//---------------------------------------------------------------------------

void __fastcall TBandMapForm::FormClose( TObject *Sender, TCloseAction &/*Action*/ )
{
   saveResize = false;
   MinosRPCObj::clearRPCObjects();
   XMPPClosedown();
   LogTimerTimer( Sender );
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------
void __fastcall TBandMapForm::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosBandMap.ini" ) );
      iniFile->WriteInteger( "Display", "Left", Left );
      iniFile->WriteInteger( "Display", "Top", Top );
      iniFile->WriteInteger( "Display", "Width", Width );
      iniFile->WriteInteger( "Display", "Height", Height );
      iniFile->UpdateFile();
   }
}
void __fastcall TBandMapForm::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

