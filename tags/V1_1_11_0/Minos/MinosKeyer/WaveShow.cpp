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
#include "LogEvents.h"

#include "keyctrl.h"
#include "vkmixer.h"
//#include "MixerNodes.h"
#include "keyercontrol.h" 
//#include "MixerForm.h"
#include "KeyerAbout.h"
#include "KeyerRPCServer.h"
#include "WaveShow.h"
#include "riff.h"
#include "sbdriver.h"
#include "keyers.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWaveShowForm *WaveShowForm = 0;
//---------------------------------------------------------------------------
__fastcall TWaveShowForm::TWaveShowForm( TComponent* Owner )
      : TForm( Owner )
{}
void TWaveShowForm::setSamples( int fno )
{
   char buff[ 20 ];
   sprintf( buff, "CQF%d.WAV", fno );
   WaveFile wavin;
   wavin.OpenForRead( buff );

   if ( sample )
      delete [] sample;

   sbDriver *sb = sbDriver::getSbDriver();

   int n = wavin.NumSamples();
   int rate = wavin.SamplingRate();
   int BitsPerSample = wavin.BitsPerSample();
   int NumChannels = wavin.NumChannels();

   if ( rate != 22050 || BitsPerSample != 16 || NumChannels != 1 )
   {
      n = 0;
   }
   int pipStartDelaySamples = ( currentKeyer->kconf.pipStartDelay * rate ) / 1000;
   sample = new INT16 [ n + sb->pipSamples ];
   memset( sample, 0, ( n + pipStartDelaySamples + sb->pipSamples ) * sizeof( INT16 ) );
   if ( wavin.ReadData( sample, n ) != DDC_SUCCESS )
   {
      Close();
      return ;
   }
   if ( sb->pipSamples )
   {
      memcpy( &sample[ n + pipStartDelaySamples ], sb->pipptr, sb->pipSamples * 2 );
   }
   Scale = 1;
   WavWidth = n + pipStartDelaySamples + sb->pipSamples;

   HScrollBar->Min = 0;
   HScrollBar->Max = WavWidth - WavWidth / Scale;
   HScrollBar->Position = 0;
   HScrollBar->PageSize = WavPanel->Width;
   HScrollBar->SmallChange = WavPanel->Width / 10;
   HScrollBar->LargeChange = WavPanel->Width * 7 / 10;

   VScrollBar->Min = 1;
   VScrollBar->Max = 1000;
   VScrollBar->Position = 1;
   VScrollBar->PageSize = 100;
   VScrollBar->SmallChange = 10;
   VScrollBar->LargeChange = 100;

   WavPanel->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::WavPanelPaint( TObject */*Sender*/ )
{
   WavPanel->Canvas->Pen->Style = psSolid;
   WavPanel->Canvas->Pen->Color = clBlack;
   WavPanel->Canvas->MoveTo( 0, WavPanel->Height / 2 );
   WavPanel->Canvas->LineTo( WavPanel->Width, WavPanel->Height / 2 );

   if ( sample )
   {
      WavPanel->Canvas->Pen->Style = psSolid;
      WavPanel->Canvas->Pen->Mode = pmBlack;

      double scaleCorrection = ( WavWidth ) / ( WavPanel->Width * Scale );  // elements/pixel

      if ( scaleCorrection == 0 )
         scaleCorrection = 1;

      // We need to pre-calc and then paint on demand from a pre-built display array
      // Still a nasty vertical offset - I hope an artifact!

      // This is good enough for now - after NFD go 16 bit and 22 KHz and see if it improves

      for ( int i = 0; i < WavPanel->Width; i++ )
      {
         // where are we in the sample?
         int spos = Position + i * scaleCorrection;
         int epos = Position + ( i + 1 ) * scaleCorrection;

         if ( spos >= WavWidth )
         {
            break;
         }
         epos = std::min( epos, WavWidth );

         short maxpix = 0;
         short minpix = 0;

         if ( epos <= spos )
         {
            maxpix = std::max( maxpix, sample[ spos ] );
            minpix = std::min( minpix, sample[ spos ] );
         }
         else
         {
            for ( int j = spos; j < epos; j++ )
            {
               maxpix = std::max( maxpix, sample[ j ] );
               minpix = std::min( minpix, sample[ j ] );
            }
         }
         int pixMax = MulDiv( maxpix, WavPanel->Height / 2, 32767 ) + WavPanel->Height / 2;
         int pixMin = MulDiv( minpix, WavPanel->Height / 2, 32767 ) + WavPanel->Height / 2;
         // need to pick abs greatest and just do the lineto, then we get a nicer picture?
         WavPanel->Canvas->MoveTo( i, pixMin );
         WavPanel->Canvas->LineTo( i, pixMax );
      }
   }
   WavPanel->Canvas->Pen->Style = psDash;
   WavPanel->Canvas->Pen->Mode = pmCopy;
   WavPanel->Canvas->Pen->Color = clRed;

   WavPanel->Canvas->MoveTo( 0, WavPanel->Height / 6 );
   WavPanel->Canvas->LineTo( WavPanel->Width, WavPanel->Height / 6 );

   WavPanel->Canvas->MoveTo( 0, 5 * WavPanel->Height / 6 );
   WavPanel->Canvas->LineTo( WavPanel->Width, 5 * WavPanel->Height / 6 );
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::HScrollBarScroll( TObject */*Sender*/,
      TScrollCode /*ScrollCode*/, int &ScrollPos )
{
   HScrollBar->Position = ScrollPos;
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::HScrollBarChange( TObject */*Sender*/ )
{
   Position = HScrollBar->Position;
   WavPanel->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::VScrollBarScroll( TObject */*Sender*/,
      TScrollCode /*ScrollCode*/, int &ScrollPos )
{
   VScrollBar->Position = ScrollPos;
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::VScrollBarChange( TObject */*Sender*/ )
{
   Scale = VScrollBar->Position;
   HScrollBar->Max = WavWidth - WavWidth / Scale;
   HScrollBar->Position = Position;
   WavPanel->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::FormDestroy( TObject */*Sender*/ )
{
   delete [] sample;
   sample = 0;
   KeyControlForm->closeShow();
}
//---------------------------------------------------------------------------
void __fastcall TWaveShowForm::FormClose( TObject */*Sender*/, TCloseAction &Action )
{
   Action = caFree;
}
//---------------------------------------------------------------------------

