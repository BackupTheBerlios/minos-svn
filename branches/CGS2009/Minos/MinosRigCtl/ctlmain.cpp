/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <deque>
#include <string>
#include <vector>
#include <stdio.h>
#include <inifiles.hpp>
#include <boost\shared_ptr.hpp>

#include "mtrace.h"
#include "GJVThreads.h"
#include "LogEvents.h"
#include "ServerEvent.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"
#include "XMPPRPCObj.h"
#include "RPCPubSub.h" 
//---------------------------------------------------------------------------
#include "hamlib\rig.h"
#include "ctlmain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRigCtlMain *RigCtlMain;
//---------------------------------------------------------------------------
/*static*/
void makeRPCObjects()
{
   RPCPubSub::initialisePubSub( 0 );   // we don't subscribe, so no callback needed
}
//---------------------------------------------------------------------------
__fastcall TRigCtlMain::TRigCtlMain( TComponent* Owner )
      : TForm( Owner ), my_rig( 0 )
{
   enableTrace( ".\\TraceLog\\MinosRigCtl_" );
}
//---------------------------------------------------------------------------
void __fastcall TRigCtlMain::logMessage( std::string s )
{
   trace( s );
}
//---------------------------------------------------------------------------

void __fastcall TRigCtlMain::LogTimerTimer( TObject *Sender )
{
   bool show = getShowServers();
   if ( !Application->MainForm->Visible && show )
   {
      Application->MainForm->Visible = true;
   }
   if ( Application->MainForm->Visible && !show )
   {
      Application->MainForm->Visible = false;
   }
   // check log queue; if anything on it then log to main window
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         logMessage( "Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   static bool closed = false;
   if ( !closed )
   {
      if ( checkCloseEvent() )
      {
         closed = true;
         CloseButtonClick( Sender );
      }
   }
}
//---------------------------------------------------------------------------
String mfg;
String model;
rig_model_t selected = -1;
static int print_model_list( const struct rig_caps *caps, void */*data*/ )
{
   char buff[ 1024 ];
   sprintf( buff, "%-16s%-24s", caps->mfg_name, caps->model_name );

   trace( std::string( "Rig available: " ) + buff );


   if ( String( caps->mfg_name ) == mfg && String( caps->model_name ) == model )
      selected = caps->rig_model;

   RigCtlMain->RigCombo->AddItem( buff, ( TObject * ) caps->rig_model );
   return 1;  /* !=0, we want them all ! */
}

void __fastcall TRigCtlMain::FormShow( TObject */*Sender*/ )
{
   TIniFile * t = new TIniFile( ".\\Configuration\\RigSelect.ini" );
   mfg = t->ReadString( "RigControl", "Manufacturer", "NoMfG" );
   model = t->ReadString( "RigControl", "Model", "NoMoDeL" );
   delete t;
   makeRPCObjects();
   XMPPInitialise( "RigCtl" );
   rig_load_all_backends();

   selected = -1;
   int status = rig_list_foreach( print_model_list, NULL );
   if ( status != RIG_OK )
   {
      char buff[ 1024 ];
      sprintf( buff, "rig_list_foreach: error = %s \n", rigerror( status ) );
      trace( buff );
   }
   RigCombo->Sorted = true;

   if ( selected != -1 )
   {
      RigCombo->ItemIndex = RigCombo->Items->IndexOfObject( ( TObject * ) selected );
      RigSelectButtonClick( this );
   }
   else
      RigCombo->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TRigCtlMain::CloseButtonClick( TObject *Sender )
{
   MinosRPCObj::clearRPCObjects();
   XMPPClosedown();
   LogTimerTimer( Sender );
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TRigCtlMain::FormClose( TObject */*Sender*/, TCloseAction &/*Action*/ )
{
   rig_cleanup( my_rig );
   my_rig = 0;
}
//---------------------------------------------------------------------------

void __fastcall TRigCtlMain::Timer1Timer( TObject */*Sender*/ )
{
   if ( !my_rig )
      return ;

   static in_timer = false;
   if ( in_timer )
      return ;
   in_timer = true;

   static freq_t lastFreq = 0;
   freq_t freq;
   shortfreq_t ritfreq;
   shortfreq_t xitfreq;
   static rmode_t last_mode = RIG_MODE_NONE;
   rmode_t mode;
   pbwidth_t width;

   char buff[ 1024 ];
   char fbuff[ 1024 ];

   DWORD t = GetTickCount();

   int ferr = rig_get_freq( my_rig, RIG_VFO_CURR, &freq );
   //  int rerr = rig_get_rit(my_rig, RIG_VFO_CURR, &ritfreq);
   //  int xerr = rig_get_xit(my_rig, RIG_VFO_CURR, &xitfreq);
   int merr = rig_get_mode( my_rig, RIG_VFO_CURR, &mode, &width );

   DWORD t1 = GetTickCount();

   if ( ferr != RIG_OK )
   {
      sprintf( buff, "Time %d rig_getFreq: error = %s", ( int ) ( t1 - t ), rigerror( ferr ) );
      Label2->Caption = buff;
   }
   else
   {
      freq += rigTransConversion;

      sprintf( fbuff, "%lld", ( long long ) freq );
      sprintf( buff, "rig_get_op_params Time %d Freq %s", ( int ) ( t1 - t ), fbuff );
      Label2->Caption = buff;
   }
   // and publish the value...

   if ( freq != lastFreq )
   {
      RPCPubSub::publish( "RigControl", "Frequency", fbuff, psPublished );

      lastFreq = freq;
   }

   if ( merr != RIG_OK )
   {
      sprintf( buff, "Time %d rig_get_mode: error = %s", ( int ) ( t1 - t ), rigerror( ferr ) );
      Label2->Caption = buff;
   }
   else
   {
      if ( mode != last_mode )
      {
         std::string strMode;
         switch ( mode )
         {
            case RIG_MODE_AM:
               strMode = "A3E";
               break;

            case RIG_MODE_CW:
               strMode = "A1A";
               break;

            case RIG_MODE_USB:
               strMode = "J3E";
               break;

            case RIG_MODE_LSB:
               strMode = "J3E";
               break;

            case RIG_MODE_RTTY:
               strMode = "RTTY";
               break;

            case RIG_MODE_FM:
               strMode = "F3E";
               break;

            case RIG_MODE_WFM:
               strMode = "F3E";
               break;

            case RIG_MODE_CWR:
               strMode = "A1A";
               break;

            case RIG_MODE_RTTYR:
               strMode = "RTTY";
               break;

            case RIG_MODE_AMS:
               strMode = "AMS";
               break;

            case RIG_MODE_PKTLSB:
               strMode = "PKT";
               break;

            case RIG_MODE_PKTUSB:
               strMode = "PKT";
               break;

            case RIG_MODE_PKTFM:
               strMode = "PKT";
               break;

            case RIG_MODE_ECSSUSB:
               strMode = "A3E";
               break;

            case RIG_MODE_ECSSLSB:
               strMode = "J3E";
               break;

            case RIG_MODE_FAX:
               strMode = "FAX";
               break;

            case RIG_MODE_NONE:
            default:
               strMode = "UNK";
               break;

         }
         switch ( mode )
         {
            case RIG_MODE_AM:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_CW:
               ModeRadioGroup->ItemIndex = 1;
               break;

            case RIG_MODE_USB:
               ModeRadioGroup->ItemIndex = 2;
               break;

            case RIG_MODE_LSB:
               ModeRadioGroup->ItemIndex = 2;
               break;

            case RIG_MODE_RTTY:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_FM:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_WFM:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_CWR:
               ModeRadioGroup->ItemIndex = 1;
               break;

            case RIG_MODE_RTTYR:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_AMS:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_PKTLSB:
               ModeRadioGroup->ItemIndex = 2;
               break;

            case RIG_MODE_PKTUSB:
               ModeRadioGroup->ItemIndex = 2;
               break;

            case RIG_MODE_PKTFM:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_ECSSUSB:
               ModeRadioGroup->ItemIndex = 2;
               break;

            case RIG_MODE_ECSSLSB:
               ModeRadioGroup->ItemIndex = 2;
               break;

            case RIG_MODE_FAX:
               ModeRadioGroup->ItemIndex = 0;
               break;

            case RIG_MODE_NONE:
            default:
               ModeRadioGroup->ItemIndex = 0;
               break;

         }
         last_mode = mode;

         RPCPubSub::publish( "RigControl", "Mode", strMode, psPublished );
      }
   }

   in_timer = false;
}
//---------------------------------------------------------------------------

void TRigCtlMain::publishCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "Publish callback from " + from + ( err ? ":Error" : ":Normal" ) );
}
//---------------------------------------------------------------------------


void __fastcall TRigCtlMain::ChangeButtonClick( TObject */*Sender*/ )
{
   long rig = RigBase->Text.ToIntDef( 0 );
   long trans = TransvertBase->Text.ToIntDef( 0 );

   rigTransConversion = trans - rig;
}
//---------------------------------------------------------------------------

void __fastcall TRigCtlMain::RigSelectButtonClick( TObject *Sender )
{
   if ( my_rig )
   {
      rig_cleanup( my_rig );
      my_rig = 0;
   }

   int rig_model = ( int ) RigCombo->Items->Objects[ RigCombo->ItemIndex ];
   std::string port = PortCombo->Text.t_str();
   //  	my_rig = rig_init(RIG_MODEL_FT1000MP);
   my_rig = rig_init( rig_model );
   if ( !my_rig )
   {
      Label1->Caption = "rig_init failed";
      return ;
   }
   strncpy( my_rig->state.rigport.pathname, port.c_str(), FILPATHLEN );
   int retcode = rig_open( my_rig );
   char buff[ 1024 ];
   if ( retcode != RIG_OK )
   {
      sprintf( buff, "rig_open: error = %s \n", rigerror( retcode ) );
      Label1->Caption = buff;
      my_rig = 0;
   }
   else
   {
      sprintf( buff, "Opened rig model %d, '%s'\n", my_rig->caps->rig_model,
               my_rig->caps->model_name );

      TIniFile *t = new TIniFile( ".\\Configuration\\RigSelect.ini" );
      t->WriteString( "RigControl", "Manufacturer", my_rig->caps->mfg_name );
      t->WriteString( "RigControl", "Model", my_rig->caps->model_name );
      delete t;

      Label1->Caption = buff;
      trace( std::string( "Rig selected: " ) + buff );

      ChangeButtonClick( Sender );
   }
}
//---------------------------------------------------------------------------

