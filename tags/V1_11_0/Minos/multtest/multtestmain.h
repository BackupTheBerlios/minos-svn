/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
   __published: 	// IDE-managed Components
      TButton *Button1;
      void __fastcall Button1Click( TObject *Sender );
   private: 	// User declarations
   public: 		// User declarations
      __fastcall TForm1( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
