/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#ifndef RCVersionH
#define RCVersionH 
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

#if (__BORLANDC__ < 0x0530)
 #define PACKAGE
#endif

//---------------------------------------------------------------------------
class TMyRCVersion
{
   public:
	  enum eInfoFrom { ifEXE, ifDLL, ifPackage };
   private:

	  DWORD * TransBlock;
	  void * FVData;
	  String __fastcall GetCompanyName();
	  String __fastcall GetFileDescription();
	  String __fastcall GetFileVersion();
	  String __fastcall GetInternalName();
	  String __fastcall GetLegalCopyright();
	  String __fastcall GetLegalTrademarks();
	  String __fastcall GetOriginalFilename();
	  String __fastcall GetProductName();
	  String __fastcall GetProductVersion();
	  String __fastcall GetMajorVersion();
	  String __fastcall GetMinorVersion();
	  String __fastcall GetRelease();
	  String __fastcall GetComments();

	  String __fastcall GetSpecialBuild( void );

	  void __fastcall setValue( String );
	  void __fastcall SetInfoFrom( eInfoFrom e );
	  void __fastcall SetInstance( HINSTANCE );

      eInfoFrom FInfoFrom;

	  HINSTANCE FhInstance;

   protected:

   public:
      __fastcall TMyRCVersion();
      virtual __fastcall ~TMyRCVersion();
      void __fastcall initialise( void );
      // returns any string value from version info
    #if defined(_UNICODE)
       String __fastcall getValue(const wchar_t * itemName);
    #else
       String __fastcall getValue(const char * itemName);
    #endif

      __property eInfoFrom InfoFrom = {write = SetInfoFrom, read = FInfoFrom};
      __property String CompanyName = {write = setValue, read = GetCompanyName};
      __property String FileDescription = {write = setValue, read = GetFileDescription};
      __property String FileVersion = {write = setValue, read = GetFileVersion};
      __property String InternalName = {write = setValue, read = GetInternalName};
      __property String LegalCopyright = {write = setValue, read = GetLegalCopyright};
      __property String LegalTrademarks = {write = setValue, read = GetLegalTrademarks};
      __property String OriginalFilename = {write = setValue, read = GetOriginalFilename};
      __property String ProductName = {write = setValue, read = GetProductName};
      __property String ProductVersion = {write = setValue, read = GetProductVersion};
      __property String Comments = {write = setValue, read = GetComments};
      __property String MajorVersion = {write = setValue, read = GetMajorVersion};
      __property String MinorVersion = {write = setValue, read = GetMinorVersion};
      __property String Release = {write = setValue, read = GetRelease};
      __property String SpecialBuild = {write = setValue, read = GetSpecialBuild};
      __property HINSTANCE Instance = {write = SetInstance};
};
//---------------------------------------------------------------------------
#endif


