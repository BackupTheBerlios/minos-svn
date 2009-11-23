/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef TManageListsDlgH
#define TManageListsDlgH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TManageListsDlg : public TForm
{
   __published: 	// IDE-managed Components
      TListBox *ListsListBox;
      TButton *CloseListButton;
      TButton *ExitButton;
      TButton *EditListButton;
      void __fastcall CloseListButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall EditListButtonClick( TObject *Sender );
   private: 	// User declarations
      void DrawList();
   public: 		// User declarations
      __fastcall TManageListsDlg( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TManageListsDlg *ManageListsDlg;
//---------------------------------------------------------------------------
#endif
