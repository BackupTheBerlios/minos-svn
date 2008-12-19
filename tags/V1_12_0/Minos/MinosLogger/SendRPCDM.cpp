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

#include "ServerEvent.h"
#include "GJVThreads.h"
#include "LogEvents.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"
#include "XMPPRPCObj.h"
#include "BandMapRPCObj.h"
#include "KeyerRPCObj.h"
#include "LoggerRPCObj.h"
#include "RPCPubSub.h"

#include "SendRPCDM.h"

#include "gridhint.h"
#include "LogFrame.h"
#include "LogMain.h"
#include "ConfigDM.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TSendDM *SendDM = 0;
//---------------------------------------------------------------------------
/*static*/
void TSendDM::makeRPCObjects()
{
   RPCPubSub::initialisePubSub( new TRPCCallback <TSendDM> ( SendDM, &TSendDM::notifyCallback ) );

   MinosRPCObj::addObj( new RPCBandMapClient( new TRPCCallback <TSendDM> ( SendDM, &TSendDM::bandMapClientCallback ) ) );
   MinosRPCObj::addObj( new RPCBandMapServer( new TRPCCallback <TSendDM> ( SendDM, &TSendDM::bandMapServerCallback ) ) );
   MinosRPCObj::addObj( new RPCKeyerControlClient( new TRPCCallback <TSendDM> ( SendDM, &TSendDM::keyerCallback ) ) );

   TRPCFunctor *lccb = new TRPCCallback <TSendDM> ( SendDM, &TSendDM::loggerServerCallback );
   MinosRPCObj::addObj( new RPCLogSubscribeServer( lccb ) );

}
//---------------------------------------------------------------------------
/*static*/ void TSendDM::sendKeyerPlay(  int fno )
{
   RPCKeyerControlClient rpc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sName(new RPCStringParam( "PlayFile" ));
   boost::shared_ptr<RPCParam>iValue(new RPCIntParam( fno ));
   st->addMember( sName, "Name" );
   st->addMember( iValue, "Value" );
   rpc.getCallArgs() ->addParam( st );
   rpc.queueCall( "Keyer@localhost" );
}
/*static*/ void TSendDM::sendKeyerRecord(  int fno )
{
   RPCKeyerControlClient rpc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sName(new RPCStringParam( "RecordFile" ));
   boost::shared_ptr<RPCParam>iValue(new RPCIntParam( fno ));
   st->addMember( sName, "Name" );
   st->addMember( iValue, "Value" );
   rpc.getCallArgs() ->addParam( st );
   rpc.queueCall( "Keyer@localhost" );
}
//---------------------------------------------------------------------------
/*static*/ void TSendDM::sendBandMap(  const std::string &freq,   const std::string &call,   const std::string &utc,   const std::string &loc,   const std::string &qth )
{
   RPCBandMapClient rpc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);

   boost::shared_ptr<RPCParam>sName(new RPCStringParam( "BandMap" ));
   st->addMember( sName, "Name" );

   boost::shared_ptr<RPCParam>sValue(new RPCStringParam( freq ));
   st->addMember( sValue, "Freq" );

   boost::shared_ptr<RPCParam>sCall(new RPCStringParam( call ));
   st->addMember( sCall, "Callsign" );

   boost::shared_ptr<RPCParam>sLoc(new RPCStringParam( loc ));
   st->addMember( sLoc, "Locator" );

   boost::shared_ptr<RPCParam>dValue(new RPCDtgParam( utc ));
   st->addMember( dValue, "UTC" );

   boost::shared_ptr<RPCParam>sQTH(new RPCStringParam( qth ));
   st->addMember( sQTH, "QTH" );

   rpc.getCallArgs() ->addParam( st );
   rpc.queueCall( "BandMap@localhost" );
}
//---------------------------------------------------------------------------
__fastcall TSendDM::TSendDM( TComponent* Owner )
      : TDataModule( Owner ), connected( false ), subscribed( false )
{}
__fastcall TSendDM::~TSendDM()
{
   //   if ( connected )
   {
      MinosRPCObj::clearRPCObjects();
      LogTimerTimer( this );
      XMPPClosedown();
      // and tidy up all loose ends
      GJV_scoped_lock::ClearDown();
   }
   SendDM = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSendDM::logMessage( std::string s )
{
   trace( s );
}
//---------------------------------------------------------------------------
void __fastcall TSendDM::ConnectTimerTimer( TObject */*Sender*/ )
{
   if ( checkServerReady() )
   {
      ConnectTimer->Enabled = false;
      makeRPCObjects();
      XMPPInitialise( "Logger" );
      connected = true;
      LogTimer->Enabled = true;
   }
}
//---------------------------------------------------------------------------
void __fastcall TSendDM::LogTimerTimer( TObject */*Sender*/ )
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
   subscribeAll(); // may need to be repeated... won't do it too often, though
}
//---------------------------------------------------------------------------
void TSendDM::bandMapClientCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "BandMap client callback from " + from + ( err ? ":Error" : ":Normal" ) );
   // transfer from contact to bandmap
}
//---------------------------------------------------------------------------
void TSendDM::bandMapServerCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "BandMap server callback from " + from + ( err ? ":Error" : ":Normal" ) );
   // transfer from bandmap to new contact
}
//---------------------------------------------------------------------------
void TSendDM::keyerCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "Keyer callback from " + from + ( err ? ":Error" : ":Normal" ) );
}
//---------------------------------------------------------------------------
/*static*/void TSendDM::subscribeAll()
{
   if ( SendDM && SendDM->connected && !SendDM->subscribed )
   {
      SendDM->subscribed = true;

      RPCPubSub::subscribe( "RigControl" );

      RPCPubSub::subscribe( "Keyer" );

      RPCPubSub::subscribe( "BandMap" );
   }
}
//---------------------------------------------------------------------------
void TSendDM::notifyCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   // PubSub notifications
   logMessage( "Notify callback from " + from + ( err ? ":Error" : ":Normal" ) );
   AnalysePubSubNotify an( err, mro );

   // called whenever frequency changes
   if ( an.getOK() )
   {
      if ( an.getCategory() == "Keyer" && an.getKey() == "Report" )
      {
         LogContainer->setCaption( an.getValue().c_str() );
         logMessage( "KeyerReport " + an.getValue() );
      }
      if ( an.getCategory() == "RigControl" && an.getKey() == "Mode" )
      {
         LogContainer->setMode( an.getValue().c_str() );
         logMessage( "RigMode " + an.getValue() );
      }
      if ( an.getCategory() == "RigControl" && an.getKey() == "Frequency" )
      {
         LogContainer->setFreq( an.getValue().c_str() );
      }
      if ( an.getCategory() == "BandMap" && an.getKey() == "Loaded" )
      {
         LogContainer->setBandMapLoaded();
      }
   }

}
//---------------------------------------------------------------------------
void TSendDM::zconfCallback( bool err, MinosRPCObj */*mro*/, const std::string &from )
{
   logMessage( "RPC Response callback from " + from + ( err ? ":Error" : ":Normal" ) );
}
//---------------------------------------------------------------------------
void TSendDM::loggerServerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   // responds to pull calls from the monitoring client
   logMessage( "RPC loggerServerCallback from " + from + ( err ? ":Error" : ":Normal" ) );
   if ( !err )
   {
      boost::shared_ptr<RPCParam> psLogName;
      boost::shared_ptr<RPCParam>psStanza;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "LogName", psLogName )
           && args->getStructArgMember( 0, "Stanza", psStanza ) )
      {
         std::string LogName;
         int Stanza;
         if ( psLogName->getString( LogName ) && psStanza->getInt( Stanza ) )
         {
            mro->clearCallArgs();
            boost::shared_ptr<RPCParam>st(new RPCParamStruct);


            bool callOK = true;
            // Find Value as a published log
            // and then send requested stanza from the log
            // we publish the stanza count; it is up to the monitor to ensure
            // it has a full set
            st->addMember( LogName, "LogName" );
            st->addMember( Stanza, "Stanza" );
            if ( callOK )
            {
               // we need to start pushing stanzas from the logfile - we can
               // only really process a log sequentially
               // Once it has all been pushed then later stanzas need
               // to go to all subscribers.

               // SO it is more in the nature of a "subscribe" but no
               // get the stanza data from the log and add it as a string
               TSingleLogFrame * lf = LogContainer ->findContest( LogName );
               if ( lf )
               {
                  std::string StanzaData;
                  callOK = lf->getStanza( Stanza, StanzaData );
                  if ( callOK )
                  {
                     st->addMember( StanzaData, "StanzaData" );
                  }
               }
               else
               {
                  callOK = false;
               }
            }

            st->addMember( callOK, "LoggerResult" );
            mro->getCallArgs() ->addParam( st );
            mro->queueResponse( from );
         }
      }
   }
}
//---------------------------------------------------------------------------

