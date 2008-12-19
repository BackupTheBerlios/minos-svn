/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef ConfigMainFrameH
#define ConfigMainFrameH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TTConfigFrame : public TFrame
{
   __published:  	// IDE-managed Components
      TScrollBox *ElementScrollBox;
      TPanel *Panel1;
      TCheckBox *HideCheckBox;
      TLabeledEdit *StationIdEdit;
      TButton *SetButton;
      TButton *ClearAllButton;
      TButton *StartButton;
      TButton *StopButton;
      TButton *CancelButton;
   TButton *ModifyButton;
      void __fastcall StartButtonClick( TObject *Sender );
      void __fastcall StopButtonClick( TObject *Sender );
      void __fastcall ClearAllButtonClick( TObject *Sender );
      void __fastcall SetButtonClick( TObject *Sender );
   void __fastcall HideCheckBoxClick(TObject *Sender);
   void __fastcall ModifyButtonClick(TObject *Sender);
   private:  	// User declarations
      std::vector <TConfigElementFrm *> framelist;
   public:  		// User declarations
      __fastcall TTConfigFrame( TComponent* Owner );
      void initialise();
};
//---------------------------------------------------------------------------
extern PACKAGE TTConfigFrame *TConfigFrame;
//---------------------------------------------------------------------------
#endif
