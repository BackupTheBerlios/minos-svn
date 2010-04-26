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
class PACKAGE TRCVersion : public TComponent
{
   public:
      enum eInfoFrom { ifEXE, ifDLL, ifPackage };
   private:

    DWORD *  TransBlock;
    void *   FVData;
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

    String __fastcall GetSpecialBuild(void);

    void __fastcall SetValue(String);
    void __fastcall SetInfoFrom(eInfoFrom e);
    void __fastcall SetInstance(int);
    void __fastcall Initialise(void);
    void __fastcall Loaded(void);

    eInfoFrom FInfoFrom;

    int FhInstance;

   protected:

public:
   __fastcall TRCVersion(TComponent* Owner);
    virtual __fastcall ~TRCVersion();
    // returns any string value from version info
    String __fastcall GetValue(const char * itemName);
__published:
    __property eInfoFrom InfoFrom           = {write=SetInfoFrom, read=FInfoFrom, default = ifEXE};
    __property String CompanyName           = {stored=false, write=SetValue, read=GetCompanyName};
    __property String FileDescription       = {stored=false, write=SetValue, read=GetFileDescription};
    __property String FileVersion           = {stored=false, write=SetValue, read=GetFileVersion};
    __property String InternalName          = {stored=false, write=SetValue, read=GetInternalName};
    __property String LegalCopyright        = {stored=false, write=SetValue, read=GetLegalCopyright};
    __property String LegalTrademarks       = {stored=false, write=SetValue, read=GetLegalTrademarks};
    __property String OriginalFilename      = {stored=false, write=SetValue, read=GetOriginalFilename};
    __property String ProductName           = {stored=false, write=SetValue, read=GetProductName};
    __property String ProductVersion        = {stored=false, write=SetValue, read=GetProductVersion};
    __property String Comments              = {stored=false, write=SetValue, read=GetComments};
    __property String MajorVersion          = {stored=false, write=SetValue, read=GetMajorVersion};
    __property String MinorVersion          = {stored=false, write=SetValue, read=GetMinorVersion};
    __property String Release               = {stored=false, write=SetValue, read=GetRelease};
    __property String SpecialBuild          = {stored=false, write=SetValue, read=GetSpecialBuild};
    __property int    Instance              = {stored=false, write=SetInstance, default = 0};
};
//---------------------------------------------------------------------------
#endif


