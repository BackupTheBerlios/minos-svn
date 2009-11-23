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
#include <inifiles.hpp>
#include <boost\shared_ptr.hpp>

#include "GJVThreads.h"
#include "LogEvents.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"
#include "XMPPRPCObj.h"
#include "RPCPubSub.h"
#include "ControlRPCObj.h"

#include "MTestMain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TClientMain *ClientMain = 0;
//---------------------------------------------------------------------------
/*static*/
void makeRPCObjects()
{
   RPCPubSub::initialisePubSub( new TRPCCallback <TClientMain> ( ClientMain, &TClientMain::notifyCallback ) );

   MinosRPCObj::addObj( new RPCControlClient( new TRPCCallback <TClientMain> ( ClientMain, &TClientMain::controlClientCallback ) ) );
   MinosRPCObj::addObj( new RPCControlServer( new TRPCCallback <TClientMain> ( ClientMain, &TClientMain::controlServerCallback ) ) );
}
//---------------------------------------------------------------------------
void __fastcall TClientMain::logMessage( std::string s )
{
   String w = s.c_str();
   LogMemo->Lines->Add( w );
}
//---------------------------------------------------------------------------
__fastcall TClientMain::TClientMain( TComponent* Owner )
      : TForm( Owner ), subscribed( false ), saveResize( false )
{
   FromUser->Text = "mjg";
}
//---------------------------------------------------------------------------
void __fastcall TClientMain::CloseButtonClick( TObject */*Sender*/ )
{
   Close();                         // and finally close

}
//---------------------------------------------------------------------------
void __fastcall TClientMain::FormClose( TObject *Sender,
                                        TCloseAction &/*Action*/ )
{
   saveResize = false;
   MinosRPCObj::clearRPCObjects();
   XMPPClosedown();
   LogTimerTimer( Sender );
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------
void __fastcall TClientMain::SendButtonClick( TObject */*Sender*/ )
{
   static bool PTTState = false;
   {
      RPCControlClient rpc( 0 );
      boost::shared_ptr<RPCParam>st(new RPCParamStruct);
      boost::shared_ptr<RPCParam>sName(new RPCStringParam( "GetLine" ));
      boost::shared_ptr<RPCParam>sValue(new RPCStringParam( "PTTIn" ));
      st->addMember( sName, "Name" );
      st->addMember( sValue, "Value" );
      rpc.getCallArgs() ->addParam( st );
      rpc.queueCall( "control@localhost" );
   }
   {
      RPCControlClient rpc( 0 );
      boost::shared_ptr<RPCParam>st(new RPCParamStruct);
      boost::shared_ptr<RPCParam>sName(new RPCStringParam( "GetLine" ));
      boost::shared_ptr<RPCParam>sValue(new RPCStringParam( "l1" ));
      st->addMember( sName, "Name" );
      st->addMember( sValue, "Value" );
      rpc.getCallArgs() ->addParam( st );
      rpc.queueCall( "control@localhost" );
   }
   {
      RPCControlClient rpc( 0 );
      boost::shared_ptr<RPCParam>st(new RPCParamStruct);
      boost::shared_ptr<RPCParam>sName(new RPCStringParam( "GetLine" ));
      boost::shared_ptr<RPCParam>sValue(new RPCStringParam( "L2" ));
      st->addMember( sName, "Name" );
      st->addMember( sValue, "Value" );
      rpc.getCallArgs() ->addParam( st );
      rpc.queueCall( "control@localhost" );
   }
   {
      RPCControlClient rpc( 0 );
      boost::shared_ptr<RPCParam>st(new RPCParamStruct);
      boost::shared_ptr<RPCParam>sName(new RPCStringParam( "SetLine" ));
      boost::shared_ptr<RPCParam>sValue(new RPCStringParam( "PTTOut" ));
      st->addMember( sName, "Name" );
      st->addMember( sValue, "Value" );

      PTTState = !PTTState;
      boost::shared_ptr<RPCParam>sState(new RPCBooleanParam( PTTState ));
      st->addMember( sState, "State" );

      rpc.getCallArgs() ->addParam( st );
      rpc.queueCall( "control@localhost" );
   }
}
//---------------------------------------------------------------------------

void __fastcall TClientMain::LogTimerTimer( TObject *Sender )
{
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

void __fastcall TClientMain::SubscribeButtonClick( TObject */*Sender*/ )
{
   // Subscribe to rig frequency
   subscribed = true;
   RPCPubSub::subscribe( "RigControl" );
   RPCPubSub::subscribe( "LineControl" );
   RPCPubSub::subscribeRemote( "dev-Station", "LineControl" );
}
//---------------------------------------------------------------------------
void TClientMain::notifyCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "Notify callback from " + from + ( err ? ":Error" : ":Normal" ) );
}
//---------------------------------------------------------------------------

void __fastcall TClientMain::FormShow( TObject */*Sender*/ )
{
   StartupTimer->Enabled = true;
   XMPPInitialise( FromUser->Text.c_str() );
}
//---------------------------------------------------------------------------
void TClientMain::controlClientCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   logMessage( "control client callback from " + from + ( err ? ":Error" : ":Normal" ) );
   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      boost::shared_ptr<RPCParam> psValue;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) && args->getStructArgMember( 0, "Value", psValue ) )
      {
         std::string Name;
         std::string Value;

         if ( psName->getString( Name ) && psValue->getString( Value ) )
         {
            logMessage( "Name " + Name + " value " + Value );
         }
      }
   }

}
void TClientMain::controlServerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   logMessage( "control server callback from " + from + ( err ? ":Error" : ":Normal" ) );
   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      boost::shared_ptr<RPCParam> psValue;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) && args->getStructArgMember( 0, "Value", psValue ) )
      {
         std::string Name;
         std::string Value;

         if ( psName->getString( Name ) && psValue->getString( Value ) )
         {
            logMessage( "Name " + Name + " value " + Value );
         }
      }
   }

}

void __fastcall TClientMain::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;

   ForceDirectories( ".\\Configuration" );
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosTestClient.ini" ) );

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

void __fastcall TClientMain::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosTestClient.ini" ) );
      iniFile->WriteInteger( "Display", "Left", Left );
      iniFile->WriteInteger( "Display", "Top", Top );
      iniFile->WriteInteger( "Display", "Width", Width );
      iniFile->WriteInteger( "Display", "Height", Height );
      iniFile->UpdateFile();
   }
}
void __fastcall TClientMain::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

