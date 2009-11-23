/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef ForceLogH
#define ForceLogH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TForceLogDlg : public TForm
{
   __published:  	// IDE-managed Components
      TLabel *Label1;
      TListBox *ErrList;
      TGroupBox *GroupBox1;
      TCheckBox *CheckBox1;
      TCheckBox *CheckBox2;
      TCheckBox *CheckBox3;
      TCheckBox *CheckBox4;
      TCheckBox *CheckBox5;
      TCheckBox *CheckBox6;
      TCheckBox *CheckBox7;
      TCheckBox *CheckBox8;
      TLabeledEdit *ScoreIl;
      TLabeledEdit *CtryMultIl;
      TButton *OKButton;
      TButton *LocCalcButton;
      TButton *CancelButton;
      TLabel *Label2;
      void __fastcall OKButtonClick( TObject *Sender );
      void __fastcall LocCalcButtonClick( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
   private:  	// User declarations
   public:  		// User declarations
      __fastcall TForceLogDlg( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TForceLogDlg *ForceLogDlg;
//---------------------------------------------------------------------------
#endif
