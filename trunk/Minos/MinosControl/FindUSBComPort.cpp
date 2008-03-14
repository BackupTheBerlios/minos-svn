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

#include "FindUSBComPort.h"
#include <vcl/registry.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)


bool FindUSBComPort( String key, String &port )
{
   TRegistry * Reg = new TRegistry;
   TStringList *keys = new TStringList;
   Reg->RootKey = HKEY_LOCAL_MACHINE;

   bool livePortFound = false;
   try
   {
      if ( Reg->OpenKeyReadOnly( key ) )
      {
         String path = Reg->CurrentPath;
         Reg->GetKeyNames( keys );
         for ( int i = 0; i < keys->Count && !livePortFound; i++ )
         {
            String key = keys->Strings[ i ];
            if ( Reg->OpenKeyReadOnly( key ) )
            {
               try
               {
                  if ( Reg->OpenKeyReadOnly( "Device Parameters" ) )
                  {
                     if ( Reg->ValueExists( "PortName" ) )
                     {
                        String COMPort = Reg->ReadString( "PortName" );
                        port = "\\\\.\\" + COMPort;

                        //check to see if device is connected via device entry in serialcomm list.

                        Reg->CloseKey();
                        if ( Reg->OpenKeyReadOnly( "Hardware\\DeviceMap\\SerialComm" ) )
                        {
                           Reg->GetValueNames( keys );
                           for ( int j = 0; j < keys->Count && !livePortFound; j++ )
                           {
                              String value = keys->Strings[ j ];
                              String data = Reg->ReadString( value );

                              // look for data value being COMx
                              // and if so look for value(name) starting "\\Device\\USBSER"

                              // If this is  so, then we have a live one...

                              if ( data == COMPort && value.Pos( "\\Device\\USBSER" ) > 0 )
                                 livePortFound = true;
                           }
                           Reg->CloseKey();
                        }
                     }
                  }
               }
               catch ( ERegistryException & )
               {}

               Reg->CloseKey();
               if ( !Reg->OpenKeyReadOnly( path ) )
                  return false;
            }
         }
      }
   }
   __finally
   {
      delete keys;
      delete Reg;
   }
   return livePortFound;
}
