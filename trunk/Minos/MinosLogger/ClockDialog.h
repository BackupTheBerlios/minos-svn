/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ClockDialogH
#define ClockDialogH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp> 
//---------------------------------------------------------------------------
class TClockDlg : public TForm
{
   __published:  	// IDE-managed Components
      TLabel *Label1;
      TLabel *Label2;
      TLabel *RawTime;
      TLabel *CorrectedTime;
      TLabel *Label5;
      TLabel *Label6;
      TButton *OKButton;
      TButton *ClearButton;
      TButton *ApplyButton;
      TTimer *Timer1;
      TLabeledEdit *DayEdit;
      TLabeledEdit *MonthEdit;
      TLabeledEdit *YearEdit;
      TLabeledEdit *HoursEdit;
      TLabeledEdit *MinutesEdit;
      TLabeledEdit *SecondsEdit;
      TUpDown *UpDown1;
      TUpDown *UpDown2;
      TUpDown *UpDown3;
      TUpDown *UpDown4;
      TUpDown *UpDown5;
      TUpDown *UpDown6;
      TButton *CancelButton;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall OKButtonClick( TObject *Sender );
      void __fastcall ClearButtonClick( TObject *Sender );
      void __fastcall ApplyButtonClick( TObject *Sender );
      void __fastcall Timer1Timer( TObject *Sender );
      void __fastcall ClockEditChange( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
   private:  	// User declarations
      double correction;
      void setEdits();
      bool initialised;
   public:  		// User declarations
      __fastcall TClockDlg( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TClockDlg *ClockDlg;
//---------------------------------------------------------------------------
#endif
