/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef keyercontrolH
#define keyercontrolH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "DrawBox.hpp"
#include "FBVUMeter.hpp"
#include "xTrackBar.h"
#include "tspanel.h"
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>

class TKeyControlForm : public TForm
{
   __published:    	// IDE-managed Components
      TPageControl *PageControl1;
      TTabSheet *VoiceTab;
      TTabSheet *CWTab;
      TLabel *Label1;
      TLabel *recind;
      TLabel *Label2;
      TButton *RecordButton;
      TButton *StopButton;
      TButton *PlayButton;
      TComboBox *KeyCombo;
      TLabel *Label3;
      TLabel *Label4;
      TCheckBox *PTTOutCheckBox;
      TCheckBox *KeyCheckBox;
      TTimer *LineTimer;
      TFBVUMeter *CommonFBVUMeter;
      TxTrackBar *MicRecTrackBar;
      TxTrackBar *RecordTrackBar;
      TxTrackBar *OutputTrackBar;
      TLabel *Label5;
      TLabel *Label6;
      TLabel *Label8;
      TLabel *Label9;
      TLabel *Label10;
      TLabel *Label12;
      TTabSheet *Mixer;
      TImageList *TreeImageList;
      TSPanel *SPanel1;
      TSPanel *SPanel2;
      TSPanel *SPanel3;
      TCheckBox *PTTReflectCheckBox;
      TCheckBox *L1ReflectCheckBox;
      TCheckBox *L2ReflectCheckBox;
      TLabel *Label7;
      TLabel *Label11;
      TxTrackBar *MicOutTrackBar;
      TCheckBox *MicOutMute;
      TCheckBox *MasterMute;
      TCheckBox *PipCheckBox;
      TMemo *Memo1;
      TButton *SendButton;
      TButton *CWStopButton;
      TButton *ClearButton;
      TEdit *SpeedControl;
      TLabel *Label13;
      TEdit *ToneEdit;
      TLabel *Label14;
      TLabel *Label15;
      TxTrackBar *WaveOutTrackBar;
      TCheckBox *WaveOutMute;
      TLabel *Label16;
      TButton *Tone1Button;
      TButton *Tone2Button;
      TButton *AboutButton;
      TTimer *ConnectTimer;
      TTimer *LogTimer;
      TButton *ShowButton;
      TEdit *CWFileEdit;
      TButton *CWBrowse;
      TButton *SendFileButton;
      TOpenDialog *CWOpenDialog;
      TTimer *StartupTimer;
      TTimer *CaptionTimer;
      TCheckBox *AutoRepeatCheckBox;
      TEdit *DelayEdit;
      TLabel *Label17;
      TComboBox *InputMixerComboBox;
      TComboBox *OutputMixerComboBox;
      TLabel *Label18;
      TLabel *Label19;
      TComboBox *SourceComboBox;
      TComboBox *DestinationComboBox;
      TLabel *Label20;
      TLabel *Label21;
      TLabel *MUXSelection;
   TUpDown *DelayUpDown;
   TUpDown *SpeedUpDown;
   TLabel *Label22;
      void __fastcall RecordButtonClick( TObject *Sender );
      void __fastcall StopButtonClick( TObject *Sender );
      void __fastcall PlayButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall LineTimerTimer( TObject *Sender );
      void __fastcall MicRecTrackBarChange( TObject *Sender );
      void __fastcall RecordTrackBarChange( TObject *Sender );
      void __fastcall OutputTrackBarChange( TObject *Sender );
      void __fastcall MonitorButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall MicOutTrackBarChange( TObject *Sender );
      void __fastcall PipCheckBoxClick( TObject *Sender );
      void __fastcall SendButtonClick( TObject *Sender );
      void __fastcall ClearButtonClick( TObject *Sender );
      void __fastcall ToneEditKeyPress( TObject *Sender, char &Key );
      void __fastcall WaveOutTrackBarChange( TObject *Sender );
      void __fastcall Tone1ButtonClick( TObject *Sender );
      void __fastcall Tone2ButtonClick( TObject *Sender );
      void __fastcall AboutButtonClick( TObject *Sender );
      void __fastcall ConnectTimerTimer( TObject *Sender );
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall FormKeyUp( TObject *Sender, WORD &Key,
                                 TShiftState Shift );
      void __fastcall FormCloseQuery( TObject *Sender, bool &CanClose );
      void __fastcall ShowButtonClick( TObject *Sender );
      void __fastcall CWBrowseClick( TObject *Sender );
      void __fastcall SendFileButtonClick( TObject *Sender );
      void __fastcall StartupTimerTimer( TObject *Sender );
      void __fastcall FormResize( TObject *Sender );
      void __fastcall CaptionTimerTimer( TObject *Sender );
      void __fastcall AutoRepeatCheckBoxClick( TObject *Sender );
      void __fastcall DelayEditChange( TObject *Sender );
      void __fastcall SpeedControlChange( TObject *Sender );
      void __fastcall ToneEditChange( TObject *Sender );
      void __fastcall InputMixerComboBoxChange( TObject *Sender );
      void __fastcall SourceLineComboBoxChange( TObject *Sender );
      void __fastcall OutputMixerComboBoxChange( TObject *Sender );
      void __fastcall DestinationComboBoxChange( TObject *Sender );

   private:    	// User declarations

      bool recordWait;
      bool recording;
      bool saveResize;

      void populateMixerList();
      void populateInputMixer();
      void populateOutputMixer();

   protected:
#define CM_INPUTLEVEL (WM_APP + 2400)
 #define CM_OUTPUTLEVEL (WM_APP + 2401)

      void __fastcall CMInputLevel( TMessage &Message );
      void __fastcall CMOutputLevel( TMessage &Message );
      void __fastcall MMMixLineChange( TMessage &Message );
      void __fastcall MMMixControlChange( TMessage &Message );
      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      VCL_MESSAGE_HANDLER( CM_INPUTLEVEL, TMessage, CMInputLevel );
      VCL_MESSAGE_HANDLER( CM_OUTPUTLEVEL, TMessage, CMOutputLevel );
      VCL_MESSAGE_HANDLER( MM_MIXM_LINE_CHANGE, TMessage, MMMixLineChange );
      VCL_MESSAGE_HANDLER( MM_MIXM_CONTROL_CHANGE, TMessage, MMMixControlChange );
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm );

   public:    		// User declarations
      __fastcall TKeyControlForm( TComponent* Owner );
      void __fastcall closeShow();
      bool PTT;
      bool keyline;
      bool PTTRef;
      bool L1Ref;
      bool L2Ref;
      void __fastcall syncSetLines();
};
//---------------------------------------------------------------------------
extern PACKAGE TKeyControlForm *KeyControlForm;
//---------------------------------------------------------------------------
#endif
