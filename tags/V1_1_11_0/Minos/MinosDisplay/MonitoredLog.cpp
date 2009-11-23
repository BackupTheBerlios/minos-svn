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

#include "display_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------

#pragma package(smart_init)

MonitoredLog::MonitoredLog() : contest( 0 ), expectedStanzaCount( 0 ), monitorEnabled( false ),
      inStanzaRequest( false ), frame( 0 ), lastScannedStanza( -1 ), mt( 0 )
{}
MonitoredLog::~MonitoredLog()
{
   mt->endImportTest();
   delete mt;
   delete contest;
}
void MonitoredLog::initialise( const std::string &srv, const std::string &name )
{
   publishedName = name;
   server = srv;

   delete contest;
   contest = new MonitoredContestLog();
   delete mt;
   mt = new MinosTestImport( contest );
   mt->startImportTest();
}

void MonitoredLog::startMonitor()
{
   monitorEnabled = true;
   lastScannedStanza = -1;
}
void MonitoredLog::stopMonitor()
{
   monitorEnabled = false;

   mt->endImportTest();

   inStanzaRequest = false;
   lastScannedStanza = -1;
   //   expectedStanzaCount = 0; // no - it is still correct as published...
   stanzasPulled.clear();

   frame = 0;

   initialise( server, publishedName );  // make sure reset for next time
}
void MonitoredLog::getLogStanza( int stanza )
{
   inStanzaRequest = ::GetTickCount();
   // and here we want to start getting the log from the remote logger
   RPCLogSubscribeClient rpc( 0 );
   boost::shared_ptr<RPCParam>st(new RPCParamStruct);
   boost::shared_ptr<RPCParam>sName(new RPCStringParam( publishedName ));
   boost::shared_ptr<RPCParam>iStanza(new RPCIntParam( stanza ));
   st->addMember( sName, "LogName" );
   st->addMember( iStanza, "Stanza" );
   rpc.getCallArgs() ->addParam( st );
   rpc.queueCall( std::string( "Logger@" ) + server );

}
void MonitoredLog::checkMonitor()
{
   // here we need to check that we haven't got any gaps in the received log
   // and re-request the lot as necessary
   if ( !contest || !frame )
   {
      return ;
   }
   int curCount = contest->getStanzaCount();
   if ( monitorEnabled && ( inStanzaRequest == 0 || ( ::GetTickCount() - inStanzaRequest > 10000 ) ) )
   {
      if ( expectedStanzaCount > curCount )
      {
         getLogStanza( curCount + 1 );
      }
   }
   // this is off a timer, so it is safe to access VCL from here
   if ( frame->LogMonitorFrame->QSOTree->RootNodeCount != ( unsigned int ) contest->getContactCount() )
   {
      PVirtualNode f = frame->LogMonitorFrame->QSOTree->FocusedNode;
      PVirtualNode l = frame->LogMonitorFrame->QSOTree->GetLastChild( frame->LogMonitorFrame->QSOTree->RootNode );

      frame->LogMonitorFrame->QSOTree->RootNodeCount = contest->getContactCount();  // The magic of virtual displays...
      frame->LogMonitorFrame->QSOTree->Invalidate();

      if ( l == f )
      {
         l = frame->LogMonitorFrame->QSOTree->GetLastChild( frame->LogMonitorFrame->QSOTree->RootNode );
         frame->LogMonitorFrame->QSOTree->FocusedNode = l;
         frame->LogMonitorFrame->QSOTree->Selected[ l ] = true;
      }
   }
   if ( expectedStanzaCount == curCount && lastScannedStanza != curCount )
   {
      contest->startScan();
      frame->LogMonitorFrame->showQSOs();
      frame->MultDispFrame->refreshMults();
      frame->LogMonitorFrame->Repaint();

      lastScannedStanza = curCount;

   }
}
void MonitoredLog::processLogStanza( int stanza, const std::string &stanzaData )
{
   // This is in a thread, so it isn't  VCL safe...
   inStanzaRequest = 0;
   if ( stanzasPulled.find( stanza ) == stanzasPulled.end() )
   {
      // we have a stanza - so pass it into the contest object
      contest->stanzaCount = mt->importTestBuffer( stanzaData );
   }
}

