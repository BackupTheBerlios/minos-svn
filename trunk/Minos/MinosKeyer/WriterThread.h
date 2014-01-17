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
