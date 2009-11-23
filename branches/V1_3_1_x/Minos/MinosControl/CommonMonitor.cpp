/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop

#include <process.h>
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
#include "ControlRPCObj.h"

#include "tinyxml.h"
#include "portconf.h"
#include "controlport.h"
#ifndef _STDEXCEPT_
 #include <stdexcept>
#endif

#include "CommonMonitor.h"
#include "lineEvents.h"

//==============================================================================

// static members
baseTimer *baseTimer::tt = 0;
int baseTimer::ttcount = 0;
HANDLE baseTimer::hTimerThread = 0;
HANDLE baseTimer::hTimerThreadCloseEvent = 0;
unsigned long currTick = 0;
unsigned long basetick = 0;

void baseTimer::timerThread()
{
   GJV_thread::setThreadName( "hires timer" );

   currTick = 0;

   basetick = GetTickCount();


   TIMECAPS tc;

   if ( timeGetDevCaps( &tc, sizeof( TIMECAPS ) ) != TIMERR_NOERROR )
   {
      // Error; application can't continue.
      MessageBox( 0, "Timer", "GetDevCaps", MB_ICONERROR | MB_OK );
   }

   UINT wTimerRes = std::min( std::max( tc.wPeriodMin, TARGET_RESOLUTION ), tc.wPeriodMax );
   timeBeginPeriod( wTimerRes );
   MMRESULT timeEventId = timeSetEvent( TIMER_INTERVAL, 0, intrTick, 0, TIME_PERIODIC );

   hTimerThreadCloseEvent = CreateEvent(
                               NULL,     // no security attributes
                               TRUE,     // manual reset event
                               FALSE,      // not-signalled
                               NULL );  // no name

   while ( 1 )
   {
      // Wait for request to close or for a read thread to terminate.
      DWORD dwWait = WaitForSingleObject( hTimerThreadCloseEvent, 1000 );

      if ( dwWait == WAIT_TIMEOUT )
         continue;

      CloseHandle( hTimerThreadCloseEvent );
      if ( timeEventId )
      {
         timeKillEvent( timeEventId );
         timeEventId = 0;
      }

      if ( wTimerRes )
      {
         timeEndPeriod( wTimerRes );
         wTimerRes = 0;
      }
      return ;
   }

}
/*static */unsigned __stdcall baseTimer::timerThread(    LPVOID lpThreadParameter )
{
   baseTimer * b = ( baseTimer * ) lpThreadParameter;
   b->timerThread();

   return 0;
}


void baseTimer::tickEvent()
{
   // need to go through the keyer chain
   for ( std::deque <timerTicker *>::iterator tt = tickers.begin(); tt != tickers.end(); tt++ )
   {
      if ( ( *tt ) ->ready )
         ( *tt ) ->tickEvent();
   }
}
void baseTimer::registerTicker( timerTicker *k )
{
   tickers.push_back( k );
}

void CALLBACK baseTimer::intrTick( UINT /*wTimerID*/, UINT /*msg*/,
                                   DWORD /*dwUser*/, DWORD /*dw1*/, DWORD /*dw2*/ )
{
   // NB we may need to have a fine timer around for CW - at a variable rate
   currTick++;
   if ( tt )
      tt->tickEvent();
}
/*static*/baseTimer *baseTimer::initialiseTimer()
{
   if ( !tt )
   {
      tt = new baseTimer();
   }
   ttcount++;
   return tt;
}
/*static*/void baseTimer::killTimer()
{
   if ( tt )
      tt->tickers.clear();

   SetEvent( hTimerThreadCloseEvent );
   WaitForSingleObject( hTimerThread, INFINITE );

}
/*static*/void baseTimer::closeTimer()
{
   if ( --ttcount <= 0 )
   {
      killTimer();
      delete tt;
      tt = 0;
   }
}
// take the clock interrupt, use it to tick
baseTimer::baseTimer()
{
   unsigned int dwTimerThreadId;
   // NB that ABOVE_NORMAL is a Win2K invention
   hTimerThread = ( HANDLE ) _beginthreadex( NULL,                    // security
                  0,                        // stacksize
                  timerThread,            // start
                  this,                   // arg
                  ABOVE_NORMAL_PRIORITY_CLASS,     // createflags
                  &dwTimerThreadId           // threadid
                                           );
}
baseTimer::~baseTimer()
{
   killTimer();
}

//==============================================================================
timerTicker::timerTicker() : ready( false )
{
   b = baseTimer::initialiseTimer();
   b->registerTicker( this );
}
timerTicker::~timerTicker()
{
   ready = false;
   b->closeTimer();
}

//==============================================================================
commonController::commonController( )
{
}
void commonController::closeDown()
{
   baseTimer::killTimer();
   // close down each port
   for ( std::deque <commonPort *>::iterator icp = portChain.begin(); icp != portChain.end(); icp++ )
   {
      if ( *icp )
         ( *icp ) ->closePort();
      delete ( *icp );
   }
   portChain.clear();
}
commonController::~commonController()
{
}
/*
void commonController::ptt(int)
{
 
}
void commonController::key(int)
{
 
}
*/
bool commonController::initialise( )
{
   timerTicker::ready = true;
   return true;
}


void commonController::tickEvent()        // this will often be an interrupt routine
{
   checkControls();
}
void commonController::checkControls( void )
{
   // loop through ports, checkControls on each
   for ( my_deque < commonPort *>::iterator i = portChain.begin(); i != portChain.end(); i++ )
   {
      ( *i ) ->checkControls();
   }
}
commonPort *commonController::createPort( const PortConfig &port )
{
   // we cannot use dynamic_cast as we have turned RTTI off to save space
   commonPort * cp = portChain.find( port.name );

   if ( cp )
      return cp;
   else
   {
      // create the port

      switch ( port.portType )
      {
         case PortConfig::eptSerial:
            cp = new serialPort( port );
            break;

         case PortConfig::eptWindows:
            cp = new WindowsMonitorPort( port );
            break;

         case PortConfig::eptK8055:
            cp = new K8055Port( port );
            break;

         case PortConfig::eptUBW:
            cp = new UBWPort( port );
            break;
      }

   }
   return cp;
}
commonLineControl *commonController::findLine( const std::string &name, bool lineIn )
{
   commonLineControl * clc = 0;
   for ( my_deque < commonPort *>::iterator i = portChain.begin(); i != portChain.end(); i++ )
   {
      clc = ( *i ) ->findLine( name, lineIn );
      if ( clc )
      {
         break;
      }
   }
   return clc;
}
void commonController::lineChange( commonLineControl *line )
{
   /*
      RPCPubSub::publish( "LineControl", line->lineName, line->getState()?"set":"unset" );  // we also want time of change and time in state?
   */
   commonLineControl * pttout = findLine( "PTTOut", false );
   commonLineControl *pttin = findLine( "PTTIn", true );
   commonLineControl *l1 = findLine( "L1", true );
   commonLineControl *l2 = findLine( "L2", true );
   if ( pttout && pttin && l1 && l2 )
   {
      setLines( pttout->getState(), pttin->getState(), l1->getState(), l2->getState() );
   }
   LineSet *ls = LineSet::GetLineSet();
   ls->publish( line->lineName, line->getState() );
}
//==============================================================================

