/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosHelpH
#define MinosHelpH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TMinosHelpForm : public TForm
{
   __published: 	// IDE-managed Components
      TPanel *Panel1;
      TButton *CloseButton;
      TMemo *HelpMemo;
      void __fastcall CloseButtonClick( TObject *Sender );
   private: 	// User declarations
   public: 		// User declarations
      __fastcall TMinosHelpForm( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TMinosHelpForm *MinosHelpForm;
//---------------------------------------------------------------------------
#endif
