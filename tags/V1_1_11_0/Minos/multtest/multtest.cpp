/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop 
//---------------------------------------------------------------------------
USEFORM( "multtestmain.cpp", Form1 );
USEFORM( "..\MinosDisplay\MMessageDialog.cpp", MessageDialog );
USEFORM( "..\MinosDisplay\MShowMessageDlg.cpp", MinosShowMessage );
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
   try
   {
      Application->Initialize();
      Application->CreateForm( __classid( TForm1 ), &Form1 );
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
