/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef LocCalcH
#define LocCalcH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TLocCalcForm : public TForm
{
   __published:   	// IDE-managed Components
      TLabel *Label1;
      TLabel *Label2;
      TLabel *Label3;
      TLabel *Label4;
      TLabel *Label5;
      TLabel *Label6;
      TEdit *S1Loc;
      TEdit *S1LatLong;
      TEdit *S1NGR;
      TEdit *S2Loc;
      TEdit *S2LatLong;
      TEdit *S2NGR;
      TButton *CalcButton;
      TEdit *Distance;
      TGroupBox *GroupBox1;
      TGroupBox *GroupBox2;
      TButton *ExitButton;
      TButton *CancelButton;
      void __fastcall CalcButtonClick( TObject *Sender );
      void __fastcall S1LocExit( TObject *Sender );
      void __fastcall S1LatLongExit( TObject *Sender );
      void __fastcall S1NGRExit( TObject *Sender );
      void __fastcall S2LocExit( TObject *Sender );
      void __fastcall S2LatLongExit( TObject *Sender );
      void __fastcall S2NGRExit( TObject *Sender );
      void __fastcall ExitButtonClick( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
   private:   	// User declarations
      bool Modal;
   public:   		// User declarations
      __fastcall ~TLocCalcForm();
      __fastcall TLocCalcForm( TComponent* Owner );
      bool __fastcall TLocCalcForm::handleExit( TEdit *Edit );
};
//---------------------------------------------------------------------------
extern PACKAGE TLocCalcForm *LocCalcForm;
//---------------------------------------------------------------------------
#endif
