/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "minos_pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMinosMainForm *MinosMainForm;
//---------------------------------------------------------------------------
bool closeApp = false;
//---------------------------------------------------------------------------
__fastcall TMinosMainForm::TMinosMainForm( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::logMessage( std::string s )
{

   trace( s );

}
//---------------------------------------------------------------------------
extern int GetSubscribedCount();
extern int GetPublishedCount();
void __fastcall TMinosMainForm::LogTimerTimer( TObject *Sender )
{

   static int lastServerCount = 0;
   static int lastClientCount = 0;
   static int lastSubCount = 0;
   static int lastPubCount = 0;

   MinosServerListener *msl = MinosServerListener::getListener();
   MinosClientListener *mcl = MinosClientListener::getListener();
   int serverCount = msl ? msl->getConnectionCount() : 0;
   int clientCount = mcl ? mcl->getConnectionCount() : 0;
   int pubCount = GetPublishedCount();
   int subCount = GetSubscribedCount();

   if ( lastServerCount != serverCount )
   {
      lastServerCount = serverCount;
      ServerLabel->Caption = serverCount;
   }
   if ( lastClientCount != clientCount )
   {
      lastClientCount = clientCount;
      ClientLabel->Caption = clientCount;
   }

   if ( lastSubCount != subCount )
   {
      lastSubCount = subCount;
      SubLabel->Caption = subCount;
   }
   if ( lastPubCount != pubCount )
   {
      lastPubCount = pubCount;
      PubLabel->Caption = pubCount;
   }

   bool show = getShowServers();
   if ( !Application->MainForm->Visible && show )
   {
      logMessage( "Log : Showing main form" );
      Application->MainForm->Visible = true;
   }
   if ( Application->MainForm->Visible && !show )
   {
      logMessage( "Log : Hiding main form" );
      Application->MainForm->Visible = false;
   }

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
   if ( closeApp )
      Close();

   static bool closed = false;
   if ( !closed )
   {
      if ( checkCloseEvent() )
      {
         closeApp = true;
         closed = true;
         CloseButtonClick( Sender );
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::FormShow( TObject * /*Sender*/ )
{
   enableTrace( ".\\TraceLog\\MinosServer_" );
   MinosServer::getMinosServer();
   clientThread = new GJV_thread( "client", &runClientThread, ( void * ) 0 );
   serverThread = new GJV_thread( "server", &runServerThread, ( void * ) 0 );
   PubSubMain = new TPubSubMain();
   ZConf = new TZConf();
   ZConf->setName( MinosServer::getMinosServer() ->getServerName() );
}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::FormClose( TObject * /*Sender*/,
      TCloseAction & /*Action*/ )
{
   delete ZConf;
   ZConf = 0;
   delete PubSubMain;
   PubSubMain = 0;
   // we might need to farm this out!
   logMessage( "(end1)" );
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         logMessage( "(end1)Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   if ( clientThread )
   {
      clientThread->GJV_join();
      delete clientThread;
      clientThread = 0;
   }
   logMessage( "(end2)" );
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         logMessage( "(end2)Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   if ( serverThread )
   {
      serverThread->GJV_join();
      delete serverThread;
      serverThread = 0;
   }
   logMessage( "(end3)" );
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         logMessage( "(end3)Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   delete MinosServer::getMinosServer();

   logMessage( "(end4)" );
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         logMessage( "(end4)Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   logMessage( "(end5)" );

   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::CloseButtonClick( TObject *Sender )
{
   logMessage("Server close requested");
   closeApp = true;
   LogTimerTimer( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::Exit1Click( TObject * /*Sender*/ )
{
   closeApp = true;
}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::ApplicationEvents1Exception(
   TObject * /*Sender*/, Exception *E )
{
   // Trap any application level exceptions; log and continue
   logMessage( ( String( "Application Event Exception : " ) + E->Message ).c_str() );
}
//---------------------------------------------------------------------------
void __fastcall TMinosMainForm::ScanTimerTimer( TObject * /*Sender*/ )
{
   // default is every 20 secs
   CsGuard g;
   // scan the server list, and try to connect to all that we know of and we don't
   // have a server connection for
   ZConf->ServerScan();

}
//---------------------------------------------------------------------------

