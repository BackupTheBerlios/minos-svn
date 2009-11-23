/////////////////////////////////////////////////////////////////////////////
// $Id: COMPORT.CPP 876 2008-01-03 21:48:26Z mjg $
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
/////////////////////////////////////////////////////////////////////////////
//=============================================================================
//  General component library for WIN32
//  Copyright (C) 2000, UAB BBDSoft ( http://www.bbdsoft.com/ )
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
//
//  The author of this program may be contacted at developers@bbdsoft.com
//=============================================================================

#pragma hdrstop
#include "ComPort.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdexcept>
#include <string>
#include <process.h>

#include <syncobjs.hpp> // TCriticalSection
#include "MTrace.h"
#include "GJVThreads.h"

class RxGuard
{
      static TCriticalSection * pCS;
   public:
      __fastcall RxGuard()
      {
         if ( !pCS )
            pCS = new TCriticalSection();
         pCS->Enter();
      }
      __fastcall ~RxGuard()
      {
         if ( pCS )
            pCS->Leave();
      }
      static void clear()
      {
         delete pCS;
         pCS = 0;
      }
};
TCriticalSection * RxGuard::pCS = 0;
class TxGuard
{
      static TCriticalSection * pCS;
   public:
      __fastcall TxGuard()
      {
         if ( !pCS )
            pCS = new TCriticalSection();
         pCS->Enter();
      }
      __fastcall ~TxGuard()
      {
         if ( pCS )
            pCS->Leave();
      }
      static void clear()
      {
         delete pCS;
         pCS = 0;
      }
};
TCriticalSection * TxGuard::pCS = 0;
//----------------------------------------------------------------------------
void trace( String mess )
{
   trace( mess.c_str() );
}
//---------------------------------------------------------------------------
String hexchar( int c )
{
   CHAR rgbDigits[] = "0123456789abcdef";
   String ret = rgbDigits[ ( c >> 4 ) & 0x0f ];
   ret += rgbDigits[ c & 0x0f ];
   return ret;
}
//---------------------------------------------------------------------------
static String PrintHexDump( DWORD length, PBYTE buffer, DWORD index = 0 )
{
   String retval;

   if ( length > 1024 )
   {
      retval = PrintHexDump( 16, buffer, 0 );
      retval += "\n" + PrintHexDump( 16, buffer, length - 16 );
      return retval;
   }

   // borrowed code... nasty stuff...
   DWORD count = 0;
   for (  /*index = 0*/; length; length -= count, index += count )
   {
      CHAR rgbLine[ 200 ];
      char cbLine;
      count = ( length > 16 ) ? 16 : length;

      wsprintf( rgbLine, "%4.4x  ", index );
      cbLine = 6;

      unsigned int i;
      for ( i = 0;i < count;i++ )
      {
         String ch = hexchar( buffer[ index + i ] );
         rgbLine[ cbLine++ ] = ch[ 1 ];
         rgbLine[ cbLine++ ] = ch[ 2 ];
         if ( i == 7 )
         {
            rgbLine[ cbLine++ ] = ' ';
            rgbLine[ cbLine++ ] = ':';
            rgbLine[ cbLine++ ] = ' ';
         }
         else
         {
            rgbLine[ cbLine++ ] = ' ';
         }
      }
      for ( ; i < 16; i++ )
      {
         if ( i == 7 )
         {
            rgbLine[ cbLine++ ] = ' ';
            rgbLine[ cbLine++ ] = ' ';
         }
         rgbLine[ cbLine++ ] = ' ';
         rgbLine[ cbLine++ ] = ' ';
         rgbLine[ cbLine++ ] = ' ';
      }

      rgbLine[ cbLine++ ] = ' ';

      for ( i = 0; i < count; i++ )
      {
         if ( buffer[ index + i ] < 32 || buffer[ index + i ] > 126 )
         {
            rgbLine[ cbLine++ ] = '.';
         }
         else
         {
            rgbLine[ cbLine++ ] = buffer[ index + i ];
         }
      }

      rgbLine[ cbLine++ ] = 0;
      if ( retval.Length() )
      {
         retval += "\n";
      }
      retval += rgbLine ;
   }
   return retval;
}
//----------------------------------------------------------------------------
COMPort::COMPort ( )
      : theDCB ( NULL ),
      ComThread( 0 ), ComTerminated( false ),
      thePortHandle( 0 ), CancelOverlappedhEvent( 0 )
{
   thePortHandle = INVALID_HANDLE_VALUE;
} // end constructor
//----------------------------------------------------------------------------
COMPort::~COMPort()
{
   CloseDown();
   delete [] theDCB;
} // end destructor


