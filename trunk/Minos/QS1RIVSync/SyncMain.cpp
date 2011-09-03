//---------------------------------------------------------------------------

#include "XMPP_pch.h"
#pragma hdrstop
#include "inifiles.hpp"
#include "SyncMain.h"
#include "qs_defines.h"
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
         lastF = "OK\r\n"
         "fHz=28123456\r\n"
         "tf=-123456\r\n";

         int fOffset = lastF.Pos("fHz=");
         int tfOffset = lastF.Pos("tf=");
         if (fOffset > 0 && tfOffset > 0)
         {
            String temp = lastF.SubString(fOffset + 4, tfOffset - fOffset - 4);
            int l = temp.Length();
            while ((temp[l] == '\r') || (temp[l] == '\n'))
            {
               temp = temp.SubString(1, l - 1);
               l = temp.Length();
            }
            fCentre = temp.ToIntDef(0);
            temp = lastF.SubString(tfOffset + 3, 100);
            l = temp.Length();
            while ((temp[l] == '\r') || (temp[l] == '\n'))
            {
               temp = temp.SubString(1, l - 1);
               l = temp.Length();
            }
            ftf = temp.ToIntDef(0);
         }
         double f = (fCentre + ftf);
         lastF = "fCentre " + String(fCentre) + "\r\n tf " + String(ftf) + " freq " + FormatFloat("#0,000,000", f);

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

   QS1RFLabel->Caption = lastF;

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

   // and we would need to send it to the QS1R
//   OmniRig->Rig2->FreqA = freq;

}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::Transfer21ButtonClick(TObject *Sender)
{
   long freq = fCentre + ftf;

   OmniRig->Rig1->FreqA = freq;
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::OmniRigRigTypeChange(TObject *Sender, long RigNumber)
{
   Rig1Label->Caption = OmniRig->Rig1->RigType;
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
//   bool ptt1 = OmniRig->Rig1->Tx & PM_TX;
//   bool ptt2 = OmniRig->Rig2->Tx & PM_TX;

}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::ConnectQS1RButtonClick(TObject *Sender)
{
// // start up the QS1R socket
   if (ClientSocket1->Active)
   {
      ClientSocket1->Close();
   }
   else
   {
      ClientSocket1->Port = RX1_CMD_SERV_TCP_PORT + 2;
      ClientSocket1->Host = "localhost";

      ClientSocket1->Open();
   }
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
   ConnectQS1RButton->Caption = "Disconnect QS1R";
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
   ConnectQS1RButton->Caption = "Connect QS1R";
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::ClientSocket1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
   static char sockbuffer[ 4096 ];
   //Hz=5.02e+07
   //tf=-121700

   try
   {
      int retlen = ClientSocket1->Socket->ReceiveBuf( sockbuffer, 4095 );
      if ( retlen > 0 )
      {
         sockbuffer[ retlen ] = 0;

         lastF =  sockbuffer;
         int fOffset = lastF.Pos("fHz=");
         int tfOffset = lastF.Pos("tf=");
         if (fOffset > 0 && tfOffset > 0)
         {
            String temp = lastF.SubString(fOffset + 4, tfOffset - fOffset - 4);
            int l = temp.Length();
            while ((temp[l] == '\r') || (temp[l] == '\n'))
            {
               temp = temp.SubString(1, l - 1);
               l = temp.Length();
            }
            fCentre = temp.ToIntDef(0);
            temp = lastF.SubString(tfOffset + 3, 100);
            l = temp.Length();
            while ((temp[l] == '\r') || (temp[l] == '\n'))
            {
               temp = temp.SubString(1, l - 1);
               l = temp.Length();
            }
            ftf = temp.ToIntDef(0);
         }
         double f = (fCentre + ftf);
         lastF = "fCentre " + String(fCentre) + "\r\n tf " + String(ftf) + " freq " + FormatFloat("#0,000,000", f);
      }
   }
   catch ( Exception & e )
   {
//      Log( "**** TQRigSyncMain::ClientSocket1Read - Exception: " + e.Message );
   }
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::ClientSocket1Write(TObject *Sender,
      TCustomWinSocket *Socket)
{
   // we can write...
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::ApplicationEvents1Exception(TObject *Sender,
      Exception *E)
{
//   
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::Timer2Timer(TObject *Sender)
{
   // Poll the QS1R
   String mess = ">UpdateRxFreq\n?fHz\n?tf\n";
   ClientSocket1->Socket->SendBuf( mess.c_str(), mess.Length() );
}
//---------------------------------------------------------------------------

