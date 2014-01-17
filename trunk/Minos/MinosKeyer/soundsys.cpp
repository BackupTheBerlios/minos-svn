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

#include <process.h>
#include "keyctrl.h"
#include "keyconf.h"
#include "vkmixer.h"
#include "sbdriver.h"
#include "soundsys.h"
#include "keyers.h"
#include "writerthread.h"
#include "keyerlog.h"
#ifndef _INC_MMSYSTEM
#error this should already be included...
// but it is useful to be able to open it easily
#include <mmsystem.h>
#endif

extern int curBlockLength;
// hope this is the one just output...
#define pttype INT16
// VU meter takes 65536 as 100%
#define shortmult 2
//==============================================================================
static CRITICAL_SECTION waveCriticalSection;
//==============================================================================

/*static*/
SoundSystem *SoundSystem::createSoundSystem()
{
   return new WindowsSoundSystem();
}
//==============================================================================
SoundSystem::SoundSystem() : cfgrate( -1 )
{}
SoundSystem::~SoundSystem()
{}


//==============================================================================
static void CALLBACK waveOutProc(
   HWAVEOUT /*hWaveOut*/,
   UINT uMsg,
   DWORD dwInstance,        // "this" for soundsystem object
   DWORD /*dwParam1*/,          // wParam - pointer to WAVEHDR for completed buffer
   DWORD /*dwParam2*/       // lParam - reserved
)
{
   if ( sbintrlog )
   {
      char * type = 0;
      if ( uMsg == WOM_DONE )
         type = "WOM_DONE";
      else
         if ( uMsg == WOM_OPEN )
            type = "WOM_OPEN";
         else
            if ( uMsg == WOM_CLOSE )
               type = "WOM_CLOSE";
            else
               type = "<unknown>";

      trace( "waveOutProc : " + makeStr( type ) );
   }
   /*
   * ignore calls that occur due to opening and closing the
   * device.
   */
   if ( uMsg != WOM_DONE )
      return ;
   WindowsSoundSystem *wss = ( WindowsSoundSystem * ) dwInstance;

   EnterCriticalSection( &waveCriticalSection );
   ( wss->waveOutFreeBlockCount ) ++;
   LeaveCriticalSection( &waveCriticalSection );
   SetEvent( wss->hWaveOutEvent );
}
//==============================================================================
static void CALLBACK waveInProc(
   HWAVEIN /*hWaveIn*/,
   UINT uMsg,
   DWORD dwInstance,        // "this" for soundsystem object
   DWORD dwParam1,          // wParam - pointer to WAVEHDR for completed buffer
   DWORD /*dwParam2*/       // lParam - reserved
)
{
   if ( sbintrlog )
   {
      char * type = 0;
      if ( uMsg == WIM_DATA )
         type = "WIM_DATA";
      else
         if ( uMsg == WIM_OPEN )
            type = "WIM_OPEN";
         else
            if ( uMsg == WIM_CLOSE )
               type = "WIM_CLOSE";
            else
               type = "<unknown>";

      WAVEHDR *current = ( WAVEHDR * ) dwParam1;
      if ( current && uMsg == WIM_DATA )
      {
         trace( "waveInProc : " + makeStr( type ) + " " + makeStr( current->dwFlags ) + " " + makeStr( current->dwBytesRecorded ) );
      }
      else
      {
         trace( "waveInProc : " + makeStr( type ) );
      }
   }
   /*
   * ignore calls that occur due to opening and closing the
   * device.
   */
   if ( uMsg != WIM_DATA )
      return ;
   WindowsSoundSystem *wss = ( WindowsSoundSystem * ) dwInstance;

   EnterCriticalSection( &waveCriticalSection );
   // NB this can happen too quickly...
   ( wss->waveInFreeBlockCount ) ++;
   ( wss->waveInEventCount ) ++;
   LeaveCriticalSection( &waveCriticalSection );
   SetEvent( wss->hWaveInEvent );
}
//==============================================================================
WindowsSoundSystem::WindowsSoundSystem() : SoundSystem(),
      sampleRate( 0 ), hWaveIn( 0 ), hInputThread( 0 ), hWaveOut( 0 ), hOutputThread( 0 ),
      waveInActive( false ), waveOutActive( false ), wt( 0 )
{
   InitializeCriticalSection( &waveCriticalSection );

   allocateBlocks( &waveInBlocks, MAX_BLOCK_LENGTH, BLOCK_COUNT );
   waveInFreeBlockCount = BLOCK_COUNT;
   waveInCurrentBlock = 0;
   waveInEventCount = 0;

   allocateBlocks( &waveOutBlocks, MAX_BLOCK_LENGTH, BLOCK_COUNT );
   waveOutFreeBlockCount = BLOCK_COUNT;
   waveOutCurrentBlock = 0;

}
WindowsSoundSystem::~WindowsSoundSystem()
{
   stopDMA();
   freeBlocks();
   DeleteCriticalSection( &waveCriticalSection );
}
int WindowsSoundSystem::setRate()
{
   int rate = cfgrate;
   if ( rate < 1 )                   // as we have no way of setting it this is always true...
      rate = 22050;
   rate = ( rate > 44100 ) ? 44100 : rate;
   rate = ( rate < 8000 ) ? 8000 : rate;

   sampleRate = rate;
   return rate;

}

