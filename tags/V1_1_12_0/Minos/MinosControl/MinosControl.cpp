/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#pragma hdrstop
#include "GJVThreads.h" 
//---------------------------------------------------------------------------
USEFORM("K8055Container.cpp", DMK8055);
USEFORM("MinosControlMain.cpp", MinosControlForm);
USEFORM("WindowsMonitor.cpp", WindowsMonitorForm);
USEFORM("UBWContainer.cpp", UBWDM);
USEFORM("HidControl.cpp", HidControlFM);
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
   GJV_thread::setThreadName( "Main" );
   try
   {
      Application->Initialize();
      Application->CreateForm(__classid(TMinosControlForm), &MinosControlForm);
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
