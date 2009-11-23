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

#include<memory>
#include <stdio.h>
#include "RCVersion.h"

#if (__BORLANDC__ >= 0x0530)
    #pragma package(smart_init)
#else
    #define PACKAGE
#endif

   /* TODO -orq :
auto_ptr with arrays causes erros in CodeGuard. The object is deleted with
delete instead of delete []. It's use needs to be removed. */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
namespace Rcversion
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TRCVersion)};
       RegisterComponents("MJGAddons", classes, 0);
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TRCVersion *)
{
   new TRCVersion(NULL);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TRCVersion::TRCVersion(TComponent* Owner)
   : TComponent(Owner),FhInstance(0),FInfoFrom(ifEXE),FVData(0)
{
}
//---------------------------------------------------------------------------
__fastcall TRCVersion::~TRCVersion()
{
  if (FVData)
    delete[] FVData;
  FVData = 0;
}
//---------------------------------------------------------------------------
void __fastcall TRCVersion::Loaded(void)
{
   FVData = 0;
   TComponent::Loaded();
   Initialise();
}
//---------------------------------------------------------------------------
void __fastcall TRCVersion::SetValue(String)
{
   ::MessageBeep(0);
}
//---------------------------------------------------------------------------
void __fastcall TRCVersion::SetInstance(int n)
{
   FhInstance = n;
   if ( FhInstance )
      FInfoFrom = ifDLL;
   else
      FInfoFrom = ifEXE;
   Initialise();
}
//---------------------------------------------------------------------------
void __fastcall TRCVersion::Initialise(void)
{
   delete[] FVData;
   FVData = 0;

   if ( ComponentState.Contains(csDestroying) )
      return;

   DWORD  fvHandle;
   unsigned int    vSize;
   char    appFName[255];
   char * subBlockName = new char[255];
   std::auto_ptr<char> p1(subBlockName);
   int hInstance = 0; // default to application
   switch ( FInfoFrom )
   {
      case ifDLL:
         hInstance = FhInstance;
         break;
      case ifPackage:
         hInstance = (int)HInstance; // DLL Module Instance for package
         break;
   }
   int nLen = ::GetModuleFileName( (void*)hInstance, appFName, 255 );
   appFName[nLen] = '\0';
   OemToChar(appFName, appFName);
   DWORD dwSize = ::GetFileVersionInfoSize(appFName,&fvHandle);
   if (dwSize)
   {
      FVData  = (void *)new char[dwSize];
      if (::GetFileVersionInfo(appFName, fvHandle, dwSize, FVData))
      {
         // Copy string to buffer so if the -dc compiler switch(Put constant strings in code segments)
         // is on VerQueryValue will work under Win16.  This works around a problem in Microsoft's ver.dll
         // which writes to the string pointed to by subBlockName.
         //
         strcpy(subBlockName, "\\VarFileInfo\\Translation");
         if (!::VerQueryValue(FVData, subBlockName,(void * *)&TransBlock, &vSize))
         {
            delete[] FVData;
            FVData = 0;
         }
      else
      // Swap the words so sprintf will print the lang-charset in the correct format.
      //
         *TransBlock = MAKELONG(HIWORD(*TransBlock), LOWORD(*TransBlock));
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TRCVersion::SetInfoFrom(eInfoFrom e)
{
   if ( FInfoFrom == e )
      return;

   FInfoFrom = e;
   Initialise();
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetValue(const char * itemName)
{
   if ( ComponentState.Contains(csDestroying) || ComponentState.Contains(csLoading) )
      return String();

  unsigned int vSize;
  char * subBlockName = new char[255];
  std::auto_ptr<char> p1(subBlockName);
  char * ItemValue = new char[2000];
  std::auto_ptr<char> p2(ItemValue);

  ItemValue[0] = '\0';
  String itemValue;
  if (FVData)
  {
    sprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *TransBlock,(LPSTR)itemName);
    if ( ::VerQueryValue(FVData, subBlockName,(void * *)&ItemValue, &vSize) )
        itemValue = ItemValue;
  }
  return itemValue;
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetLegalCopyright(void)
{
   return GetValue("LegalCopyright");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetCompanyName(void)
{
   return GetValue("CompanyName");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetFileDescription(void)
{
   return GetValue("FileDescription");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetFileVersion(void)
{
   return GetValue("FileVersion");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetInternalName(void)
{
   return GetValue("InternalName");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetLegalTrademarks(void)
{
   return GetValue("LegalTrademarks");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetOriginalFilename(void)
{
   return GetValue("OriginalFilename");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetProductName(void)
{
   return GetValue("ProductName");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetProductVersion(void)
{
   return GetValue("ProductVersion");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetComments(void)
{
   return GetValue("Comments");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetSpecialBuild(void)
{
   return GetValue("SpecialBuild");
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetMajorVersion(void)
{
   String s = GetFileVersion();
   // s = major.minor.release.build
   int n = s.Pos('.');
   if ( n )
      return s.SubString(1,n-1);
   return s;
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetMinorVersion(void)
{
   String s = GetFileVersion();
   int nStart = s.Pos('.');
   if ( !nStart )
      return s;
   s[nStart] = ' ';
   int nEnd = s.Pos('.');
   return s.SubString(nStart+1,nEnd-1-nStart);
}
//---------------------------------------------------------------------------
String __fastcall TRCVersion::GetRelease(void)
{
   String s = GetFileVersion();
   int nStart = s.Pos('.');
   if ( !nStart )
      return s;
   s[nStart] = ' ';
   nStart = s.Pos('.');
   if ( !nStart )
      return s;
   s[nStart] = ' ';
   int nEnd = s.Pos('.');
   return s.SubString(nStart+1,nEnd-1-nStart);
}
//---------------------------------------------------------------------------
