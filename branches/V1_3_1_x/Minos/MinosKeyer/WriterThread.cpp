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

#include "WriterThread.h"
#include "soundsys.h"

#pragma package(smart_init) 
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
RecBuffer bufflist[ NBUFFS ]; // same number as sampler

__fastcall WriterThread::WriterThread( bool CreateSuspended )
      : TThread( CreateSuspended ), RecordBlock( 0 ), WriteBlock( 0 )
{
   ReleaseWriteEvent = CreateEvent( NULL, FALSE, FALSE, NULL );  // anonymous private event
   // auto reset, initially false
   ResetEvent( ReleaseWriteEvent );
   FreeOnTerminate = true;
   for ( int i = 0; i < NBUFFS; i++ )
      bufflist[ i ].reset();
}
//---------------------------------------------------------------------------
bool __fastcall WriterThread::openFile( std::string fn )
{
   if ( outWave.OpenForWrite( fn.c_str(), 22050, 16, 1 ) == DDC_SUCCESS )
      return true;

   return false;
}
//---------------------------------------------------------------------------
void __fastcall WriterThread::Kill1( void )
{
   ShowMessage( "Mismatched blocks - Overrun?" );
}
//---------------------------------------------------------------------------
void __fastcall WriterThread::Kill2( void )
{
   ShowMessage( "Buffer not filled - Overrun?" );
}
//---------------------------------------------------------------------------
void __fastcall WriterThread::Kill3( void )
{
   ShowMessage( "Write to disk failed" );
}
//---------------------------------------------------------------------------
void __fastcall WriterThread::Execute()
{
   //---- Place thread code here ----
   // sit on WaitForSingleObject, waiting for recorder buffers to be written
   // When write done update write block number

   GJV_thread::setThreadName( "WAV writer" );
   try
   {
      while ( !Terminated )
      {
         if ( WaitForSingleObject( ReleaseWriteEvent, 1000 ) == WAIT_OBJECT_0 )
         {
            // Our event signalled
            while ( WriteBlock < RecordBlock )
            {
               RecBuffer & r = bufflist[ WriteBlock % NBUFFS ];

               if ( r.filled )
               {
                  if ( r.RecBlock != WriteBlock )
                  {
                     Synchronize( &Kill1 );
                     return ;
                  }

#define pttype INT16
                  pttype *q = ( pttype * ) ( r.buff );
                  DDCRET ret = outWave.WriteData ( q, r.Size / 2 );   // size is samples
                  if ( ret != DDC_SUCCESS )
                  {
                     Synchronize( &Kill3 );
                     return ;
                  }
               }
               else
               {
                  Synchronize( &Kill2 );
                  return ;
               }

               r.reset();
               WriteBlock++;
            }
         }
      }
   }
   __finally
   {
      outWave.Close();
   }
}
//---------------------------------------------------------------------------