//----------------------------------------------------------------------------
void COMPort::getState () const
{

   if ( !GetCommState ( ( HANDLE ) thePortHandle
                        , ( LPDCB ) theDCB
                      )
      )
   {
      throw std::runtime_error ( "COMPort: could not retrieve serial port state." );
   } // endif

} // end COMPort::getState () const


//----------------------------------------------------------------------------
COMPort& COMPort::setState ()
{

   if ( !SetCommState ( ( HANDLE ) thePortHandle
                        , ( LPDCB ) theDCB
                      )
      )
   {
      throw std::runtime_error ( "COMPort: could not modify serial port state." );
   } // endif

   return *this;
} // end COMPort::setState ()


//-----------------------------------------------------------------------------
COMPort& COMPort::setBitRate ( unsigned long Param )
{

   DCB & aDCB = *( ( LPDCB ) theDCB );
   aDCB.BaudRate = Param;

   return setState();
} // end COMPort::setBitRate (..)


//-----------------------------------------------------------------------------
unsigned long COMPort::bitRate() const
{

   DCB & aDCB = *( ( LPDCB ) theDCB );

   return aDCB.BaudRate;
} // end COMPort::bitRate () const


//-----------------------------------------------------------------------------
COMPort& COMPort::setLineCharacteristics( char * inConfig )
{

   COMMTIMEOUTS aTimeout;
   if ( !BuildCommDCBAndTimeouts ( inConfig
                                   , ( LPDCB ) theDCB
                                   , &aTimeout
                                 )
      )
   {
      throw std::runtime_error ( "COMPort: could not set line characteristics." );
   } // endif

   if ( ! SetCommTimeouts ( ( HANDLE( thePortHandle ) )
                            , &aTimeout
                          )
      )
   {
      throw std::runtime_error ( "COMPort: could not set line characteristics." );
   } // endif

   return setState();
}


//----------------------------------------------------------------------------
/*int COMPort::read ( unsigned char &ch )
{
 
   char buffer;
   DWORD charsRead = 0;
 
   if ( ! ReadFile ( ( HANDLE( thePortHandle ) )
                     , &buffer
                     , sizeof( char )
                     , &charsRead
                     , NULL
                   )
      )
   {
      throw std::runtime_error ( "COMPort: read failed." );
   } // endif
 
   ch = buffer;
 
   return charsRead;
} // end COMPort::read()
*/

//----------------------------------------------------------------------------
/*unsigned long COMPort::read ( void *inBuffer
                              , const unsigned long inCharsReq
                            )
{
 
   DWORD charsRead = 0;
   if ( !ReadFile ( ( HANDLE( thePortHandle ) )
                    , inBuffer
                    , inCharsReq
                    , &charsRead
                    , NULL
                  )
      )
   {
      throw std::runtime_error ( "COMPort: read failed." );
   } // endif
 
   return charsRead;
} // end COMPort::read (..)
*/

//----------------------------------------------------------------------------
/*
COMPort & COMPort::write ( const char inChar )
{
 
   char buffer = inChar;
   DWORD charsWritten = 0;
 
   if ( !WriteFile ( ( HANDLE( thePortHandle ) )
                     , &buffer
                     , sizeof( char )
                     , &charsWritten
                     , NULL
                   )
      )
   {
      throw std::runtime_error ( "COMPort: write failed." );
   } // endif
 
   return *this;
} // end COMPort::write (..)
*/

//----------------------------------------------------------------------------
/*
unsigned long COMPort::write ( const void *inBuffer
                               , const unsigned long inBufSize
                             )
{
 
   DWORD charsWritten = 0;
 
   if ( !WriteFile ( ( HANDLE( thePortHandle ) )
                     , inBuffer
                     , inBufSize
                     , &charsWritten
                     , NULL
                   )
      )
   {
      throw std::runtime_error ( "COMPort: write failed." );
   } // endif
 
   return charsWritten;
} // end COMPort::write()
 
*/ 
//-----------------------------------------------------------------------------
COMPort& COMPort::setxONxOFF ( bool Param )
{

   DCB & aDCB = *( ( LPDCB ) theDCB );
   aDCB.fOutX = Param ? 1 : 0;
   aDCB.fInX = Param ? 1 : 0;

   return setState();
} // end COMPort::setxONxOFF (..)


