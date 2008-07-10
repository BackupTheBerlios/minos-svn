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


#include "tinyxml.h"
#include "controlport.h"
#include "portconf.h"

#include <math.h>
#include "K8055Container.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init) 
#pragma resource "*.dfm"
TDMK8055 *DMK8055;
//---------------------------------------------------------------------------
__fastcall TDMK8055::TDMK8055( TComponent* Owner )
      : TForm( Owner ),
      theDev( 0 ), timed( false ), viewed( false ), deviceFound( false )
      , deviceNo( 0 )

{
   for ( int i = 0; i < 10; i++ )
      bufTx[ i ] = 0;
   for ( int i = 0; i < 10; i++ )
      bufRx[ i ] = 0;
   bufTx[ 7 ] = 4;
   bufTx[ 8 ] = 4;
}
__fastcall TDMK8055::~TDMK8055()
{
   for ( unsigned int i = 0; i < devList.size(); i++ )
   {
      theDev = devList[ i ];
      JvHidDeviceController1->CheckIn( theDev );
      delete theDev;
   }
   devList.clear();
}
//---------------------------------------------------------------------------
void __fastcall TDMK8055::JvHidDeviceController1DeviceChange(
   TObject */*Sender*/ )
{
   if ( devList.size() )
   {
      for ( unsigned int i = 0; i < devList.size(); i++ )
      {
         theDev = devList[ i ];
         delete theDev;
      }
      devList.clear();
   }

   JvHidDeviceController1->Enumerate();

   if ( devList.size() > 0 )
   {
      theDev = devList[ 0 ];
   }
}
//---------------------------------------------------------------------------
bool __fastcall TDMK8055::JvHidDeviceController1Enumerate(
   TJvHidDevice *HidDev, const int Idx )
{
   TJvHidDevice * Dev;
   if ( HidDev->Attributes.VendorID == 0x10CF &&
        ( HidDev->Attributes.ProductID == ( 0x5500 | deviceNo )    // we need to set devno...
        ) )
   {
      JvHidDeviceController1->CheckOutByIndex( Dev, Idx );
      devList.push_back( Dev );
   }
   return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TDMK8055::doRead()
{
   unsigned int toRead;
   unsigned int read;

   if ( theDev )
   {
      toRead = theDev->Caps.InputReportByteLength;    // tests give this as 9
      theDev->ReadFile( bufRx, toRead, read );
   }
}
//---------------------------------------------------------------------------

void __fastcall TDMK8055::doWrite()
{
   unsigned int written;
   unsigned int toWrite;
   if ( theDev )
   {
      bufTx[ 0 ] = 0;
      toWrite = 9;
      theDev = devList[ 0 ];
      written = 0;
      theDev->WriteFile( bufTx, toWrite, written );
   }
}
void __fastcall TDMK8055::setShareData ( unsigned int address, unsigned int data )
{
   shared[ address ] = data;
}
//---------------------------------------------------------------------------
void __fastcall TDMK8055::doAction( unsigned int k0, unsigned int k1, unsigned int k2 )
{
   switch ( k0 )
   {
      case 1:
         bufTx[ 2 ] = k1;  	//digital out
         bufTx[ 1 ] = 5;
         doWrite();
         break;

      case 2:
         bufTx[ 2 ] = bufTx[ 2 ] | k1 ;  	//set bit
         bufTx[ 1 ] = 5;
         doWrite();
         break;

      case 3:
         bufTx[ 2 ] = bufTx[ 2 ] & ~k1 ;  	//clear bit
         bufTx[ 1 ] = 5;
         doWrite();
         break;

      case 4:
         if ( k1 > 2 )
            k1 = 2;
         if ( k1 < 1 )
            k1 = 1;
         bufTx[ 2 + k1 ] = k2;
         bufTx[ 1 ] = 5;
         doWrite();		//analog 1 or 2
         break;

      case 5:
         bufTx[ 3 ] = k1;
         bufTx[ 4 ] = k2;
         bufTx[ 1 ] = 5;
         doWrite();		//analog 1 and 2
         break;

      case 6:
         doRead();
         for ( unsigned int i = 1; i <= 8; i++ )
            setShareData( 9 + i, bufRx[ i ] );
         break;

      case 7:
         bufTx[ 1 ] = 2 + k1;
         doWrite();           //reset counter
         break;

      case 8:
         bufTx[ 1 ] = k1;
         if ( k1 == 1 )
            bufTx[ 7 ] = k2;
         if ( k1 == 2 )
            bufTx[ 8 ] = k2;
         doWrite();           // debounce time
         break;

      case 9:
         setShareData( 0, 0 );
         //           Close();
         break;

   }
   setShareData( 0, 0 );
}

namespace K8055
{
   long openDevice( long CardNumber )
   {
      if ( !DMK8055 )
      {
         DMK8055 = new TDMK8055( 0 );
         if ( CardNumber > 3 )
            CardNumber = 3;
         if ( CardNumber < 0 )
            CardNumber = 0;
         DMK8055->deviceNo = CardNumber;
         memset( &DMK8055->shared, 0, sizeof( DMK8055->shared ) );
         DMK8055->Show();
      }
      //   DMK8055->JvHidDeviceController1DeviceChange(0);
      DMK8055->doAction( 6, 0, 0 );

      return DMK8055->shared[ 11 ];
   }
   void closeDevice()
   {
      // we don't actually do this... until the app is closed
   }
   long readAnalogChannel( long Channel )
   {
      DMK8055->doAction( 6, 0, 0 );
      return DMK8055->shared[ 11 + Channel ] & 0xff;

   }
   void readAllAnalog( long * Data1, long * Data2 )
   {
      DMK8055->doAction( 6, 0, 0 );
      *Data1 = DMK8055->shared[ 12 ];
      *Data2 = DMK8055->shared[ 13 ];
   }
   void outputAnalogChannel( long Channel, long Data )
   {
      DMK8055->doAction( 4, Channel, Data );
   }
   void outputAllAnalog( long Data1, long Data2 )
   {
      DMK8055->doAction( 5, Data1, Data2 );
   }
   void clearAnalogChannel( long Channel )
   {
      DMK8055->doAction( 4, Channel, 0 );
   }
   void clearAllAnalog()
   {
      DMK8055->doAction( 5, 0, 0 );
   }
   void setAnalogChannel( long Channel )
   {
      DMK8055->doAction( 4, Channel, 255 );
   }
   void setAllAnalog()
   {
      DMK8055->doAction( 5, 255, 255 );
   }
   void writeAllDigital( long Data )
   {
      DMK8055->doAction( 1, Data, 0 );
   }
   void clearDigitalChannel( long Channel )
   {
      int k = 1 << ( Channel - 1 );   // bit for channel - might be wrong
      DMK8055->doAction( 3, k, 0 );
   }
   void clearAllDigital()
   {
      DMK8055->doAction( 1, 0, 0 );
   }
   void setDigitalChannel( long Channel )
   {
      int k = 1 << ( Channel - 1 );   // bit for channel - might be wrong
      DMK8055->doAction( 2, k, 0 );
   }
   void setAllDigital()
   {
      DMK8055->doAction( 1, 255, 0 );
   }
   bool readDigitalChannel( long Channel )
   {
      DMK8055->doAction( 6, 0, 0 );
      unsigned int n1 = DMK8055->shared[ 10 ];
      unsigned int k = ( unsigned int ) ( ( n1 & 16 ) > 0 ) * 1 +
                       ( unsigned int ) ( ( n1 & 32 ) > 0 ) * 2 +
                       ( unsigned int ) ( ( n1 & 1 ) > 0 ) * 4 +
                       ( unsigned int ) ( ( n1 & 64 ) > 0 ) * 8 +
                       ( unsigned int ) ( ( n1 & 128 ) > 0 ) * 16;

      int ch = 1 << ( Channel - 1 );   // bit for channel - might be wrong
      return ( k & ch );

   }
   long readAllDigital()
   {
      DMK8055->doAction( 6, 0, 0 );
      unsigned int n1 = DMK8055->shared[ 10 ];
      unsigned int k = ( unsigned int ) ( ( n1 & 16 ) > 0 ) * 1 +
                       ( unsigned int ) ( ( n1 & 32 ) > 0 ) * 2 +
                       ( unsigned int ) ( ( n1 & 1 ) > 0 ) * 4 +
                       ( unsigned int ) ( ( n1 & 64 ) > 0 ) * 8 +
                       ( unsigned int ) ( ( n1 & 128 ) > 0 ) * 16;

      return k;
   }
   long readCounter( long CounterNr )
   {
      if ( CounterNr < 1 )
         CounterNr = 1;
      if ( CounterNr > 2 )
         CounterNr = 2;
      DMK8055->doAction( 6, 0, 0 );
      return ( DMK8055->shared[ 12 + CounterNr * 2 ] + 256 * DMK8055->shared[ 13 + CounterNr * 2 ] ) & 0xffff;
   }
   void resetCounter( long CounterNr )
   {
      if ( CounterNr < 1 )
         CounterNr = 1;
      if ( CounterNr > 2 )
         CounterNr = 2;
      DMK8055->doAction( 7, CounterNr, 0 );
   }
   void setCounterDebounceTime( long CounterNr, long DebounceTime )
   {
      if ( CounterNr < 1 )
         CounterNr = 1;
      if ( CounterNr > 2 )
         CounterNr = 2;
      unsigned int t1 = floor( 0.9 * 2.4541 * pow( DebounceTime, 0.5328 ) );
      if ( t1 < 1 )
         t1 = 1;
      if ( t1 > 255 )
         t1 = 255;
   }
}


void __fastcall TDMK8055::FormShow( TObject */*Sender*/ )
{
   Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TDMK8055::Timer1Timer( TObject */*Sender*/ )
{
   Timer1->Enabled = false;
   Hide();
}
//---------------------------------------------------------------------------
