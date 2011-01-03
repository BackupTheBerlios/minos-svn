//---------------------------------------------------------------------------

#ifndef SyncMainH
#define SyncMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "OmniRig_OCX.h"
#include <OleServer.hpp>
//---------------------------------------------------------------------------
class TQRigSyncMain : public TForm
{
__published:	// IDE-managed Components
   TButton *CloseButton;
   TTimer *Timer1;
   TLabel *Rig1Label;
   TButton *RigSelectButton;
   TButton *Transfer12Button;
   TLabel *QF1Label;
   TOmniRigX *OmniRig;
   TLabel *Label1;
   TLabel *Rig2Label;
   TLabel *QF2Label;
   TButton *Transfer21Button;
   void __fastcall CloseButtonClick(TObject *Sender);
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall RigSelectButtonClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall Transfer12ButtonClick(TObject *Sender);
   void __fastcall OmniRigRigTypeChange(TObject *Sender, long RigNumber);
   void __fastcall OmniRigStatusChange(TObject *Sender, long RigNumber);
   void __fastcall OmniRigParamsChange(TObject *Sender, long RigNumber, long Params);
   void __fastcall Transfer21ButtonClick(TObject *Sender);

private:	// User declarations
   bool muted;
   long getQS1RFreq();

public:		// User declarations
   __fastcall TQRigSyncMain(TComponent* Owner);
   __fastcall ~TQRigSyncMain();
   int print_model_list( const struct rig_caps *caps );
};
//---------------------------------------------------------------------------
extern PACKAGE TQRigSyncMain *QRigSyncMain;
//---------------------------------------------------------------------------
#endif
