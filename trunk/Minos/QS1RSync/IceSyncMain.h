//---------------------------------------------------------------------------

#ifndef IceSyncMainH
#define IceSyncMainH
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
   TLabel *RigLabel;
   TButton *MuteButton;
   TButton *RigSelectButton;
   TButton *TransferButton;
   TLabel *QFLabel;
   TOmniRigX *OmniRig;
   TLabel *Label1;
   void __fastcall CloseButtonClick(TObject *Sender);
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall MuteButtonClick(TObject *Sender);
   void __fastcall RigSelectButtonClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TransferButtonClick(TObject *Sender);
   void __fastcall OmniRigRigTypeChange(TObject *Sender, long RigNumber);
   void __fastcall OmniRigStatusChange(TObject *Sender, long RigNumber);
private:	// User declarations
   Ice::CommunicatorPtr ic;
   QS1R::RadioCommanderPrx qs1r;
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