//-----------------------------------------------------------------------------
bool COMPort::isxONxOFF () const
{

   DCB & aDCB = *( ( LPDCB ) theDCB );

   return ( aDCB.fOutX && aDCB.fInX );
} // end COMPort::isxONxOFF () const


//----------------------------------------------------------------------------
COMPort& COMPort::setBlockingMode ( unsigned long inReadInterval
                                    , unsigned long inReadMultiplyer
                                    , unsigned long inReadConstant
                                  )
{

   COMMTIMEOUTS commTimeout;
   if ( !GetCommTimeouts ( ( HANDLE( thePortHandle ) )
                           , &commTimeout
                         )
      )
   {
      throw std::runtime_error ( "COMPort: failed to retrieve timeouts." );
   } // endif

   commTimeout.ReadIntervalTimeout = inReadInterval;

   if ( inReadInterval == MAXDWORD )
   {
      commTimeout.ReadTotalTimeoutMultiplier = 0;
      commTimeout.ReadTotalTimeoutConstant = 0;
   }
   else
   {
      commTimeout.ReadTotalTimeoutMultiplier = inReadMultiplyer;
      commTimeout.ReadTotalTimeoutConstant = inReadConstant;
   }  // endifelse

   if ( !SetCommTimeouts ( ( HANDLE( thePortHandle ) )
                           , &commTimeout
                         )
      )
   {
      throw std::runtime_error ( "COMPort: failed to modify timeouts." );
   } // endif

   return *this;
} // end COMPort::setBlockingMode (..)


//-----------------------------------------------------------------------------
COMPort & COMPort::setHandshaking ( bool inHandshaking )
{

   DCB & aDCB = *( ( LPDCB ) theDCB );
   if ( inHandshaking )
   {
      aDCB.fOutxCtsFlow = TRUE;
      aDCB.fOutxDsrFlow = FALSE;
      aDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;
   }
   else
   {
      aDCB.fOutxCtsFlow = FALSE;
      aDCB.fOutxDsrFlow = FALSE;
      aDCB.fRtsControl = RTS_CONTROL_DISABLE;
      aDCB.fDtrControl = DTR_CONTROL_DISABLE	;
   } // endifelse

   return setState();
} // end COMPort::setHandshaking (..)


//-----------------------------------------------------------------------------
unsigned long COMPort::getMaximumBitRate() const
{

   COMMPROP aProp;
   if ( !GetCommProperties ( ( HANDLE ) thePortHandle
                             , &aProp )
      )
   {
      throw std::runtime_error ( "COMPort: failed to retrieve port properties." );
   } // endif

   return aProp.dwMaxBaud;
} // end COMPort::getMaximumBitRate () const


//-----------------------------------------------------------------------------
COMPort::MSPack COMPort::getModemSignals() const
{

   MSPack aPack;
   // 1 bit - DTR, 2 - bit RTS                           (output signals)
   // 4 bit - CTS, 5 bit - DSR, 6 bit - RI, 7 bit - DCD  (input signals)
   if ( !GetCommModemStatus ( ( HANDLE ) thePortHandle
                              , ( LPDWORD ) & aPack )
      )
   {
      throw std::runtime_error ( "COMPort: failed to retrieve modem signals." );
   } // endif

   return aPack;
} // end COMPort::getModemSignals () const

//-----------------------------------------------------------------------------

void COMPort::setRTS( bool state )
{
   if ( !EscapeCommFunction( ( HANDLE ) thePortHandle, state ? SETRTS : CLRRTS ) )
   {
      throw std::runtime_error ( "COMPort: failed to set RTS." );
   } // endif
}
void COMPort::setDTR( bool state )
{
   if ( !EscapeCommFunction( ( HANDLE ) thePortHandle, state ? SETDTR : CLRDTR ) )
   {
      throw std::runtime_error ( "COMPort: failed to set DTR." );
   } // endif
}
//-----------------------------------------------------------------------------
COMPort& COMPort::setParity ( Parity Param )
{

   DCB & aDCB = *( ( LPDCB ) theDCB );
   aDCB.Parity = Param;

   return setState();
} // end COMPort::setParity (..)


