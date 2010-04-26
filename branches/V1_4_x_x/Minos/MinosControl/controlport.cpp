/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop

#include <process.h>

#include "tinyxml.h"
#include "portconf.h"
#include "CommonMonitor.h"
#include "controlport.h"
#ifndef _STDEXCEPT_
 #include <stdexcept>
#endif

#include "WindowsMonitor.h"
#include "K8055Container.h"
#include "UBWContainer.h" 
//#include "K8055D.h"        // Velleman K8055 interface

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


//==============================================================================
commonLineControl::commonLineControl( std::string name, bool lineIn, std::string portLine, commonPort *port )
      : lineName( name ), lineIn( lineIn ), portLineName( portLine ), port( port ), state( false ), lastState( false )
{
}
commonLineControl::~commonLineControl()
{
}
void commonLineControl::checkLine()
{
   state = port->getLine( this );
   if ( state != lastState )
   {
      lastState = state;
      port->reportLineChange( this );
   }
}
void commonLineControl::setState( bool pstate )
{
   if ( state != pstate )
   {
      state = pstate;
      port->setLine( this );
      port->reportLineChange( this );
   }
}
//==============================================================================
commonPort::commonPort( const PortConfig &port ) :
      openCount( 0 ),
      pName( port.name ), portName( port.portName )
{

}
void commonPort::addLine( const LineConfig &line )
{
   commonLineControl * cll = new commonLineControl( line.name, line.lineIn, line.portLineName, this );
   lines.push_back( cll );
}

commonPort::~commonPort()
{
   for ( std::vector<commonLineControl *>::iterator i = lines.begin(); i != lines.end(); i++ )
   {
      delete ( *i );
   }
   lines.clear();
}
bool commonPort::initialise( const PortConfig & /*port*/, commonController &mon )
{
   if ( initialisePort() && openPort() )
   {
      mon.portChain.push_back( this );
      monitor = &mon;
      return true;
   }
   return false;
}
commonLineControl *commonPort::findLine( const std::string &name, bool lineIn )
{
   for ( std::vector<commonLineControl *>::iterator i = lines.begin(); i != lines.end(); i++ )
   {
      if ( ( *i ) ->lineIn == lineIn && ( *i ) ->lineName == name )
         return ( *i );
   }
   return 0;
}
void commonPort::checkControls( void )
{
   getLineState();
   for ( std::vector<commonLineControl *>::iterator i = lines.begin(); i != lines.end(); i++ )
   {
      // output lines can't sort their state in the same way
      if ( ( *i ) ->lineIn )
      {
         ( *i ) ->checkLine();
      }
   }
}
void commonPort::reportLineChange( commonLineControl *line )
{
   monitor->lineChange( line );
}

//==============================================================================
/*
serialPort *serialPort::C1 = 0;
serialPort *serialPort::C2 = 0;
*/
serialPort::serialPort( const PortConfig &port ) : commonPort( port ),
      speed( -1 ), parity( -1 ), bits( -1 ), stopbits( -1 )
{}
serialPort::~serialPort()
{
   closePort();
}

/*
struct MSPack
{
   unsigned char DTR : 1;
   unsigned char RTS : 1;
   unsigned char dummy : 2;
   unsigned char CTS : 1;
   unsigned char DSR : 1;
   unsigned char RI  : 1;
   unsigned char DCD : 1;
};
*/

void serialPort::getLineState()
{
   msr = aPort.getModemSignals();
}
bool serialPort::getLine( commonLineControl *line )
{
   if ( line->portLineName == "DSR" )
   {
      return ( msr.DSR == 0 );
   }
   if ( line->portLineName == "CTS" )
   {
      return ( msr.CTS == 0 );
   }
   if ( line->portLineName == "DCD" )
   {
      return ( msr.DCD == 0 );
   }
   return false;
}
void serialPort::setLine( commonLineControl *line )
{
   if ( line->portLineName == "RTS" )
   {
      aPort.setRTS( line->getState() );
   }
   if ( line->portLineName == "DTR" )
   {
      aPort.setDTR( line->getState() );
   }
}
bool serialPort::initialisePort()
{
   // read the port parameters from the INI file
   /* Communications parameters */

   if ( speed == -1 )
      speed = 2400;
   if ( parity == -1 )
      parity = COMPort::None;
   if ( bits == -1 )
      bits = 8;
   //  if (stopbits == -1)
   stopbits = COMPort::sb1;

   return true;
}
bool serialPort::openPort()
{
   try
   {
      if ( !aPort.Initialise ( portName.c_str() ) )
         return false;
      aPort.setBitRate ( speed );
      aPort.setParity ( ( COMPort::Parity ) parity );
      aPort.setDataBits ( ( COMPort::DataBits ) bits );
      aPort.setStopBits ( ( COMPort::StopBits ) stopbits );

      aPort.setRTS( false );
      aPort.setDTR( false );
   }
   catch ( const std::exception & sre )
   {
      return false;
   }
   catch ( ... )
   {
      return false;
   }
   return true;
}
bool serialPort::closePort()
{
   aPort.CloseDown();
   return true;
}
//==============================================================================
//=============================================================================
WindowsMonitorPort::WindowsMonitorPort( const PortConfig &port ) : commonPort( port )
{}
WindowsMonitorPort::~WindowsMonitorPort()
{
   closePort();
}


