/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop
#include "boost\format.hpp"

#include "FindUSBComPort.h"
#include "UBWContainer.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUBWDM *UBWDM;
//---------------------------------------------------------------------------
__fastcall TUBWDM::TUBWDM( TComponent* Owner )
      : TDataModule( Owner ), dig( -1 )
{
   // first
}
bool TUBWDM::openPort()
{
   try
   {
      String key = PIBUBW;
      String portName;
      if ( !FindUSBComPort( key, portName ) )
      {
         trace( ( "No COM port found for " + key ).c_str() );
         return false;
      }

      trace( ( "COM port found for " + key + " at " + portName ).c_str() );
      if ( !aPort.Initialise ( portName.c_str() ) )
         return false;

      unsigned long written = 0;
      char *r = "R\r\n";
      char *vt = "V\r\nT,100,0\r\n";
      char *sp = r;
      bool res = aPort.COMPortWrite( sp, strlen( sp ), written ); // reset, version, set up timed input
      if ( !res )
      {
         trace( "Failed to write initial data (reset) to COM port - written " + makeStr( written ) );
      }

      WaitSpinning( 1000 );

      sp = vt;
      while ( true )
      {
         bool res = aPort.COMPortWrite( sp, strlen( sp ), written ); // reset, version, set up timed input
         if ( !res )
         {
            trace( "Failed to write initial data to COM port - written " + makeStr( written ) );
         }
         sp += written;
         //         if (strlen(rvt) <= sp - rvt)
         {
            return true;
         }
      }
   }
   catch ( const std::exception & sre )
   {
      return false;
   }
   catch ( ... )
   {}
   return false;
}
void TUBWDM::setLineDirection( const std::string &portLineName, bool lineIn )
{
   int portno;
   int lineno;
   if ( parseLine( portLineName, portno, lineno ) )
   {
      // state "true" means "pulled down", so invert it
      std::string pd = ( boost::format( "PD,%c,%d,%d\r\n" ) % ( "ABC"[ portno ] ) % lineno % lineIn ).str();
      const char *sp = pd.c_str();
      while ( true )
      {
         unsigned long written = 0;
         bool res = aPort.COMPortWrite( sp, strlen( sp ), written ); // reset, version, set up timed input
         if ( !res )
         {
            trace( "Failed to write port direction data to COM port - written " + makeStr( written ) );
         }
         sp += written;
         //         if (strlen(rvt) <= sp - rvt)
         {
            trace( pd );
            return ;
         }
      }
   }
}
bool TUBWDM::closePort()
{
   char * t = "T,0,0\r\n";
   unsigned long written = 0;
   /*bool res =*/ aPort.COMPortWrite( t, strlen( t ), written ); // reset, version, set up timed input
   trace( t );
   aPort.CloseDown();
   return true;
}
bool TUBWDM::parseLine( const std::string &R, int &portno, int &lineno )
{
   if ( R.size() >= 2 )
   {
      char port = R[ 0 ];
      char line = R[ 1 ];
      if ( port == 'R' && R.size() >= 3 )
      {
         port = R[ 1 ];
         line = R[ 2 ];
      }
      if ( !isalpha( port ) || !isdigit( line ) )
      {
         return false;
      }
      portno = port - 'A';
      lineno = line - '0';
      if ( portno > 2 || lineno > 7 )
      {
         return false;
      }
      return true;
   }
   return false;
}
long TUBWDM::getReadMask( const std::string &R )
{
   long mask = 0;
   int portno;
   int lineno;
   if ( parseLine( R, portno, lineno ) )
   {
      int shift = portno * 8 + lineno;
      mask = 1 << shift;
   }
   return mask;
}
void TUBWDM::setLine( const std::string &R, bool state )
{
   int portno;
   int lineno;
   if ( parseLine( R, portno, lineno ) )
   {
      // state "true" means "pulled down", so invert it
      std::string po = ( boost::format( "PO,%c,%d,%d\r\n" ) % ( "ABC"[ portno ] ) % lineno % !state ).str();
      const char *sp = po.c_str();
      while ( true )
      {
         unsigned long written = 0;
         bool res = aPort.COMPortWrite( sp, strlen( sp ), written ); // reset, version, set up timed input
         if ( !res )
         {
            trace( "Failed to write output data to COM port - written " + makeStr( written ) );
         }
         sp += written;
         //         if (strlen(rvt) <= sp - rvt)
         {

            trace( po );
            return ;
         }
      }
   }
}
bool TUBWDM::getLine( const std::string &R )
{
   // decompose line string into char and number
   // then get the correct part of "dig"
   return !( dig & getReadMask( R ) );        // values are inverted - switch closed is 0 and true....
}
std::string TUBWDM::processRxBuff( const std::string &rxBuff )
{
   // each response is 0x0d0x0a terminated
   // OK
   // UBW FW ...
   // I,001,045,205 - RA, RB, RC
   // A,0145,1004,0000,0045 - 0 - 1023 for each defined analogue input
   // MR,28 (memory read) - don't plan to use this
   // PI,1 - response to PI,port,pin

   std::string ostr;
   std::string result;
   unsigned int crlf = rxBuff.find( "\n\r" );
   if ( crlf != std::string::npos )
   {
      result = rxBuff.substr( 0, crlf );
      ostr = rxBuff.substr( crlf + 2, rxBuff.size() );
      // and analyse it...
      //trace( "UBWPort::read " + result );
      if ( result[ 0 ] == 'I' )
      {
         std::string RA = result.substr( 2, 3 );
         std::string RB = result.substr( 6, 3 );
         std::string RC = result.substr( 10, 3 );

         int ra = atoi( RA.c_str() );
         int rb = atoi( RB.c_str() );
         int rc = atoi( RC.c_str() );
         dig = ( ( ( ( ( rc & 0xff ) << 8 ) + ( rb & 0xff ) ) << 8 ) + ( ra & 0xff ) );
      }
      return ostr;
   }
   return rxBuff;

}
void USBComPort::processRxBuff()
{
   rxBuff = UBWDM->processRxBuff( rxBuff );
}
//---------------------------------------------------------------------------
namespace UBW
{
   long openDevice( )
   {
      if ( !UBWDM )
      {
         UBWDM = new TUBWDM( 0 );   // data module, so no need to show it
         UBWDM->openPort();
      }
      return true;
   }
   void closeDevice()
   {
      // we don't actually ever close it
      if ( UBWDM )
      {
         UBWDM->closePort();
      }
   }
   long readAnalogChannel( const std::string &/*R*/ )
   {
      return 0;
   }
   void readAllAnalog( long * /*Data1*/, long * /*Data2*/ )
   {
   }
   void outputAnalogChannel( long /*Channel*/, long /*Data*/ )
   {
   }
   void outputAllAnalog( long /*Data1*/, long /*Data2*/ )
   {
   }
   void clearAnalogChannel( const std::string &/*R*/ )
   {
   }
   void clearAllAnalog()
   {
   }
   void setAnalogChannel( const std::string &/*R*/ )
   {
   }
   void setAllAnalog()
   {
   }
   void writeAllDigital( long Data )
   {
      UBWDM->setLines( Data );
   }
   void clearAllDigital()
   {
      writeAllDigital( 0 );
   }
   void setDigitalChannel( const std::string &R, bool state )
   {
      UBWDM->setLine( R, state );
   }
   void setAllDigital()
   {
      writeAllDigital( -1 );
   }
   bool readDigitalChannel( const std::string &R )
   {
      return UBWDM->getLine( R );
   }
   void setLineDirection( const std::string &portLineName, bool lineIn )
   {
      UBWDM->setLineDirection( portLineName, lineIn );
   }
}

