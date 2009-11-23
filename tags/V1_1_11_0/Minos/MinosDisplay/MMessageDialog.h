/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MMessageDialogH
#define MMessageDialogH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TMessageDialog : public TForm
{
   __published:  	// IDE-managed Components
      TLabel *MessageLabel;
      TButton *Button1;
      TButton *Button2;
   private:  	// User declarations
   public:  		// User declarations
      __fastcall TMessageDialog( TComponent* Owner );
};
//---------------------------------------------------------------------------
bool mShowYesNoMessage( TComponent* Owner, String mess );
//---------------------------------------------------------------------------
#endif
