/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "MinosVK_pch.h"
#pragma hdrstop
#ifndef _STDEXCEPT_
 #include <stdexcept>
#endif

//typedef void FAR PASCAL (*LineCallBack)(bool PTT, bool keyline);

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

#include "keyctrl.h"
#include "keyers.h"
#include "portcon.h"
#include "WindowsMonitor.h"
#include "lineEvents.h"

// For serial port
//#include "serial.h"     // serial port constants

//#include "K8055D.h"        // Velleman K8055 interface

#include "KeyerRPCServer.h" 
//==============================================================================

// This module needs splitting up into common, serial, parallel
// (and later anything else, such as USB, or whatever)

//==============================================================================
/*
      Configuration: we need INI file sections for each possible port and ticker
      so we can set each one up as it is registered to a keyer.
 
      Each keyer INI section defines which port(s) it uses, and which
      ticker it uses;
      then when it registers we set up the corresponding port object
      and let the keyer object register to it.
 
*/

my_deque < commonPort *> portChain;
WinMonitor *winp = 0;
/*
*/ 
//==============================================================================
commonPort *createPort( const PortConfig &port )
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
         case PortConfig::eptWindows:
            cp = new WindowsMonitorPort( port );
            break;

         case PortConfig::eptMinosControl:
            cp = new LineEventsPort( port );
            break;
      }

      if ( !cp->initialise( port ) )
      {
         delete cp;
         cp = 0;
      }
      if ( cp && !winp )
      {
         winp = new WinMonitor();
         cp->monitors.push_back( winp );
      }
   }
   return cp;
}
//==============================================================================
commonPort::commonPort( const PortConfig &port ) :
      openCount( 0 ),
      lastPttState( false ), lastL1State( false ), lastL2State( false ),
      pName( port.name )
{

}
commonPort::~commonPort()
{
   if ( sblog )
   {
      trace( "~commonPort" );
   }
   monitors.freeAll();
}
bool commonPort::initialise( const PortConfig &/*port*/ )
{
   if ( initialisePort() && openPort() )
   {
      portChain.push_back( this );
      return true;
   }
   return false;
}
void commonPort::registerMonitor( lineMonitor *k )
{
   monitors.push_back( k );
}
//==============================================================================
void commonPort::pttChanged( int state )
{
   for ( my_deque < lineMonitor * >::iterator l = monitors.begin(); l != monitors.end(); l++ )
      ( *l ) ->pttChanged( state );
}
void commonPort::L1Changed( int state )
{
   for ( my_deque < lineMonitor * >::iterator l = monitors.begin(); l != monitors.end(); l++ )
      ( *l ) ->L1Changed( state );
}
void commonPort::L2Changed( int state )
{
   for ( my_deque < lineMonitor * >::iterator l = monitors.begin(); l != monitors.end(); l++ )
      ( *l ) ->L2Changed( state );
}
//=============================================================================
LineCallBack WindowsMonitorPort::WinLineCallback = 0;
bool WindowsMonitorPort::PTTInState = false;
bool WindowsMonitorPort::L1State = false;
bool WindowsMonitorPort::L2State = false;

WindowsMonitorPort::WindowsMonitorPort( const PortConfig &port ) : commonPort( port ),
      PTTState( false ), keyState( false ), winMonForm( 0 )
{}
WindowsMonitorPort::~WindowsMonitorPort()
{
   if ( sblog )
   {
      trace( "WindowsMonitorPort::~WindowsMonitorPort" );
   }
   closePort();
}


bool WindowsMonitorPort::initialisePort()
{
   winMonForm = new TWindowsMonitorForm( 0 );
   winMonForm->Show();
   return true;
}

bool WindowsMonitorPort::openPort()
{
   return true;
}

bool WindowsMonitorPort::closePort()
{
   delete winMonForm;
   winMonForm = 0;

   return true;
}


void WindowsMonitorPort::ptt( int state )
{
   if ( currentKeyer && currentKeyer->started )
   {
      PTTState = state;
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   }
}

void WindowsMonitorPort::key( int state )
{
   if ( currentKeyer && currentKeyer->started )
   {
      keyState = state;
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   }
}


void WindowsMonitorPort::checkControls( void )
{
   // check and action PTT, L1, L2
   if ( L2State != lastL2State )
   {
      lastL2State = L2State;
      if ( sblog )
      {
         trace( "L2 changed to " + makeStr( lastL2State ) );
      }
      L2Changed( lastL2State );
   }
   if ( L1State != lastL1State )
   {
      lastL1State = L1State;
      if ( sblog )
      {
         trace( "L1 changed to " + makeStr( lastL1State ) );
      }
      L1Changed( lastL1State );
   }
   if ( PTTInState != lastPttState )
   {
      lastPttState = PTTInState;
      if ( sblog )
      {
         trace( "PTT changed to " + makeStr( !lastPttState ) );
      }
      pttChanged( lastPttState );
   }
}
//==============================================================================
LineCallBack WinMonitor::WinLineCallback = 0;
bool WinMonitor::PTTInState = false;
bool WinMonitor::L1State = false;
bool WinMonitor::L2State = false;

