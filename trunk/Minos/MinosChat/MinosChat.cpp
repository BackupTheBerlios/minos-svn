//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "GJVThreads.h" 
//---------------------------------------------------------------------------
USEFORM("MinosChatMain.cpp", MinosChatForm);
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
   GJV_thread::setThreadName( "Main" );
   try
   {
      Application->Initialize();
      Application->CreateForm(__classid(TMinosChatForm), &MinosChatForm);
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
