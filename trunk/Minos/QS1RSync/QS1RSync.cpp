//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("IceSyncMain.cpp", QRigSyncMain);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
   try
   {
       Application->Initialize();
       Application->MainFormOnTaskBar = true;
       Application->CreateForm(__classid(TQRigSyncMain), &QRigSyncMain);
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   catch (...)
   {
       try
       {
          throw Exception("");
       }
       catch (Exception &exception)
       {
          Application->ShowException(&exception);
       }
   }
   return 0;
}
//---------------------------------------------------------------------------
