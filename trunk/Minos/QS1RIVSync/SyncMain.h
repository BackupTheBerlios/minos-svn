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
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <ScktComp.hpp>
#include <AppEvnts.hpp>
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
   TButton *ConnectQS1RButton;
   TButton *Transfer21;
   TClientSocket *ClientSocket1;
   TApplicationEvents *ApplicationEvents1;
   TLabel *QS1RLabel;
   TLabel *QS1RFLabel;
   TTimer *Timer2;
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
   void __fastcall ConnectQS1RButtonClick(TObject *Sender);
   void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket);
   void __fastcall ClientSocket1Write(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ApplicationEvents1Exception(TObject *Sender, Exception *E);
   void __fastcall Timer2Timer(TObject *Sender);

private:	// User declarations
   bool muted;
   long getQS1RFreq();
   String lastF;
   int fCentre;
   int ftf;

public:		// User declarations
   __fastcall TQRigSyncMain(TComponent* Owner);
   __fastcall ~TQRigSyncMain();
   int print_model_list( const struct rig_caps *caps );
};
//---------------------------------------------------------------------------
extern PACKAGE TQRigSyncMain *QRigSyncMain;
//---------------------------------------------------------------------------
#endif
