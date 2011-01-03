//---------------------------------------------------------------------------

#include "XMPP_pch.h"
#pragma hdrstop
#include "inifiles.hpp"

#include "SyncMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OmniRig_OCX"
#pragma resource "*.dfm"

TQRigSyncMain *QRigSyncMain;

/*
   // IF I work as the example with the TLB rather than the OCX
   // does omnirig need to be "installed"? Or can I run up the
   // omnirig.exe as and when I require it, and carry both
   // exe, rigs, and omnirig.ini with MY app?

   // Lets get it working first...
*/
//---------------------------------------------------------------------------
__fastcall TQRigSyncMain::TQRigSyncMain(TComponent* Owner)
   : TForm(Owner)
{
   enableTrace( ".\\TraceLog\\QS1RIVSync_" );
   Timer1->Enabled = true;
}
__fastcall TQRigSyncMain::~TQRigSyncMain()
{
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::CloseButtonClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::Timer1Timer(TObject *Sender)
{
   static bool inTimer = false;
   if (inTimer)
   {
      return;
   }

   inTimer = true;

   long freq = 0;

   freq = OmniRig->Rig1->FreqA;
   QF1Label->Caption = String(freq);

   freq = OmniRig->Rig2->FreqA;
   QF2Label->Caption = String(freq);

   inTimer = false;
}
//---------------------------------------------------------------------------


void __fastcall TQRigSyncMain::RigSelectButtonClick(TObject *Sender)
{
   if (!OmniRig->DialogVisible)
   {
      OmniRig->DialogVisible = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::FormShow(TObject *Sender)
{
   Top = Screen->Height - Height;
   Left = (Screen->Width - Width)/2;

   OmniRig->Connect();
   Rig1Label->Caption = OmniRig->Rig1->RigType;
   Rig2Label->Caption = OmniRig->Rig2->RigType;
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   //
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::Transfer12ButtonClick(TObject *Sender)
{
   long freq = 0;

   freq = OmniRig->Rig1->FreqA;

   OmniRig->Rig2->FreqA = freq;

}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::Transfer21ButtonClick(TObject *Sender)
{
   long freq = 0;

   freq = OmniRig->Rig2->FreqA;

   OmniRig->Rig1->FreqA = freq;
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::OmniRigRigTypeChange(TObject *Sender, long RigNumber)
{
   Rig1Label->Caption = OmniRig->Rig1->RigType;
   Rig2Label->Caption = OmniRig->Rig2->RigType;
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::OmniRigStatusChange(TObject *Sender, long RigNumber)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::OmniRigParamsChange(TObject *Sender, long RigNumber,
          long Params)
{
//
   bool ptt1 = OmniRig->Rig1->Tx & RigParamX::PM_TX;
   bool ptt2 = OmniRig->Rig2->Tx & RigParamX::PM_TX;

}
//---------------------------------------------------------------------------