void WindowsSoundSystem::stopDMA()
{
   trace( "WindowsSoundSystem::stopDMA" );
   done = true;
   samplesremaining = 0;
   samplesOutput = samples;

   if ( hWaveOut && waveOutActive )
   {
      waveOutActive = false;
      waveOutReset( hWaveOut );
   }
   if ( hWaveIn && waveInActive )
   {
      if ( sblog )
      {
         trace( "waveInReset in stopDMA" );
      }
      waveInActive = false;
      waveInReset( hWaveIn );
      if ( wt )
      {
         wt->Terminate();
         wt = 0;
      }
      if ( sblog )
      {
         trace( "waveInReset in stopDMA finished" );
      }
   }

   if ( sbDriver::getSbDriver() ->WinVUInCallback )
      sbDriver::getSbDriver() ->WinVUInCallback( 0 );
   if ( sbDriver::getSbDriver() ->WinVUOutCallback )
      sbDriver::getSbDriver() ->WinVUOutCallback( 0 );
}
//=============================================================================
void WindowsSoundSystem::allocateBlocks( WAVEHDR** waveBlocks, int size, int count )
{
   unsigned char * buffer;
   int i;
   WAVEHDR* blocks;
   DWORD totalBufferSize = ( size * 2 + sizeof( WAVEHDR ) ) * count;

   // allocate memory for the entire set in one go

   if ( ( buffer = ( unsigned char* ) HeapAlloc(
                      GetProcessHeap(),
                      HEAP_ZERO_MEMORY,
                      totalBufferSize
                   ) ) == NULL )
   {
      ExitProcess( 1 );
   }

   // and set up the pointers to each bit

   blocks = ( WAVEHDR* ) buffer;
   buffer += sizeof( WAVEHDR ) * count;

   for ( i = 0; i < count; i++ )
   {
      blocks[ i ].dwBufferLength = size * 2;
      blocks[ i ].lpData = buffer;
      buffer += size * 2;
   }
   *waveBlocks = blocks;
}
//=============================================================================
void WindowsSoundSystem::freeBlocks()
{
   /*
   * and this is why allocateBlocks works the way it does
   */
   HeapFree( GetProcessHeap(), 0, waveInBlocks );
   HeapFree( GetProcessHeap(), 0, waveOutBlocks );
}
//=============================================================================
void WindowsSoundSystem::InputThread()
{
   hWaveInEvent = CreateEvent(
                     NULL,     // no security attributes
                     FALSE,     // auto reset event
                     FALSE,      // not-signalled
                     NULL );  // no name
   hInputThreadCloseEvent = CreateEvent(
                               NULL,     // no security attributes
                               TRUE,     // manual reset event
                               FALSE,      // not-signalled
                               NULL );  // no name

   HANDLE hEvents[ 5 ] = {NULL, NULL, NULL, NULL, NULL};
   hEvents[ 0 ] = hInputThreadCloseEvent;
   hEvents[ 1 ] = hWaveInEvent;

   // used to handle input buffers as they fill
   while ( 1 )
   {
      // Wait for request to close or for a read thread to terminate.
      DWORD dwWait = WaitForMultipleObjects( 2, hEvents, FALSE, 1000 );

      if ( dwWait == WAIT_FAILED )
      {
         if ( sblog )
         {
            std::string le = lastError();
            trace( "WaitForMultipleObjects failed " + le );
         }

         CloseHandle( hInputThreadCloseEvent );
         if ( wt )
         {
            wt->Terminate();
            wt = 0;
         }

         return ;
      }
      if ( dwWait == WAIT_OBJECT_0 )     // Close down requested
      {
         CloseHandle( hInputThreadCloseEvent );
         if ( wt )
         {
            wt->Terminate();
            wt = 0;
         }
         return ;
      }
      if ( dwWait == WAIT_OBJECT_0 + 1 )     // Wave event
      {
         // here we have to empty buffers, etc
         disableInterrupts guard;
         KeyerAction * sba = KeyerAction::getCurrentAction();
         if ( sba /*&& samplesremaining > 0*/ )
            sba->interruptOK();	// so as we do not time it out immediately

         readAudio();

      }
   }

}
/*static */unsigned __stdcall WindowsSoundSystem::InputThread(    LPVOID lpThreadParameter )
{
   GJV_thread::setThreadName( "input" );
   WindowsSoundSystem * wss = ( WindowsSoundSystem * ) lpThreadParameter;
   wss->InputThread();
   return 0;
}
void WindowsSoundSystem::OutputThread()
{
   hWaveOutEvent = CreateEvent(
                      NULL,     // no security attributes
                      FALSE,     // auto reset event
                      FALSE,      // not-signalled
                      NULL );  // no name

   hOutputThreadCloseEvent = CreateEvent(
                                NULL,     // no security attributes
                                TRUE,     // manual reset event
                                FALSE,      // not-signalled
                                NULL );  // no name

   HANDLE hEvents[ 5 ] = {NULL, NULL, NULL, NULL, NULL};
   hEvents[ 0 ] = hOutputThreadCloseEvent;
   hEvents[ 1 ] = hWaveOutEvent;

   // used to fill output buffers as they are required

   while ( 1 )
   {
      // Wait for request to close or for a read thread to terminate.
      DWORD dwWait = WaitForMultipleObjects( 2, hEvents, FALSE, 1000 );

      if ( dwWait == WAIT_FAILED )
      {
         if ( sblog )
         {
            std::string le = lastError();
            trace( "WaitForMultipleObjects failed " + le );
         }

         CloseHandle( hOutputThreadCloseEvent );
         return ;
      }
      if ( dwWait == WAIT_OBJECT_0 )     // Close down requested
      {
         CloseHandle( hOutputThreadCloseEvent );
         return ;
      }
      if ( dwWait == WAIT_OBJECT_0 + 1 )     // Wave event
      {
         // here we have to fill buffers, etc
         disableInterrupts guard;

         KeyerAction * sba = KeyerAction::getCurrentAction();
         if ( sba && ( samplesOutput <= samples || sba->pipStartDelaySamples || sba->tailWithPip ) )
         {
            // once all finished, this leaves a bit of time for the buffers to be
            // returned
            sba->interruptOK();	// so as we do not time it out immediately
         }
         bool wadone = false;
         if ( samplesOutput < samples )
         {
            wadone = ( writeAudio() > 0 );
            samplesOutput += curBlockLength;
         }

         if ( !done && !wadone )
         {
            if ( sba && sba->pipStartDelaySamples > 0 )
            {
               if ( sblog )
               {
                  trace( "pipStartDelaySamples = " + makeStr( sba->pipStartDelaySamples ) );
               }
               sba->pipStartDelaySamples -= writeAudio( sba->pipStartDelaySamples );
            }
            else
               if ( sba && sba->tailWithPip )
               {
                  // we need to initiate writing the pip
                  // But if another action is chained - don't!
                  KeyerAction * sbn = sba->getNextAction();
                  if ( !sbn )
                  {
                     now = 0;
                     samplesOutput = 0;
                     samples = sbDriver::getSbDriver() ->pipSamples;
                     samplesremaining = samples;
                     dataptr = sbDriver::getSbDriver() ->pipptr;

                     sba->tailWithPip = false;
                     if ( sblog )
                     {
                        trace( "tailWithPip samples = " + makeStr( samples ) );
                     }
                     writeAudio();
                     samplesOutput += curBlockLength;
                  }
               }
            if ( sba )
            {
               // We keep going until all buffers are returned...
               if ( waveOutFreeBlockCount == BLOCK_COUNT )
                  sba->queueFinished();
            }
         }
      }
   }

}
/*static */unsigned __stdcall WindowsSoundSystem::OutputThread(    LPVOID lpThreadParameter )
{
   GJV_thread::setThreadName( "output" );
   WindowsSoundSystem * wss = ( WindowsSoundSystem * ) lpThreadParameter;
   wss->OutputThread();

   return 0;
}
bool WindowsSoundSystem::startInput( std::string fname )
{
   wt = new WriterThread( true );
   wt->openFile( fname );
   wt->Resume();

   if ( !hWaveIn )
   {
      // warning WAVEFORMATEX settings imply the audio quality
      WAVEFORMATEX wfx;
      /*
      * set up the WAVEFORMATEX structure.
      */
      wfx.wFormatTag = WAVE_FORMAT_PCM;
      wfx.nChannels = 1; /* channels*/
      wfx.nSamplesPerSec = sampleRate; /* sample rate */
      wfx.wBitsPerSample = 16; /* sample size */

      wfx.nBlockAlign = ( wfx.wBitsPerSample * wfx.nChannels ) >> 3;
      wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

      wfx.cbSize = 0; /* size of _extra_ info */

	  MMRESULT res;
	  int vin = VKMixer::getInputVKMixerID();
      if ( ( res = waveInOpen(
					  &hWaveIn,
					  /*WAVE_MAPPER*/vin,
                      &wfx,
                      ( DWORD_PTR ) waveInProc,
                      ( DWORD_PTR ) this,              // user instance data
                      CALLBACK_FUNCTION
                   ) ) != MMSYSERR_NOERROR )
      {
         // should pass an error back here...
         trace( std::string( "WindowsSoundSystem::initialise - wavein open failed " ) + makeStr( res ) );
         return false;
      }

      if ( !hInputThread )
      {
         unsigned int dwInputThreadId;

         // NB that ABOVE_NORMAL is a Win2K invention
         hInputThread = ( HANDLE ) _beginthreadex( NULL,                    // security
                        0,                        // stacksize
                        InputThread,            // start
                        this,                   // arg
                        HIGH_PRIORITY_CLASS,       // createflags
                        &dwInputThreadId           // threadid
                                                 );
      }
   }
   return true;
}
bool WindowsSoundSystem::startOutput()
{
   if ( !hWaveOut )
   {
      // warning WAVEFORMATEX settings imply the audio quality
      WAVEFORMATEX wfx;
      /*
      * set up the WAVEFORMATEX structure.
      */
      wfx.wFormatTag = WAVE_FORMAT_PCM;
      wfx.nChannels = 1; /* channels*/
      wfx.nSamplesPerSec = sampleRate; /* sample rate */
      wfx.wBitsPerSample = 16; /* sample size */

      wfx.nBlockAlign = ( wfx.wBitsPerSample * wfx.nChannels ) >> 3;
      wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

      wfx.cbSize = 0; /* size of _extra_ info */

      MMRESULT res;
      if ( ( res = waveOutOpen(
                      &hWaveOut,
                      /*WAVE_MAPPER*/VKMixer::getOutputVKMixerID(),
                      &wfx,
                      ( DWORD_PTR ) waveOutProc,
                      ( DWORD_PTR ) this,              // user instance data
                      CALLBACK_FUNCTION
                   ) ) != MMSYSERR_NOERROR )
      {
         // should pass an error back here...
         trace( std::string( "WindowsSoundSystem::initialise - waveout open failed " ) + makeStr( res ) );
         return false;
      }

      if ( !hOutputThread )
      {
         unsigned int dwOutputThreadId;
         hOutputThread = ( HANDLE ) _beginthreadex( NULL,                    // security
                         0,                        // stacksize
                         OutputThread,            // start
                         this,                   // arg
                         HIGH_PRIORITY_CLASS,                        // createflags
                         &dwOutputThreadId           // threadid
                                                  );
      }
   }
   return true;
}
bool WindowsSoundSystem::initialise( std::string &/*errmess*/ )
{
   return true;
}
void WindowsSoundSystem::terminate()
{
   if ( hInputThreadCloseEvent != NULL )
      SetEvent( hInputThreadCloseEvent );
   if ( hOutputThreadCloseEvent != NULL )
      SetEvent( hOutputThreadCloseEvent );

   HANDLE hEvents[ 5 ] = {NULL, NULL, NULL, NULL, NULL};
   hEvents[ 0 ] = hInputThread;
   hEvents[ 1 ] = hOutputThread;

   // Wait for request to close or for a read thread to terminate.
   /*DWORD dwWait = */
   WaitForMultipleObjects( 2, hEvents, TRUE, 1000 );

   // either 1 sec or all threads terminated...

   CloseHandle( hInputThread );
   CloseHandle( hOutputThread );
}
//=============================================================================
void WindowsSoundSystem::addNextInBuffer()
{
   WAVEHDR * current = &waveInBlocks[ waveInCurrentBlock ];
   /*
   * first make sure the header we're going to use is unprepared
   */
   if ( current->dwFlags & WHDR_PREPARED )
   {
      waveInUnprepareHeader( hWaveIn, current, sizeof( WAVEHDR ) );
   }

   if ( !done )
   {
      current->dwBufferLength = curBlockLength * 2;
      current->dwBytesRecorded = 0;

      waveInPrepareHeader( hWaveIn, current, sizeof( WAVEHDR ) );
      if ( waveInAddBuffer( hWaveIn, current, sizeof( WAVEHDR ) ) != MMSYSERR_NOERROR )
      {
         if ( sblog )
         {
            trace( "waveInAddBuffer failed " );
            return ;
         }
      }
      // Don't common up, save critical sections
      EnterCriticalSection( &waveCriticalSection );
      waveInFreeBlockCount--;
      waveInCurrentBlock++;
      waveInCurrentBlock %= BLOCK_COUNT;
      LeaveCriticalSection( &waveCriticalSection );
      return ;
   }
   else
   {
      EnterCriticalSection( &waveCriticalSection );
      waveInCurrentBlock++;
      waveInCurrentBlock %= BLOCK_COUNT;
      LeaveCriticalSection( &waveCriticalSection );
   }
}
//=============================================================================
void WindowsSoundSystem::readAudio()
{
   pttype maxvol = 0;

   while ( waveInEventCount > 0 )
   {
      WAVEHDR * current = &waveInBlocks[ waveInCurrentBlock ];
      // First we need to grab the collected data
      if ( current->dwFlags & WHDR_PREPARED )
      {
         if ( current->dwBytesRecorded )
         {

            // copy to disk write buffer, release writer thread.
            // If writer is getting too far behind - PANIC!

            // If we are terminated then there might be no thread variable
            if ( wt )
            {
               RecBuffer & r = wt->bufflist[ wt->RecordBlock % NBUFFS ];
               r.reset();
               r.RecBlock = wt->RecordBlock;

               memcpy( r.buff, current->lpData, current->dwBytesRecorded );
               r.Size = current->dwBytesRecorded;
               r.filled = true;

               wt->RecordBlock++;
               SetEvent( wt->ReleaseWriteEvent );
            }

            pttype *q = ( pttype * ) ( current->lpData );

            // determine max for VU meter
            for ( unsigned int i = 0; i < current->dwBytesRecorded / 2; i++ )
            {
               int sample = abs( *q++ );
               if ( sample > maxvol )
                  maxvol = sample;
            }
         }
      }
      addNextInBuffer();    // also increments waveInCurrentBlock

      waveInEventCount--;
   }

   EnterCriticalSection( &waveCriticalSection );
   if ( waveInFreeBlockCount == BLOCK_COUNT )
   {
      disableInterrupts guard;
      KeyerAction * sba = KeyerAction::getCurrentAction();
      if ( sba )
      {
         if ( sblog )
         {
            trace( "All buffers now returned" );
         }
         sba->queueFinished();
      }
   }
   LeaveCriticalSection( &waveCriticalSection );
   sbDriver::getSbDriver() ->WinVUInCallback( maxvol * shortmult );
}
//=============================================================================
int WindowsSoundSystem::writeAudio( int deadSamples )
{
   int samplesWritten = 0;
   WAVEHDR * current = &waveOutBlocks[ waveOutCurrentBlock ];
   /*
   * first make sure the header we're going to use is unprepared
   */
   if ( current->dwFlags & WHDR_PREPARED )
   {
      pttype * q = ( pttype * ) ( current->lpData );
      pttype maxvol = 0;

      // determine max for VU meter
      if ( !deadSamples )
      {
         for ( unsigned int i = 0; i < current->dwBufferLength / 2; i++ )
         {
            int sample = abs( *q++ );
            if ( sample > maxvol )
               maxvol = sample;
         }
      }
      else
         maxvol = 0;
      if ( !done )
         sbDriver::getSbDriver() ->WinVUOutCallback( maxvol * shortmult );

      waveOutUnprepareHeader( hWaveOut, current, sizeof( WAVEHDR ) );
   }
   else
   {
      sbDriver::getSbDriver() ->WinVUOutCallback( 0 );
   }
   memset( current->lpData, 0, current->dwBufferLength ); // clear buffer down
   if ( samplesremaining > 0 )
   {

      if ( samplesremaining < curBlockLength )
      {
         current->dwBufferLength = samplesremaining * 2;
      }
      else
      {
         current->dwBufferLength = curBlockLength * 2;
      }
      memcpy( current->lpData, ( ( char * ) dataptr ) + now, current->dwBufferLength );
      samplesremaining -= current->dwBufferLength / 2;
      now += current->dwBufferLength;
   }
   else
   {
      if ( deadSamples && deadSamples < curBlockLength )
      {
         current->dwBufferLength = deadSamples * 2;
      }
      else
         if ( deadSamples )
         {
            current->dwBufferLength = curBlockLength * 2;
         }
         else
         {
            return 0;
         }
   }
   samplesWritten = current->dwBufferLength / 2;
   waveOutPrepareHeader( hWaveOut, current, sizeof( WAVEHDR ) );
   waveOutWrite( hWaveOut, current, sizeof( WAVEHDR ) );

   EnterCriticalSection( &waveCriticalSection );
   waveOutFreeBlockCount--;
   waveOutCurrentBlock++;
   waveOutCurrentBlock %= BLOCK_COUNT;
   LeaveCriticalSection( &waveCriticalSection );

   return samplesWritten;
}
bool WindowsSoundSystem::startDMA( bool play, const std::string &fname )
{
   // start input / output

   if ( play )
   {
      if ( sblog )
      {
         trace( "Starting output" );
      }
      if ( !startOutput() )
      {
         if ( sblog )
         {
            trace( "Start output failed" );
         }
         return false;
      }
      if ( sblog )
      {
         trace( "Start output OK" );
      }
      // and now we have to start writing the file to the wave
      // buffers
      waveOutActive = true;
      waveOutFreeBlockCount = BLOCK_COUNT;
      waveOutCurrentBlock = 0;
      samplesOutput = 0;

      for ( int i = 0; i < BLOCK_COUNT; i++ )
         writeAudio();
      // and the output thread should pick it up from here...
      if ( sblog )
      {
         trace( "All buffers written" );
      }
   }
   else
   {

      if ( !startInput( fname ) )
         return false;
      // and now we have to start giving buffers to the system
      waveInFreeBlockCount = BLOCK_COUNT;
      waveInCurrentBlock = 0;
      samplesInput = 0;

      for ( int i = 0; i < BLOCK_COUNT; i++ )
         addNextInBuffer();

      waveInActive = true;
      if ( waveInStart( hWaveIn ) != MMSYSERR_NOERROR )
      {
         waveInActive = false;
         if ( sblog )
         {
            trace( "waveInStart failed " );
         }
         return false;
      }
   }
   return true;
}

