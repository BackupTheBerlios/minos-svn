//---------------------------------------------------------------------------

#include "XMPP_pch.h"
#pragma hdrstop
#include "inifiles.hpp"
#include <ice\ice.h>
#include "radiointerface.h"

#include "IceSyncMain.h"
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
   : TForm(Owner), muted(false), ic(0)
{
   static bool failSeen = false;
   enableTrace( ".\\TraceLog\\QS1RSync_" );
   try
   {
      int args = 0;
      ic = Ice::initialize(args, _argv);
      Ice::ObjectPrx base = ic->stringToProxy(
            "RadioCommander:default -t 2000 -h opti -p 43065");
      qs1r = QS1R::RadioCommanderPrx::checkedCast(base);
      if (!qs1r)
         throw "Invalid proxy";
      Timer1->Enabled = true;
   }
   catch (const Ice::Exception& ex)
   {

      trace(std::string("Connection to QS1R failed : ") + ex.what());
      if (!failSeen)
      {
         failSeen = true;
         ShowMessage(ex.what());
      }
   }
   catch (const char* msg)
   {
      trace(std::string("Connection to QS1R failed : ") + msg);
      if (!failSeen)
      {
         failSeen = true;
         ShowMessage(msg);
      }
   }
}
__fastcall TQRigSyncMain::~TQRigSyncMain()
{
   if (ic)
      ic->destroy();
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::CloseButtonClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
long TQRigSyncMain::getQS1RFreq()
{
   float freq = 0.0;
   try
   {
      freq = qs1r->getFrequency(0);
      QFLabel->Caption = String(freq);
   }
   catch (const Ice::Exception& ex)
   {
      trace(std::string("QS1R get frequency failed : ") + ex.what());
      ShowMessage(ex.what());
   }
   catch (const char* msg)
   {
      trace(std::string("QS1R  get frequency failed : ") + msg);
      ShowMessage(msg);
   }
   return (long)freq;
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
   /*long freq =*/ getQS1RFreq();
   inTimer = false;
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::MuteButtonClick(TObject *Sender)
{
   try
   {
      qs1r->setMute(0, !muted);
      muted = !muted;
      MuteButton->Caption = muted?String("UnMute"):String("Mute");
   }
   catch (const Ice::Exception& ex)
   {
      trace(std::string("QS1R mute failed : ") + ex.what());
      ShowMessage(ex.what());
   }
   catch (const char* msg)
   {
      trace(std::string("QS1R mute failed : ") + msg);
      ShowMessage(msg);
   }
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
   RigLabel->Caption = OmniRig->Rig1->RigType;
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   //
}
//---------------------------------------------------------------------------

void __fastcall TQRigSyncMain::TransferButtonClick(TObject *Sender)
{
   if (qs1r)
   {
      long freq = getQS1RFreq();
      if (freq < 1)
      {
         return;
      }
      OmniRig->Rig1->FreqA = freq;
   }
}
//---------------------------------------------------------------------------
void __fastcall TQRigSyncMain::OmniRigRigTypeChange(TObject *Sender, long RigNumber)
{
   RigLabel->Caption = OmniRig->Rig1->RigType;
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
   bool ptt = OmniRig->Rig1->Tx & RigParamX::PM_TX;
   if (qs1r)
   {
      try
      {
         if (muted != ptt)
         {
            qs1r->setMute(0, !muted);
            muted = !muted;
            MuteButton->Caption = muted?String("UnMute"):String("Mute");
         }
      }
      catch (const Ice::Exception& ex)
      {
         trace(std::string("QS1R mute failed : ") + ex.what());
         ShowMessage(ex.what());
      }
      catch (const char* msg)
      {
         trace(std::string("QS1R mute failed : ") + msg);
         ShowMessage(msg);
      }
   }

}
//---------------------------------------------------------------------------

