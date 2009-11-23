/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
#ifndef enqdlgH
 #define enqdlgH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp> 
//---------------------------------------------------------------------------
class PACKAGE TEnquireDlg : public TForm
{
   __published:    // IDE-managed Components
      TLabel *PromptLabel;
      TEdit *EnqValue;
      TButton *Button1;
      TButton *Button2;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall EnqValueKeyPress( TObject *Sender, char &Key );
   private:    // User declarations
   public:     // User declarations
      bool intenq;
      __fastcall TEnquireDlg( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern bool enquireDialog( TComponent* Owner, const String &prompt, String &Value, int limit = 0 );
extern bool enquireDialog( TComponent* Owner, const String &prompt, std::string &Value, int limit = 0 );
extern bool enquireDialog( TComponent* Owner, const String &prompt, int &Value );
//---------------------------------------------------------------------------
#endif