WinMonitor::WinMonitor() : lineMonitor( "WinMon" ),
      PTTState( false ), keyState( false )
{}
WinMonitor::~WinMonitor()
{
   if ( sblog )
   {
      trace( "WinMonitor::~WinMonitor" );
   }
}

bool WinMonitor::initialise( const KeyerConfig &/*keyer*/, const PortConfig &/*port*/ )
{
   return true;
}

void WinMonitor::ptt( int state )
{
   if ( currentKeyer && currentKeyer->started )
   {
      PTTState = state;
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   }
}

void WinMonitor::key( int state )
{
   if ( currentKeyer && currentKeyer->started )
   {
      keyState = state;
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   }
}


void WinMonitor::checkControls( void )
{}
bool WinMonitor::L1Changed( int state )
{
   L1State = state;
   if ( WinLineCallback )
      ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   return true;
}
bool WinMonitor::L2Changed( int state )
{
   L2State = state;
   if ( WinLineCallback )
      ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   return true;
}
bool WinMonitor::pttChanged( int state )
{
   PTTInState = state;
   if ( WinLineCallback )
      ( *WinLineCallback ) ( PTTState, keyState, PTTInState, L1State, L2State );
   return true;
}
//==============================================================================
LineCallBack LineEventsPort::WinLineCallback = 0;

void LineLog( const std::string &msg )
{
   trace( msg );
}
LineEventsPort::LineEventsPort( const PortConfig &port ) : commonPort( port ),
      lastPTTState( false ), lastL1State( false ), lastL2State( false ),
      portLineEventThread( 0 ), closing( false ),
      linePTTState( false ), lineL1State( false ), lineL2State( false )
{
}
LineEventsPort::~LineEventsPort()
{
   closing = true;
   portLineEventThread->GJV_join();
   delete portLineEventThread;
   portLineEventThread = 0;
   closePort();
}

void LineEventsPort::doProcLineEvents( )
{
   LineSet * ls = LineSet::GetLineSet();
   while ( !closing && !checkCloseEvent() )
   {
      ls->readLines();
      linePTTState = ls->getState( "PTTIn" );
      lineL1State = ls->getState( "L1" );
      lineL2State = ls->getState( "L2" );

      ls->waitForChange( 500 );
   }
}
void procLineEvents( void *p )
{
   //---- Place thread code here ----
   LineEventsPort * f = ( LineEventsPort* ) p;
   f->doProcLineEvents();
}
bool LineEventsPort::initialisePort()
{
   LineSet * ls = LineSet::GetLineSet();
   ls->lsLog = LineLog;

   portLineEventThread = new GJV_thread( "portLineEventThread", procLineEvents, ( void * ) this, true ) ;
   return true;
}

bool LineEventsPort::openPort()
{
   return true;
}
bool LineEventsPort::closePort()
{
   return true;
}

void LineEventsPort::ptt( int state )
{
   if ( currentKeyer && currentKeyer->started )
   {
      LineSet * ls = LineSet::GetLineSet();
      ls->publish( "PTTOut", state );
      PTTState = state;
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, lastPTTState, lastL1State, lastL2State );
   }
}
void LineEventsPort::key( int state )
{
   if ( currentKeyer && currentKeyer->started )
   {
      LineSet * ls = LineSet::GetLineSet();
      ls->publish( "Key", state );
      keyState = state;
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, lastPTTState, lastL1State, lastL2State );
   }
}

void LineEventsPort::checkControls( void )
{
   if ( lineL2State != lastL2State )       //L2
   {
      lastL2State = !lastL2State;
      L2Changed( lastL2State );
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, lastPTTState, lastL1State, lastL2State );
   }
   if ( lineL1State != lastL1State )       //L1
   {
      lastL1State = !lastL1State;
      L1Changed( lastL1State );
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, lastPTTState, lastL1State, lastL2State );
   }
   if ( linePTTState != lastPTTState )       //PTT
   {
      lastPTTState = !lastPTTState;
      pttChanged( lastPTTState );
      if ( WinLineCallback )
         ( *WinLineCallback ) ( PTTState, keyState, lastPTTState, lastL1State, lastL2State );
   }
}
//==============================================================================

