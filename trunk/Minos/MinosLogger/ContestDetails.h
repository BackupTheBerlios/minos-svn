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
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include "BundleFrame.h"
#include "JvDateTimePicker.hpp"
#include "JvExComCtrls.hpp"
#include <ComCtrls.hpp>
#include "JvCheckedMaskEdit.hpp"
#include "JvDatePickerEdit.hpp"
#include "JvExMask.hpp"
#include "JvMaskEdit.hpp"
#include "JvToolEdit.hpp"
#include <Mask.hpp> 
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
      TCheckBox *ReadOnlyOption;
      TCheckBox *PostEventOption;
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
      TLabel *Label1;
      TLabel *Label2;
      TJvDatePickerEdit *StartDateEdit;
      TJvDatePickerEdit *EndDateEdit;
      TComboBox *StartTimeCombo;
      TComboBox *EndTimeCombo;
      TLabel *StartTimeLabel;
      TLabel *EndTimeLabel;
      TComboBox *ExchangeComboBox;
      TButton *VHFCalendarButton;
      TEdit *ContestNameSelected;
   TGroupBox *LocatorGroupBox;
   TCheckBox *AllowLoc8CB;
   TCheckBox *AllowLoc4CB;
   TCheckBox *MultiBandCheckBox4;
      void __fastcall OKButtonClick( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall EntDetailButtonClick( TObject *Sender );
      void __fastcall BundleOverrideButtonClick( TObject *Sender );
      void __fastcall BSHelpButtonClick( TObject *Sender );
      void __fastcall DateEditKeyPress( TObject *Sender, char &Key );
      void __fastcall VHFCalendarButtonClick( TObject *Sender );
   void __fastcall BundleFrameBundleSectionChange(TObject *Sender);
   void __fastcall BundleFrameBundleEditClick(TObject *Sender);
   private:   	// User declarations
      TCalendarForm *CalendarDlg;
      LoggerContestLog * contest;
      LoggerContestLog * inputcontest;
      TWinControl * getNextFocus();
      void setDetails( const IndividualContest &ic );
      void setDetails( );
      TWinControl * getDetails( );
   public:   		// User declarations
      __fastcall TContestEntryDetails( TComponent* Owner );
      __fastcall ~TContestEntryDetails( );
      void setDetails( LoggerContestLog * ct );
};
//---------------------------------------------------------------------------
#endif
