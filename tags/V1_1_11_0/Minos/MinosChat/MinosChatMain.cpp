//---------------------------------------------------------------------------
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
#include <algorithm>
#include <deque>
#include <string>
#include <vector>
#include <inifiles.hpp>
#include <boost\shared_ptr.hpp>

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
#include "mtrace.h"
#include "ChatRPCObj.h"

#include "MinosChatMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvExExtCtrls"
#pragma link "JvNetscapeSplitter"
#pragma resource "*.dfm"
TMinosChatForm *MinosChatForm = 0;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*static*/
void TMinosChatForm::makeRPCObjects()
{
   RPCPubSub::initialisePubSub( new TRPCCallback <TMinosChatForm> ( MinosChatForm, &TMinosChatForm::notifyCallback ) );
   MinosRPCObj::addObj( new RPCChatControlClient( new TRPCCallback <TMinosChatForm> ( MinosChatForm, &TMinosChatForm::chatClientCallback ) ) );
   MinosRPCObj::addObj( new RPCChatControlServer( new TRPCCallback <TMinosChatForm> ( MinosChatForm, &TMinosChatForm::chatServerCallback ) ) );
}
//---------------------------------------------------------------------------
void __fastcall TMinosChatForm::logMessage( std::string s )
{
   trace( s );
}
//---------------------------------------------------------------------------
__fastcall TMinosChatForm::TMinosChatForm( TComponent* Owner )
      : TForm( Owner ), subscribed( false ), saveResize( false )
{
   enableTrace( ".\\TraceLog\\MinosChat_" );
}
//---------------------------------------------------------------------------
void __fastcall TMinosChatForm::FormClose( TObject *Sender,
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

void __fastcall TMinosChatForm::LogTimerTimer( TObject */*Sender*/ )
{
   syncStations();
   syncChat();
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
         Close();
      }
   }
}
//---------------------------------------------------------------------------

bool syncstat = false;
std::vector<std::string> chatQueue;
char *stateIndicator[] =
{
   "Av",
   "NA",
   "NC"
};
void TMinosChatForm::syncStations()
{
   CsGuard g;
   if ( syncstat )
   {
      syncstat = false;
      StationList->Items->Clear();
      for ( std::vector<Server>::iterator i = serverList.begin(); i != serverList.end(); i++ )
      {
         // should we link an object with the state, and owner draw
         StationList->Items->Add( ( std::string(stateIndicator[(*i).state]) + " " + (*i).name).c_str() );
      }
   }
}
void TMinosChatForm::addChat(const std::string &mess)
{
   TDateTime dt = TDateTime::CurrentDateTime();
   unsigned short h, m, s, ms;
   dt.DecodeTime( &h, &m, &s, &ms );
   String sdt = dt.FormatString( "hh:nn:ss " ) + mess.c_str();
   chatQueue.push_back(sdt.c_str());
}
void TMinosChatForm::syncChat()
{
   CsGuard g;
   for ( std::vector<std::string>::iterator i = chatQueue.begin(); i != chatQueue.end(); i++ )
   {
      ChatMemo->Lines->Add( ( *i ).c_str() );
   }
   chatQueue.clear();
}
//---------------------------------------------------------------------------
char *stateList[] =
{
   "Available",
   "Not Available",
   "No Contact"
};
void TMinosChatForm::notifyCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   logMessage( "Notify callback from " + from + ( err ? ":Error" : ":Normal" ) );
   AnalysePubSubNotify an( err, mro );

   if ( an.getOK() )
   {
      if ( an.getCategory() == "Station" )
      {
         CsGuard g;
         std::string key = an.getKey();
         std::string value = an.getValue();
         PublishState state = an.getState();
         logMessage( std::string(stateIndicator[state]) + " " + key + " " + value );
         std::vector<Server>::iterator stat;
         for ( stat = serverList.begin(); stat != serverList.end(); stat++ )
         {
            if ((*stat).name == key)
            {
               if ((*stat).state != state)
               {
                  (*stat).state = state;
                  std:: string mess = key + " changed state to " + std::string(stateList[state]);
                  addChat( mess );
                  syncstat = true;
               }
               break;
            }
         }

         if ( stat == serverList.end() )
         {
            // We have received notification from a previously unknown station - so report on it
            Server s;
            s.name = key;
            s.ip = value;
            s.state = state;
            serverList.push_back( s );
            std:: string mess = key + " changed state to " + std::string(stateList[state]);
            addChat( mess );
            syncstat = true;
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMinosChatForm::ConnectTimerTimer( TObject */*Sender*/ )
{
   if ( !connected && checkServerReady() )
   {
      makeRPCObjects();
      XMPPInitialise( "Chat" );
      connected = true;
      RPCPubSub::subscribe( "Station" );
   }
}
//---------------------------------------------------------------------------

void __fastcall TMinosChatForm::SendButtonClick( TObject */*Sender*/ )
{
   // We need to send the message to all connected stations
   for ( std::vector<Server>::iterator i = serverList.begin(); i != serverList.end(); i++ )
   {
      RPCChatControlClient rpc( 0 );
      boost::shared_ptr<RPCParam>st(new RPCParamStruct);
      boost::shared_ptr<RPCParam>sName(new RPCStringParam( "SendChatMessage" ));
      boost::shared_ptr<RPCParam>sValue(new RPCStringParam( ChatEdit->Text.c_str() ));
      st->addMember( sName, "Name" );
      st->addMember( sValue, "Value" );
      rpc.getCallArgs() ->addParam( st );
      rpc.queueCall( "chat@" + ( (*i).name ) );
   }
   ChatEdit->Text = ""; // otherwise it is a pain!
   ActiveControl = ChatEdit;
}
//---------------------------------------------------------------------------
void TMinosChatForm::chatClientCallback( bool /*err*/, MinosRPCObj */*mro*/, const std::string &/*from*/ )
{
   // call back says OK/not OK
}
//---------------------------------------------------------------------------
void TMinosChatForm::chatServerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "chat callback from " + from + ( err ? ":Error" : ":Normal" ) );

   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      boost::shared_ptr<RPCParam>psValue;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) && args->getStructArgMember( 0, "Value", psValue ) )
      {
         std::string Name;
         std::string Value;

         if ( psName->getString( Name ) && psValue->getString( Value ) )
         {
            boost::shared_ptr<RPCParam>st(new RPCParamStruct);

            if ( Name == "SendChatMessage" )
            {
               CsGuard g;
               // add to chat window
               std:: string mess = from + " : " + Value;
               addChat( mess );
            }
            st->addMember( true, "ChatResult" );
            mro->clearCallArgs();
            mro->getCallArgs() ->addParam( st );
            mro->queueResponse( from );
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMinosChatForm::StartUpTimerTimer( TObject */*Sender*/ )
{
   StartUpTimer->Enabled = false;

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

void __fastcall TMinosChatForm::FormShow( TObject */*Sender*/ )
{
   StartUpTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMinosChatForm::FormResize( TObject */*Sender*/ )
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
void __fastcall TMinosChatForm::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

