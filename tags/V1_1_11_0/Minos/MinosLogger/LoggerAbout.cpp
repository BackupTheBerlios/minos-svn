/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop

#include "RCVersion.h"
#include "GJVThreads.h"
#include "ConfigDM.h"
#include "GuardianThread.h"
#include "ConfigElementFrame.h"
#include "ConfigMainFrame.h"
#include "LoggerAbout.h" 
//---------------------------------------------------------------------
#pragma link "ConfigMainFrame"
#pragma resource "*.dfm" 
//---------------------------------------------------------------------
bool started = false;

String MinosText =
   "The Inferno, Dante, (trans Dorothy L Sayers, Penguin Classics); Canto 5.\r\n"
   "\r\n"
   "\"From the first circle thus I came descending\r\n"
   "To the second, which, in narrower compass turning,\r\n"
   "Holds greater woe, with outcry loud and rending.\"\r\n"
   "\r\n"
   "\"There in the threshold, horrible and giming,\r\n"
   "Grim Minos sits, holding his ghastly session,\r\n"
   "And, as he girds him, sentencing and spurning;\"\r\n"
   "\r\n"
   "\"For when the ill soul faces him, confession\r\n"
   "Pours out of it till nothing's left to tell;\r\n"
   "Whereon that connoisseur of all transgression\"\r\n"
   "\r\n"
   "\"Assigns it to its proper place in hell,\r\n"
   "As many grades as he would have it fall,\r\n"
   "So oft he belts him round with his own tail.\"\r\n"
   "\r\n"
   "\"Before him stands a throng continual;\r\n"
   "Each comes in turn to abye the fell arraign;\r\n"
   "They speak - they hear - they're whirled down one and all.\"\r\n"
   "\r\n"
   "So when I was thinking a name for a messaging server, it amused me to think of it belting messages round a field with its tail!\r\n"
   "\r\n"
   "Of course another quote from Dante is most apt for anyone thinking of becoming involved in this kind of project…\r\n"
   "\r\n"
   "“Abandon Hope, all ye who enter here”…"
   ;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox( TComponent *Owner, bool onStartup )
      : TForm( Owner ), doStartup(false)
{

   TMyRCVersion RCVersion;
   RCVersion.initialise();
   ProductName->Caption = RCVersion.ProductName;
   String minor = RCVersion.MinorVersion;
   bool Beta = ( minor.ToIntDef( 1 ) % 2 == 1 );
   Version->Caption = "Version " + RCVersion.FileVersion + ( Beta ? " BETA" : "" );
   Copyright->Caption = RCVersion.LegalCopyright;
   Comments->Caption = ( Beta ? "Beta version - use at your own risk!\r\n\r\n\r\n" : "" ) + RCVersion.Comments;

   if ( !FileExists( ".\\Configuration\\MinosConfig.ini" ) || !onStartup || checkServerReady() )
   {
      AutoStartTabSheet->TabVisible = false;
      LoggerOnlyButton->Visible = false;
   }
   else
   {
      TConfigFrame1->StartButton->Enabled = !started;
      TConfigFrame1->StopButton->Visible = false;
      doStartup = true;
   }

}
//---------------------------------------------------------------------
/*static*/bool TAboutBox::ShowAboutBox( TComponent *Owner,  bool onStartup )
{
   std::auto_ptr <TAboutBox> aboutBox( new TAboutBox( Owner, onStartup ) );
   return aboutBox->ShowModal() == mrOk;
}
//---------------------------------------------------------------------

void __fastcall TAboutBox::FormShow( TObject */*Sender*/ )
{
   PageControl1->ActivePage = AboutTabSheet;
   TConfigFrame1->initialise();
   //   ScaleBy( TContestApp::getContestApp() ->sysfont->Size, Font->Size );
   //   Font->Assign( TContestApp::getContestApp() ->sysfont );
   WebLabel->Font->Color = clBlue;
   WebLabel->Font->Style = WebLabel->Font->Style << fsUnderline;

   MinosMemo->Text = MinosText;
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::WebLabelClick( TObject */*Sender*/ )
{
   ShellExecute( Handle, "open", WebLabel->Caption.c_str(), 0, 0, SW_SHOWNORMAL );   //The home page for this program
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::TConfigFrame1StartButtonClick( TObject *Sender )
{
   TConfigFrame1->StartButton->Enabled = false;
   TConfigFrame1->StartButtonClick( Sender );
   TConfigFrame1->StopButton->Enabled = true;
   started = true;
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::FormClose( TObject */*Sender*/, TCloseAction &/*Action*/ )
{
   if ( !started && doStartup )
   {
      // auto start on first run, but only if we gave that option
      TConfigFrame1StartButtonClick( this );
   }
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::TConfigFrame1StopButtonClick( TObject *Sender )
{
   started = false;
   TConfigFrame1->StopButtonClick( Sender );
   TConfigFrame1->StartButton->Enabled = true;
   TConfigFrame1->StopButton->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::LoggerOnlyButtonClick( TObject */*Sender*/ )
{
   doStartup = false;
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::TConfigFrame1CancelButtonClick( TObject */*Sender*/ )
{
   // cancel the start up
   doStartup = false;
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


