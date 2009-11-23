/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MShowMessageDlgH
#define MShowMessageDlgH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TMinosShowMessage : public TForm
{
   __published:  	// IDE-managed Components
      TLabel *Label1;
      TButton *Button1;
      void __fastcall Button1Click( TObject *Sender );
   private:  	// User declarations
   public:  		// User declarations
      __fastcall TMinosShowMessage( TComponent* Owner );
};
//---------------------------------------------------------------------------

extern void mShowMessage( String mess, TComponent* Owner = 0 );
#endif
