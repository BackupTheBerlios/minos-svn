/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef testrmainH
#define testrmainH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "xTrackBar.h"
#include <ComCtrls.hpp> 
//---------------------------------------------------------------------------
class TTestRigMain : public TForm
{
   __published:  	// IDE-managed Components
      TLabel *Label1;
      TLabel *Label2;
      TButton *CloseButton;
      TTimer *Timer1;
      TxTrackBar *FreqSlider;
      TComboBox *RigCombo;
      TLabel *Label3;
      TButton *RigSelectButton;
      TComboBox *PortCombo;
      TLabel *Label4;
      TRadioGroup *ModeRadioGroup;
      TTimer *StartupTimer;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall Timer1Timer( TObject *Sender );
      void __fastcall FreqSliderChange( TObject *Sender );
      void __fastcall RigSelectButtonClick( TObject *Sender );
      void __fastcall ModeRadioGroupClick( TObject *Sender );
      void __fastcall StartupTimerTimer( TObject *Sender );
      void __fastcall FormResize( TObject *Sender );
   private:  	// User declarations
      RIG *my_rig;		/* handle to rig (nstance) */
      bool noSetFreq;
      bool noSetMode;
      bool saveResize;
   public:  		// User declarations
      __fastcall TTestRigMain( TComponent* Owner );
      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TTestRigMain *TestRigMain;
//---------------------------------------------------------------------------
#endif
