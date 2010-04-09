//---------------------------------------------------------------------------

#include "XMPPClient_pch.h"
#pragma hdrstop
#include <inifiles.hpp>

#include <ice\ice.h>
#include "radiointerface.h"

#include "SecondRXRPCObj.h"
#include "IceProxyMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TQRigSyncMain *QRigSyncMain;
//---------------------------------------------------------------------------
void TQRigSyncMain::makeRPCObjects()
{
   RPCPubSub::initialisePubSub( 0 );   // we don't subscribe, so no callback needed
   // we need to subscribe to SecondRXMute
   MinosRPCObj::addObj( new RPCSecondRXClient( new TRPCCallback <TQRigSyncMain> ( QRigSyncMain, &TQRigSyncMain::secondRXClientCallback ) ) );
   MinosRPCObj::addObj( new RPCSecondRXServer( new TRPCCallback <TQRigSyncMain> ( QRigSyncMain, &TQRigSyncMain::secondRXServerCallback ) ) );
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::logMessage( std::string s )
{
   trace( s );
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::LogTimerTimer( TObject *Sender )
{
   bool show = getShowServers();
   if (Application->MainForm)
   {
      if ( !Application->MainForm->Visible && show )
      {
         Application->MainForm->Visible = true;
      }
      if ( Application->MainForm->Visible && !show )
      {
         Application->MainForm->Visible = false;
      }
   }
   else
   {
      if ( !Visible && show )
      {
         Visible = true;
      }
      if ( Visible && !show )
      {
         Visible = false;
      }
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
__fastcall TQRigSyncMain::TQRigSyncMain(TComponent* Owner)
   : TForm(Owner), muted(false)
{
   enableTrace( ".\\TraceLog\\QS1RProxy_" );
   try
   {
      int args = 0;
      ic = Ice::initialize(args, _argv);
      Ice::ObjectPrx base = ic->stringToProxy(
            "RadioCommander:default -t 2000 -h opti -p 43065");
      qs1r = QS1R::RadioCommanderPrx::checkedCast(base);
      if (!qs1r)
         throw "Invalid proxy";
      Timer1->Enabled = true;
   }
   catch (const Ice::Exception& ex)
   {
      ShowMessage(ex.what());
   }
   catch (const char* msg)
   {
      ShowMessage(msg);
   }
}
__fastcall TQRigSyncMain::~TQRigSyncMain()
{
   if (ic)
      ic->destroy();
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::CloseButtonClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::Timer1Timer(TObject *Sender)
{
   static bool inTimer = false;
   if (inTimer)
   {
      return;
   }
   inTimer = true;
   try
   {
      float freq = qs1r->getFrequency(0);
      Label1->Caption = String(freq);
      static long lastFreq = -1;
      long lfreq = (long)freq;
      if ( lfreq != lastFreq )
      {
         RPCPubSub::publish( "SecondRX", "Frequency", makeStr(lfreq), psPublished );

         lastFreq = lfreq;
      }
   }
   catch (const Ice::Exception& ex)
   {
      ShowMessage(ex.what());
   }
   catch (const char* msg)
   {
      ShowMessage(msg);
   }
   inTimer = false;
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::MuteButtonClick(TObject *Sender)
{
   static bool inTimer = false;
   if (inTimer)
   {
      return;
   }
   inTimer = true;
   try
   {
      qs1r->setMute(0, !muted);
      muted = !muted;
      MuteButton->Caption = muted?String("UnMute"):String("Mute");
   }
   catch (const Ice::Exception& ex)
   {
      ShowMessage(ex.what());
   }
   catch (const char* msg)
   {
      ShowMessage(msg);
   }
   inTimer = false;
}
//---------------------------------------------------------------------------

void TQRigSyncMain::publishCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "Publish callback from " + from + ( err ? ":Error" : ":Normal" ) );
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::FormShow(TObject *Sender)
{
   makeRPCObjects();
   XMPPInitialise( "RigCtl" );
   StartUpTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosChat.ini" ) );
      iniFile->WriteInteger( "Display", "Left", Left );
      iniFile->WriteInteger( "Display", "Top", Top );
      iniFile->WriteInteger( "Display", "Width", Width );
      iniFile->WriteInteger( "Display", "Height", Height );
      iniFile->UpdateFile();
   }
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   saveResize = false;
   MinosRPCObj::clearRPCObjects();
   XMPPClosedown();
   LogTimerTimer( Sender );
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::StartUpTimerTimer(TObject *Sender)
{

   ForceDirectories( ".\\Configuration" );
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosChat.ini" ) );

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
void TQRigSyncMain::secondRXClientCallback( bool /*err*/, MinosRPCObj */*mro*/, const std::string &/*from*/ )
{
   // call back says OK/not OK
}
//---------------------------------------------------------------------------
void TQRigSyncMain::secondRXServerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "SecondRX callback from " + from + ( err ? ":Error" : ":Normal" ) );
   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      boost::shared_ptr<RPCParam>psValue;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) && args->getStructArgMember( 0, "Value", psValue ) )
      {
         std::string Name;
         bool Value;

         if ( psName->getString( Name ) && psValue->getBoolean( Value ) )
         {
            boost::shared_ptr<RPCParam>st(new RPCParamStruct);

            if ( Name == "Mute" )
            {
               try
               {
                  qs1r->setMute(0, Value);
                  muted = Value;
                  MuteButton->Caption = muted?String("UnMute"):String("Mute");
               }
               catch (const Ice::Exception& ex)
               {
                  ShowMessage(ex.what());
               }
               catch (const char* msg)
               {
                  ShowMessage(msg);
               }
            }
            st->addMember( true, "MuteResult" );
            mro->clearCallArgs();
            mro->getCallArgs() ->addParam( st );
            mro->queueResponse( from );
         }
      }
   }
}
//---------------------------------------------------------------------------

