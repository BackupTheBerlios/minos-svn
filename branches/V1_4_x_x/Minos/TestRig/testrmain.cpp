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
#include <stdio.h>
#include <inifiles.hpp> 
//---------------------------------------------------------------------------
#include "hamlib\rig.h"
#include "testrmain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "xTrackBar"
#pragma resource "*.dfm"
TTestRigMain *TestRigMain = 0;
//---------------------------------------------------------------------------
static HANDLE MinosCloseEvent;
void createCloseEvent()
{
   if ( !MinosCloseEvent )
      MinosCloseEvent = CreateEvent( NULL, TRUE, FALSE, "MinosClosedownEvent" ); // manual reset, initially false
}
//---------------------------------------------------------------------------
void signalCloseEvent()
{
   createCloseEvent();
   SetEvent( MinosCloseEvent );
}
//---------------------------------------------------------------------------
bool checkCloseEvent()
{
   createCloseEvent();
   return ( WaitForSingleObject( MinosCloseEvent , 0 ) == WAIT_OBJECT_0 );
}
//---------------------------------------------------------------------------
__fastcall TTestRigMain::TTestRigMain( TComponent* Owner )
      : TForm( Owner ), my_rig( 0 ), noSetFreq( false ), noSetMode( false ),
      saveResize( false )
{}
//---------------------------------------------------------------------------
String mfg;
String model;
rig_model_t selected = -1;

static int print_model_list( const struct rig_caps *caps, void */*data*/ )
{
   char buff[ 1024 ];
   sprintf( buff, "%-16s%-24s", caps->mfg_name, caps->model_name );

   if ( String( caps->mfg_name ) == mfg && String( caps->model_name ) == model )
      selected = caps->rig_model;

   TestRigMain->RigCombo->AddItem( buff, ( TObject * ) caps->rig_model );
   return 1;  /* !=0, we want them all ! */
}

void __fastcall TTestRigMain::FormShow( TObject */*Sender*/ )
{
   StartupTimer->Enabled = true;
   TIniFile * t = new TIniFile( ".\\Configuration\\RigSelect.ini" );
   mfg = t->ReadString( "TestRig", "Manufacturer", "NoMfG" );
   model = t->ReadString( "TestRig", "Model", "NoMoDeL" );
   delete t;
   rig_load_all_backends();

   selected = -1;
   int status = rig_list_foreach( print_model_list, NULL );
   if ( status != RIG_OK )
   {
      return ;
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
void __fastcall TTestRigMain::CloseButtonClick( TObject */*Sender*/ )
{
   // and tidy up all loose ends
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TTestRigMain::FormClose( TObject */*Sender*/, TCloseAction &/*Action*/ )
{
   saveResize = false;
   rig_cleanup( my_rig );
   my_rig = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTestRigMain::Timer1Timer( TObject *Sender )
{
   static in_timer = false;
   if ( in_timer )
      return ;

   static bool closed = false;
   if ( !closed )
   {
      if ( checkCloseEvent() )
      {
         closed = true;
         CloseButtonClick( Sender );
      }
   }

   in_timer = true;

   freq_t freq;
   shortfreq_t ritfreq;
   shortfreq_t xitfreq;
   rmode_t mode;
   pbwidth_t width;

   char buff[ 1024 ];

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
      sprintf( buff, "rig_getFreq Time %d Freq %lld", ( int ) ( t1 - t ), ( long long ) freq );
      Label2->Caption = buff;

      noSetFreq = true;
      FreqSlider->Position = ( freq - 28000000 ) / 100;
      noSetFreq = false;
   }
   if ( merr != RIG_OK )
   {
      sprintf( buff, "Time %d rig_get_mode: error = %s", ( int ) ( t1 - t ), rigerror( ferr ) );
      Label2->Caption = buff;
   }
   else
   {
      sprintf( buff, "rig_get_mode Time %d mode %d", ( int ) ( t1 - t ), ( long ) mode );
      Label2->Caption = buff;

      noSetMode = true;
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
      noSetMode = false;
   }

   in_timer = false;
}
//---------------------------------------------------------------------------

void __fastcall TTestRigMain::FreqSliderChange( TObject */*Sender*/ )
{
   if ( !noSetFreq )
   {
      int pos = FreqSlider->Position;
      freq_t freq = 28000000;

      freq += pos * 100;
      int ferr = rig_set_freq( my_rig, RIG_VFO_CURR, freq );
      if ( ferr != RIG_OK )
      {
         char buff[ 1024 ];
         sprintf( buff, "rig_setFreq: error = %s", rigerror( ferr ) );
         Label2->Caption = buff;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TTestRigMain::RigSelectButtonClick( TObject */*Sender*/ )
{
   if ( my_rig )
   {
      rig_cleanup( my_rig );
      my_rig = 0;
   }

   int rig_model = ( int ) RigCombo->Items->Objects[ RigCombo->ItemIndex ];
   String port = PortCombo->Text;

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
      sprintf( buff, "Opened rig model %d, %s '%s'\n", my_rig->caps->rig_model,
               my_rig->caps->mfg_name, my_rig->caps->model_name );

      TIniFile *t = new TIniFile( ".\\Configuration\\RigSelect.ini" );
      t->WriteString( "TestRig", "Manufacturer", my_rig->caps->mfg_name );
      t->WriteString( "TestRig", "Model", my_rig->caps->model_name );
      delete t;

      Label1->Caption = buff;
   }

}
//---------------------------------------------------------------------------

void __fastcall TTestRigMain::ModeRadioGroupClick( TObject */*Sender*/ )
{
   if ( !noSetMode )
   {
      rmode_t mode;
      if ( ModeRadioGroup->ItemIndex == 0 )
         mode = RIG_MODE_NONE;
      if ( ModeRadioGroup->ItemIndex == 1 )
         mode = RIG_MODE_CW;
      else
         if ( ModeRadioGroup->ItemIndex == 2 )
            mode = RIG_MODE_USB;
         else
            mode = RIG_MODE_NONE;

      int ferr = rig_set_mode( my_rig, RIG_VFO_CURR, mode, RIG_FREQ_NONE );
      if ( ferr != RIG_OK )
      {
         char buff[ 1024 ];
         sprintf( buff, "rig_set_mode: error = %s", rigerror( ferr ) );
         Label2->Caption = buff;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TTestRigMain::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;

   ForceDirectories( ".\\Configuration" );
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\TestRig.ini" ) );

   int L = Left, T = Top, W = Width, H = Height;
   L = iniFile->ReadInteger( "Display", "Left", L );
   T = iniFile->ReadInteger( "Display", "Top", T );
   W = iniFile->ReadInteger( "Display", "Width", W );
   H = iniFile->ReadInteger( "Display", "Height", H );
   SetBounds( L, T, W, H );
   Repaint();
   saveResize = true;
}
//---------------------------------------------------------------------------

void __fastcall TTestRigMain::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\TestRig.ini" ) );
      iniFile->WriteInteger( "Display", "Left", Left );
      iniFile->WriteInteger( "Display", "Top", Top );
      iniFile->WriteInteger( "Display", "Width", Width );
      iniFile->WriteInteger( "Display", "Height", Height );
      iniFile->UpdateFile();
   }
}
void __fastcall TTestRigMain::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