bool WindowsMonitorPort::initialisePort()
{
   WindowsMonitorForm = new TWindowsMonitorForm( 0 );
   WindowsMonitorForm->Show();
   return true;
}

bool WindowsMonitorPort::openPort()
{
   return true;
}

bool WindowsMonitorPort::closePort()
{
   delete WindowsMonitorForm;
   WindowsMonitorForm = 0;

   return true;
}

void WindowsMonitorPort::setLine( commonLineControl *line )
{
   if ( line->portLineName == "PTTOut" )
   {
      ;
   }
}
bool WindowsMonitorPort::getLine( commonLineControl *line )
{
   if ( line->portLineName == "PTTIn" )
   {
      return WindowsMonitorForm->PTTInCheckBox->Checked;
   }
   if ( line->portLineName == "L1" )
   {
      return WindowsMonitorForm->L1CheckBox->Checked;
   }
   if ( line->portLineName == "L2" )
   {
      return WindowsMonitorForm->L2CheckBox->Checked;
   }
   return false;
}
void WindowsMonitorPort::getLineState()
{
   // no need to get it all in one go
}


//==============================================================================
K8055Port::K8055Port( const PortConfig &port ) : commonPort( port ),
      addr( 0 ), dig( 0 )
{
   trace( "K8055Port::K8055Port" );
   addr = String( portName.c_str() ).ToIntDef( 0 );
}
K8055Port::~K8055Port()
{
   closePort();
}
bool K8055Port::initialisePort()
{
   return true;
}


bool K8055Port::openPort()
{
   int ret = K8055::openDevice( addr );
   return ret != -1;
}

bool K8055Port::closePort()
{
   K8055::closeDevice();
   return true;
}


void K8055Port::setLine( commonLineControl *line )
{
   int l = atoi( line->portLineName.c_str() );
   if ( line->getState() )
   {
      K8055::setDigitalChannel( l );
   }
   else
   {
      K8055::clearDigitalChannel( l );
   }
   //   K8055::writeAllDigital( line->getState() ? l : 0 );
}
bool K8055Port::getLine( commonLineControl *line )
{
   int l = atoi( line->portLineName.c_str() );
   return ( dig & l );
}
void K8055Port::getLineState()
{
   dig = K8055::readAllDigital();
}

//==============================================================================
UBWPort::UBWPort( const PortConfig &port ) : commonPort( port ), dig( 0 )
{
   trace( "UBWPort::UBWPort" );
}
UBWPort::~UBWPort()
{
   closePort();
}
bool UBWPort::initialisePort()
{
   return true;
}


bool UBWPort::openPort()
{
   int ret = UBW::openDevice();
   if ( ret != -1 )
   {
      // On startup, need to configure all the lines we intend to use as input or output
      for ( std::vector<commonLineControl *>::iterator i = lines.begin(); i != lines.end(); i++ )
      {
         UBW::setLineDirection( ( *i ) ->portLineName, ( *i ) ->lineIn );
      }
   }
   return ret != -1;
}

bool UBWPort::closePort()
{
   UBW::closeDevice();
   return true;
}


void UBWPort::setLine( commonLineControl *line )
{
   // a line is of the form A1, where A is RA (a - C), 1 is line 1 (0 - 7)
   UBW::setDigitalChannel( line->portLineName, line->getState() );
}
bool UBWPort::getLine( commonLineControl *line )
{
   bool l = UBW::readDigitalChannel( line->portLineName );
   return l;
}
void UBWPort::getLineState()
{
   // it happens in the background
   //   dig = UBW::readAllDigital();
}

//==============================================================================

