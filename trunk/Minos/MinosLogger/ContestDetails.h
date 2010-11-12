/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef ContestDetailsH
#define ContestDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include "BundleFrame.h"
//---------------------------------------------------------------------------
class Calendar;
class IndividualContest;
class TCalendarForm;
class TContestEntryDetails : public TForm
{
   __published:   	// IDE-managed Components
      TButton *OKButton;
      TButton *CancelButton;
      TButton *EntDetailButton;
      TLabeledEdit *ContestNameEdit;
      TComboBox *BandComboBox;
      TLabel *BandLabel;
      TLabeledEdit *CallsignEdit;
      TLabeledEdit *LocatorEdit;
      TLabeledEdit *ExchangeEdit;
      TRadioGroup *ScoreOptions;
      TGroupBox *MultGroupBox;
      TCheckBox *DXCCMult;
      TCheckBox *LocatorMult;
      TGroupBox *OptionsGroupBox;
      TGroupBox *FieldsGroupBox;
   TCheckBox *ProtectedOption;
      TCheckBox *RSTField;
      TCheckBox *SerialField;
      TCheckBox *LocatorField;
      TCheckBox *QTHField;
      TScrollBox *BundleBox;
      TSettingBundleFrame *QTHBundleFrame;
      TSettingBundleFrame *StationBundleFrame;
      TSettingBundleFrame *EntryBundleFrame;
      TLabel *SectionLabel;
      TComboBox *SectionComboBox;
      TLabeledEdit *PowerEdit;
      TButton *BSHelpButton;
      TComboBox *StartTimeCombo;
      TComboBox *EndTimeCombo;
      TLabel *StartTimeLabel;
      TLabel *EndTimeLabel;
      TComboBox *ExchangeComboBox;
   TGroupBox *LocatorGroupBox;
   TCheckBox *AllowLoc8CB;
   TCheckBox *AllowLoc4CB;
   TLabeledEdit *StartDateEdit;
   TLabeledEdit *EndDateEdit;
   TButton *StartDateButton;
   TButton *EndDateButton;
   TLabel *Label1;
   TComboBox *MainOpComboBox;
   TLabel *Label2;
   TComboBox *SecondOpComboBox;
   TPanel *Panel1;
   TButton *VHFCalendarButton;
   TEdit *ContestNameSelected;
   TCheckBox *GLocMult;
      void __fastcall OKButtonClick( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall EntDetailButtonClick( TObject *Sender );
      void __fastcall BSHelpButtonClick( TObject *Sender );
      void __fastcall DateEditKeyPress( TObject *Sender, char &Key );
      void __fastcall VHFCalendarButtonClick( TObject *Sender );
   void __fastcall BundleFrameBundleSectionChange(TObject *Sender);
   void __fastcall BundleFrameBundleEditClick(TObject *Sender);
   void __fastcall EndDateButtonClick(TObject *Sender);
   void __fastcall StartDateButtonClick(TObject *Sender);
   void __fastcall CallsignEditExit(TObject *Sender);
   void __fastcall ProtectedOptionClick(TObject *Sender);
   private:   	// User declarations
//      TCalendarForm *CalendarDlg;
      LoggerContestLog * contest;
      LoggerContestLog * inputcontest;
      String sectionList;
      bool saveContestOK;
      bool suppressProtectedOnClick;

      TWinControl * getNextFocus();
      void setDetails( const IndividualContest &ic );
      void setDetails( );
      void refreshOps();
      void enableControls();
      TWinControl * getDetails( );
   public:   		// User declarations
      __fastcall TContestEntryDetails( TComponent* Owner );
      __fastcall ~TContestEntryDetails( );
      void setDetails( LoggerContestLog * ct );
};
//---------------------------------------------------------------------------
#endif
