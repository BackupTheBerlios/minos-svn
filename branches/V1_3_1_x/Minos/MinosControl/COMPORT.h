/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
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


#ifndef ComportH
#define ComportH

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string> 
//-----------------------------------------------------------------------------
class COMPort
{
   public:

      enum Parity
      {
         None = 0
                , Odd
         , Even
         , Mark
         , Space
   };

      enum DataBits
      {
         db4 = 4
               , db5
         , db6
         , db7
         , db8
   };

      enum StopBits
      {
         sb1 = 0,
         sb15,
         sb2
   };

      enum BitRate
      {
         br110 = 110,
         br300 = 300,
         br600 = 600,
         br1200 = 1200,
         br2400 = 2400,
         br4800 = 4800,
         br9600 = 9600,
         br19200 = 19200,
         br38400 = 38400,
         br56000 = 56000,
         br57600 = 57600,
         br115200 = 115200,
         br256000 = 256000
   };
      // for function getModemSignals
      struct MSPack
      {
      unsigned char DTR :
         1;
      unsigned char RTS :
         1;
      unsigned char :
         2;
      unsigned char CTS :
         1;
      unsigned char DSR :
         1;
      unsigned char RI :
         1;
      unsigned char DCD :
         1;
      };

      COMPort ( );
      ~COMPort ();

      //      bool openPort ( const char * const portName );
      //      void close();

      // I/O operations
      /*
      int read ( unsigned char &ch );
      COMPort & write ( const char inChar );

      unsigned long read ( void *
                           , const unsigned long count
                         );

      unsigned long write ( const void *
                            , const unsigned long count
                          );

      */
      COMPort& setBitRate ( unsigned long Param );
      unsigned long bitRate () const;

      COMPort& setParity ( Parity Param );
      Parity parity () const;

      COMPort& setDataBits ( DataBits Param );
      DataBits dataBits () const;

      COMPort& setStopBits ( StopBits Param );
      StopBits stopBits () const;

      COMPort & setHandshaking ( bool inHandshaking = true );

      COMPort& setLineCharacteristics ( char * Param );

      unsigned long getMaximumBitRate () const;

      COMPort & setxONxOFF ( bool Param = true );
      bool isxONxOFF () const;

      MSPack getModemSignals () const;

      void setRTS( bool state );
      void setDTR( bool state );

      COMPort& setBlockingMode ( unsigned long inReadInterval = 0
                                 , unsigned long inReadMultiplyer = 0
                                                                    , unsigned long inReadConstant = 0
                               );

   protected:

   private:

      // disable copy constructor and assignment operator
      COMPort ( const COMPort & );
      COMPort& operator= ( const COMPort & );

      void getState () const;
      COMPort& setState ();

      HANDLE thePortHandle;
      LPDCB theDCB;

   private:
      HANDLE ComThread;
      OVERLAPPED ReadOverlapped;
      OVERLAPPED WriteOverlapped;
      HANDLE CancelOverlappedhEvent;
      bool StartComThread();
      bool ComTerminated;
      void CancelIO();
      bool OpenPort( const std::string &PortName );
      bool ClosePort();
   protected:
      std::string rxBuff;
   public:
      bool Initialise( const std::string &PortName );
      bool COMPortWrite ( const void *inBuffer, const unsigned long inBufSize, unsigned long &written );
      bool CloseDown();
      bool ComThreadExecute();
      virtual void processRxBuff() = 0;
}
; // End of COMPort class declaration
void WaitSpinning( unsigned int ms );

#endif
