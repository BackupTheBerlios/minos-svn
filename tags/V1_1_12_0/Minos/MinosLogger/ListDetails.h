/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ListDetailsH
#define ListDetailsH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class ContactList;
class TContactListDetails : public TForm
{
   __published: 	// IDE-managed Components
      TButton *CloseButton;
      TButton *Button1;
      TLabeledEdit *ListNameEdit;
      void __fastcall CloseButtonClick( TObject *Sender );
   private: 	// User declarations
   public: 		// User declarations
      __fastcall TContactListDetails( TComponent* Owner );
      bool getDetails( ContactList * ct );
      void setDetails( ContactList * ct );
};
//---------------------------------------------------------------------------
#endif
