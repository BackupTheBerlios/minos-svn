/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef WriterThreadH
#define WriterThreadH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "riff.h" 
//----------------------------------------------------------------------------
class RecBuffer
{
   public:
      int RecBlock;
      int WriteBlock;

      char buff[ 40000 ]; // should be same size as sound buffers
      int Size;
      bool filled;
      RecBuffer() : Size( 0 ), filled( false ), RecBlock( -1 ), WriteBlock( -1 )
      {}
      void reset()
      {
         RecBlock = -1;
         WriteBlock = -1;
         Size = 0;
         filled = false;
      }
};
#define NBUFFS 20 
//---------------------------------------------------------------------------
class WriterThread : public TThread
{
   private:
      void __fastcall Kill1( void );
      void __fastcall Kill2( void );
      void __fastcall Kill3( void );
   protected:
      void __fastcall Execute();
      WaveFile outWave;
   public:
      __fastcall WriterThread( bool CreateSuspended );
      bool __fastcall openFile( std::string );
      RecBuffer bufflist[ NBUFFS ]; // same number as sampler  ??

      HANDLE ReleaseWriteEvent;

      int RecordBlock;
      int WriteBlock;
};
//---------------------------------------------------------------------------
#endif
