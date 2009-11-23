/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop 
//---------------------------------------------------------------------------
USEFORM( "MMain.cpp", MinosMainForm );
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
   try
   {
      Application->Initialize();
      Application->CreateForm( __classid( TMinosMainForm ), &MinosMainForm );
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
