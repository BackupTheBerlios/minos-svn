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
//---------------------------------------------------------------------------
USEFORM( "KeyerAbout.cpp", AboutBox );
USEFORM( "keyercontrol.cpp", KeyControlForm );
USEFORM( "WaveShow.cpp", WaveShowForm );
USEFORM( "WindowsMonitor.cpp", WindowsMonitorForm );
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
   GJV_thread::setThreadName( "Main" );
   try
   {
      Application->Initialize();
      Application->CreateForm( __classid( TKeyControlForm ), &KeyControlForm );
      Application->Run();
   }
   catch ( Exception & exception )
   {
      Application->ShowException( &exception );
   }
   catch ( ... )
   {
      try
      {
         throw Exception( "" );
      }
      catch ( Exception & exception )
      {
         Application->ShowException( &exception );
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