//-----------------------------------------------------------------------------
COMPort& COMPort::setDataBits ( DataBits Param )
{

   DCB & aDCB = *( ( LPDCB ) theDCB );
   aDCB.ByteSize = Param;

   return setState();
} // end COMPort::setDataBits (..)


//-----------------------------------------------------------------------------
COMPort& COMPort::setStopBits ( StopBits Param )
{

   DCB & aDCB = *( ( LPDCB ) theDCB );
   aDCB.StopBits = Param;

   return setState();
} // end COMPort::setStopBits (..)


//-----------------------------------------------------------------------------
COMPort::Parity COMPort::parity () const
{

   DCB & aDCB = *( ( LPDCB ) theDCB );

   return ( COMPort::Parity ) aDCB.Parity;
} // end COMPort::parity () const


//-----------------------------------------------------------------------------
COMPort::DataBits COMPort::dataBits () const
{

   DCB & aDCB = *( ( LPDCB ) theDCB );

   return ( COMPort::DataBits ) aDCB.ByteSize;
} // end COMPort::dataBits () const


//-----------------------------------------------------------------------------
COMPort::StopBits COMPort::stopBits () const
{

   DCB & aDCB = *( ( LPDCB ) theDCB );

   return ( COMPort::StopBits ) aDCB.StopBits;
} // end COMPort::stopBits () cosnt
// clean wait - Sleep tends to be unfriendly and inaccurate

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void WaitSpinning( unsigned int ms )
{
   HANDLE dummyEvent = CreateEvent( NULL, FALSE, FALSE, 0 );   // internal, auto reset
   DWORD start_tc = GetTickCount();
   while ( true )
   {
      DWORD now_tc = GetTickCount();
      if ( now_tc - start_tc >= ms )
      {
         // no need to reset start_tc - we aren't coming back!
         break;
      }

      WaitForSingleObject( dummyEvent, 10 );
   }
   CloseHandle( dummyEvent );
}
//---------------------------------------------------------------------------
bool COMPort::Initialise( const std::string &PortName )
{
   bool Success = false;
   trace( "OpenPort" );

   ReadOverlapped.Internal = 0;
   ReadOverlapped.InternalHigh = 0;
   ReadOverlapped.Offset = 0;
   ReadOverlapped.OffsetHigh = 0;
   ReadOverlapped.hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

   WriteOverlapped.Internal = 0;
   WriteOverlapped.InternalHigh = 0;
   WriteOverlapped.Offset = 0;
   WriteOverlapped.OffsetHigh = 0;
   WriteOverlapped.hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

   CancelOverlappedhEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

   Success = OpenPort( PortName );
   trace( "OpenPort result = " + IntToStr( (int)Success ) );
   if ( Success )
   {
      WaitSpinning( 1000 );     // allow time to clear the buffers - and maybe see a packet start
   }
   return Success;
}
//---------------------------------------------------------------------------
bool COMPort::CloseDown()
{
   SetEvent( CancelOverlappedhEvent );
   ComTerminated = true;
   WaitForSingleObject( ComThread, 10000 );
   CloseHandle( ComThread );
   ComThread = 0;
   bool Success = ClosePort();
   if ( ReadOverlapped.hEvent )
      CloseHandle( ReadOverlapped.hEvent );
   if ( WriteOverlapped.hEvent )
      CloseHandle( WriteOverlapped.hEvent );
   if ( CancelOverlappedhEvent )
      CloseHandle( CancelOverlappedhEvent );
   ReadOverlapped.hEvent = 0;
   WriteOverlapped.hEvent = 0;
   CancelOverlappedhEvent = 0;
   trace( "ClosePort result = " + IntToStr( (int)Success ) );
   RxGuard::clear();
   TxGuard::clear();
   return Success;
}
//---------------------------------------------------------------------------
bool COMPort::COMPortWrite ( const void *inBuffer
                             , const unsigned long inBufSize
                             , unsigned long &charsWritten
                           )
{

   if ( thePortHandle != INVALID_HANDLE_VALUE )
   {
      charsWritten = 0;

      ResetEvent( WriteOverlapped.hEvent );
      BOOL ret = WriteFile ( thePortHandle
                             , inBuffer
                             , inBufSize
                             , &charsWritten
                             , &WriteOverlapped
                           );

      if ( !ret && GetLastError() == ERROR_IO_PENDING )
      {
         // return before write completes - WriteOverlapped.hEvent signalled on completion
         if ( WaitForSingleObject( WriteOverlapped.hEvent, 5000 ) == WAIT_TIMEOUT )
         {
            typedef BOOL ( __stdcall * FUNCTPTR ) ( HANDLE );
            static HINSTANCE hInstKernel = NULL;
            if ( !hInstKernel )
               hInstKernel = LoadLibrary( _T( "kernel32.dll" ) );
            static FUNCTPTR FunctPtr = NULL;
            if ( !FunctPtr )
               FunctPtr = ( FUNCTPTR ) GetProcAddress( hInstKernel, "CancelIo" );
            if ( FunctPtr )
               FunctPtr( thePortHandle );
            trace ( String( "COMPort: write failed - timeout of async write" ) );
            ret = false;
         }
         else
         {
            if ( !GetOverlappedResult( thePortHandle, &WriteOverlapped, &charsWritten, FALSE ) )
            {
               // Error in communications; report it.
               trace( "Comms error writing async characters" );
               charsWritten = 0;
            }
            else
               return true;
         }
      }
      else
      {
         // Possibly OK
         if ( !ret )
         {
            trace ( std::string( "COMPort: write failed - " ) + lastError() );
         }
         else
         {
            trace( std::string( "Write successful " ) + ( const char * ) inBuffer );
         }
      }
      return ret;
   }
   return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool COMPort::OpenPort( const std::string &PortName )
{

   try
   {
      std::string lasterr;
      for ( int opencount = 0; opencount < 10; opencount++ )
      {
         thePortHandle = CreateFile ( PortName.c_str()
                                      , GENERIC_READ | GENERIC_WRITE
                                      , 0
                                      , NULL
                                      , OPEN_EXISTING
                                      , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED
                                      , NULL
                                    );

         lasterr = lastError();
         if ( thePortHandle != INVALID_HANDLE_VALUE )
            break;

         WaitSpinning( 100 );
      }
      if ( thePortHandle == INVALID_HANDLE_VALUE )
      {
         trace( std::string( "Failed to open " ) + PortName + " - " + lasterr );
         return false;
      } // endif

      WaitSpinning( 100 );

      theDCB = ( LPDCB ) new char [ sizeof( DCB ) ];
      memset( theDCB, 0, sizeof( DCB ) );
      theDCB->DCBlength = sizeof( DCB );

      theDCB->BaudRate = 111520;
      theDCB->Parity = 0;
      theDCB->ByteSize = 8;
      theDCB->StopBits = 0;             // which is 1 stop bit...
      if ( !SetCommState ( thePortHandle, theDCB ) )
      {
         trace ( "COMPort: could not modify serial port state." );
         return false;
      } // endif


      COMMTIMEOUTS TimeOuts;
      memset( &TimeOuts, 0, sizeof( TimeOuts ) );
      TimeOuts.ReadIntervalTimeout = 1;
      TimeOuts.ReadTotalTimeoutConstant = 1;
      TimeOuts.ReadTotalTimeoutMultiplier = 1;
      TimeOuts.WriteTotalTimeoutConstant = 1;
      TimeOuts.WriteTotalTimeoutMultiplier = 1;

      if ( !SetCommTimeouts ( thePortHandle, &TimeOuts ) )
      {
         trace ( "COMPort: failed to modify timeouts." );
         return false;
      } // endif

      WaitSpinning( 100 );
      // start new thread
      StartComThread();
      WaitSpinning( 100 );
   }
   catch ( std::runtime_error & e )
   {
      trace( String( "Exception in initialise COM port : " ) + e.what() );
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------
bool COMPort::ClosePort()
{
   trace ( "ClosePort" );
   delete [] theDCB;
   // close serial port device
   if ( thePortHandle != INVALID_HANDLE_VALUE )
   {
      if ( !CloseHandle ( thePortHandle ) )
      {
         trace ( "ClosePort failed - " + lastError() );
      }
      thePortHandle = INVALID_HANDLE_VALUE;
   }
   return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
static unsigned int __stdcall tComThreadExecute( void *args )
{
   COMPort * myobj = ( COMPort * ) args;
   return myobj->ComThreadExecute();
}
//---------------------------------------------------------------------------
void COMPort::CancelIO()
{
   typedef BOOL ( __stdcall * FUNCTPTR ) ( HANDLE );
   static HINSTANCE hInstKernel = NULL;
   if ( !hInstKernel )
      hInstKernel = LoadLibrary( _T( "kernel32.dll" ) );
   static FUNCTPTR FunctPtr = NULL;
   if ( !FunctPtr )
      FunctPtr = ( FUNCTPTR ) GetProcAddress( hInstKernel, "CancelIo" );
   if ( FunctPtr )
      FunctPtr( thePortHandle );
}
//---------------------------------------------------------------------------
bool COMPort::ComThreadExecute()
{
   GJV_thread::setThreadName( "COM Port thread" );
   //   DWORD start_tc = GetTickCount();
   while ( !ComTerminated )
   {
      // This thread seems to need a message loop...
      // or we stick waiting for things to happen
      MSG msg;
      while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessage( &msg );
      }

      int readSize = 16;
      unsigned char c[ 30 ];
      DWORD charsRead = 0;


      /*
      if ( ! ReadFile ( PortHandle
                        , c
                        , readSize
                        , &charsRead
                        , NULL
                      )
         )
      */
      ResetEvent( ReadOverlapped.hEvent );
      BOOL retRead = ReadFile( thePortHandle, c, readSize, &charsRead, &ReadOverlapped );

      if ( retRead )
      {
         // read completed immediately - carry on
      }
      else
      {
         DWORD ret = GetLastError();
         if ( GetLastError() != ERROR_IO_PENDING )
         {
            // Error in communications; report it.
            trace( std::string( "COMPort: read failed - " ) + lastError( ret ) );
            WaitSpinning( 1000 );
            continue;
         }
         else
         { //read delayed

            DWORD dwRes;
            HANDLE waitEvents[ 2 ];
            waitEvents[ 0 ] = ReadOverlapped.hEvent;
            waitEvents[ 1 ] = CancelOverlappedhEvent;

            dwRes = WaitForMultipleObjects( 2, waitEvents, false, 5000 );   // we should always get something quicker than that...
            switch ( dwRes )
            {
                  // Read completed.
               case WAIT_OBJECT_0:
                  if ( !GetOverlappedResult( thePortHandle, &ReadOverlapped, &charsRead, FALSE ) )
                  {
                     // Error in communications; report it.
                     trace( "Comms error reading async characters" );
                     charsRead = 0;
                  }
                  break;

               case WAIT_OBJECT_0 + 1:
                  trace( "Cancel event signalled" );
                  CancelIO();
                  charsRead = 0;
                  break;

               case WAIT_TIMEOUT:
                  {
                     trace( "Timeout reading async characters" );
                     CancelIO();
                     charsRead = 0;
                     break;
                  }

               default:
                  // Error in the WaitForSingleObject; abort.
                  trace ( std::string( "Error waiting for async event - " ) + lastError() );
                  // This indicates a problem with the OVERLAPPED structure's
                  // event handle.
                  charsRead = 0;
                  break;
            }
         }
      }
      /*
      if ( ! ReadFile ( PortHandle
                        , c
                        , readSize
                        , &charsRead
                        , NULL
                      )
         )
      {
         proxyLogInfo ( String("COMPort: read failed - ") + LastError() );
         WaitSpinning(1000);
         continue;
      } // endif
      */

      if ( charsRead )
      {
         // add to read buffer
         // BUT this is not what we want!
         //trace( "          Read... " + PrintHexDump( charsRead, ( PBYTE ) c )) ;
         for ( unsigned int i = 0; i < charsRead; i++ )
            rxBuff += c[ i ];

         processRxBuff();
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
bool COMPort::StartComThread()
{
   ComTerminated = false;
   RxGuard g;              // in case it is being started from two threads at once
   ComTerminated = false;
   if ( !ComThread )
   {
      unsigned int tid;

      ComThread = ( HANDLE ) _beginthreadex(
                     0,                           // security
                     0,                           // stacksize
                     &tComThreadExecute,          // start
                     this,                           // arg
                     ABOVE_NORMAL_PRIORITY_CLASS,         // create flags
                     &tid );                   // thread id
   }
   return ( ComThread != 0 );
}


