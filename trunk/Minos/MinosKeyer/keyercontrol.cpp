/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "MinosVK_pch.h"
#pragma hdrstop
#include <inifiles.hpp>
#include "LogEvents.h"

#include "keyctrl.h"
#include "vkmixer.h"
#include "keyercontrol.h"
#include "KeyerAbout.h"
#include "KeyerRPCServer.h"
#include "WaveShow.h"
#include "lineEvents.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DrawBox"
#pragma link "FBVUMeter"
#pragma link "xTrackBar" 
#pragma link "tspanel"
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
TKeyControlForm *KeyControlForm = 0;
//---------------------------------------------------------------------------
#warning Need to be able to record received audio, low(ish) quality, compressed, and play time based segments on request
#warning Want to make CW settings (tone, speed) configurable 
//---------------------------------------------------------------------------
void FAR PASCAL lcallback( bool pPTT, bool pkeyline, bool pPTTRef, bool pL1Ref, bool pL2Ref )
{
   KeyControlForm->PTT = pPTT;
   KeyControlForm->keyline = pkeyline;
   KeyControlForm->PTTRef = pPTTRef;
   KeyControlForm->L1Ref = pL1Ref;
   KeyControlForm->L2Ref = pL2Ref;
}
//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::syncSetLines()
{
   PTTOutCheckBox->Checked = PTT;
   KeyCheckBox->Checked = keyline;
   PTTReflectCheckBox->Checked = PTTRef;
   L1ReflectCheckBox->Checked = L1Ref;
   L2ReflectCheckBox->Checked = L2Ref;
}
//---------------------------------------------------------------------------
// NB the VU meters do attempt a logarithmic scale; we have
// red     +0  - -6
// yellow  -6  - -12
// lime    -12 - -50
//---------------------------------------------------------------------------
bool inrecvolcallback = false;
void FAR PASCAL recvolcallback( unsigned int vol )
{
   if ( !inrecvolcallback )
      PostMessage( KeyControlForm->Handle, CM_INPUTLEVEL, 0, vol );
   Application->ProcessMessages();
}
void __fastcall TKeyControlForm::CMInputLevel( TMessage &Message )
{
   inrecvolcallback = true;
   CommonFBVUMeter->SetValue( Message.LParam / 65536.0, Message.LParam / 65536.0 );
   inrecvolcallback = false;
}
//---------------------------------------------------------------------------
bool inoutvolcallback = false;
void FAR PASCAL outvolcallback( unsigned int vol )
{
   if ( !inoutvolcallback )
      PostMessage( KeyControlForm->Handle, CM_OUTPUTLEVEL, 0, vol );
   Application->ProcessMessages();
}
void __fastcall TKeyControlForm::CMOutputLevel( TMessage &Message )
{
   inoutvolcallback = true;
   CommonFBVUMeter->SetValue( Message.LParam / 65536.0, Message.LParam / 65536.0 );
   inoutvolcallback = false;
}
//---------------------------------------------------------------------------
__fastcall TKeyControlForm::TKeyControlForm( TComponent* Owner )
      : TForm( Owner ), saveResize( false )
{
   enableTrace( ".\\TraceLog\\MinosKeyer" );
   SetPriorityClass( GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS );  // Win2K and Above only!

   std::vector<boost::shared_ptr<VKMixer> > mixers = VKMixer::getMixers( Handle );   // initialisae the VKMixer system

   OutputMixerComboBox->Enabled = !LockMixersCB->Checked;

   populateMixerList();

   loadKeyers();

   setLineCallBack( lcallback );
   setVUCallBack( recvolcallback, outvolcallback );

   TMessage Message;
   MMMixControlChange( Message );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::RecordButtonClick( TObject */*Sender*/ )
{
   int fno = KeyCombo->Text.ToIntDef( 1 );
   if ( fno >= 1 && fno <= 12 )
   {
      startRecordDVPFile( fno );
      recind->Caption = "Push PTT to Commence Recording";
      KeyerServer::publishCommand( recind->Caption.c_str() );
      recordWait = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::MonitorButtonClick( TObject */*Sender*/ )
{
   // We want this to monitor microphone level, but not to record it
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::StopButtonClick( TObject */*Sender*/ )
{
   if ( recordWait )
   {
      recind->Caption = "";
      recordWait = false;
   }
   if ( recording )
   {
      finishRecordDVPFile();
      recind->Caption = "";
      recording = false;
   }
   else
      stopKeyer();
   KeyerServer::publishCommand( recind->Caption.c_str() );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::PlayButtonClick( TObject */*Sender*/ )
{
   int fno = KeyCombo->Text.ToIntDef( 1 );
   if ( fno >= 1 && fno <= 12 )
   {
      recind->Caption = "";
      playKeyerFile( fno, false );
   }
   KeyerServer::publishCommand( recind->Caption.c_str() );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::FormClose( TObject *Sender, TCloseAction &/*Action*/ )
{
   saveResize = false;

   unloadKeyers();
   VKMixer::closeMixer();

   {
//      disableInterrupts guard;	// unloadKeyers killed the critical section
      LineSet *ls = LineSet::GetLineSet();
      delete ls;
      ls = 0;

      delete KS;
      KS = 0;
   }

   LogTimerTimer( Sender );
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------

char *msets[] = {"emsUnloaded", "emsPassThroughNoPTT", "emsPassThroughPTT",
                 "emsReplay", "emsReplayPip", "emsReplayT1", "emsReplayT2",
                 "emsVoiceRecord",
                 "emsCWTransmit", "emsCWPassThrough",
                 "emsMicMonitor", "emsReplayMonitor",
                 "emsMaxMixerSet"
                };

void __fastcall TKeyControlForm::LineTimerTimer( TObject */*Sender*/ )
{
   syncSetLines();
   bool PTT = getPTT();
   if ( recordWait && PTT )
   {
      recind->Caption = "Release PTT to stop Recording";
      recordWait = false;
      recording = true;
   }
   else
      if ( recording && !PTT )
      {
         recind->Caption = "";
         recordWait = false;
         recording = false;
      }
   KeyerServer::publishCommand( recind->Caption.c_str() );
   eMixerSets m = GetCurrentMixerSet();

   std::string astate;
   getActionState( astate );

   // This isn't quite what we want - needs to be better english and slower changing

   std::string kstatus;
   if ( getKeyerStatus( kstatus ) )
      Caption = String( msets[ m ] ) + " : " + astate.c_str() + " : " + kstatus.c_str();
   else
      Caption = String( msets[ m ] ) + " : " + astate.c_str();

   static String old;

   if ( Caption != old )
   {
      old = Caption;
      CaptionTimer->Enabled = true;
   }

}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::MMMixLineChange( TMessage &/*Message*/ )
{}
//---------------------------------------------------------------------------
static std::string hexVal( DWORD d )
{
   char buff[ 20 ];
   sprintf( buff, "0x%x", d );
   return std::string( buff );
}
void __fastcall TKeyControlForm::MMMixControlChange( TMessage & /*Message*/ )
{
   VKMixer * vkm = VKMixer::GetInputVKMixer();
   if ( vkm )
   {
      DWORD micrec;
      if ( vkm->GetMicRecVolume( micrec ) )
      {
         MicRecTrackBar->Visible = true;
         Label9->Visible = true;
         MicRecTrackBar->Position = 0xFFFF - micrec;
         Label9->Caption = String( micrec );
      }
      else
      {
         MicRecTrackBar->Visible = false;
         Label9->Visible = false;
      }


      DWORD rec;
      if ( vkm->GetRecVolume( rec ) )
      {
         RecordTrackBar->Visible = true;
         Label10->Visible = true;
         RecordTrackBar->Position = 0xFFFF - rec;
         Label10->Caption = String( rec );
      }
      else
      {
         RecordTrackBar->Visible = false;
         Label10->Visible = false;
      }

      /*
      DWORD muxId;
      std::string sInputName;
      if (vkm->GetMuxInput(muxId, sInputName))
      {
         MUXSelection->Visible = true;
         MUXSelection->Caption = (sInputName + " " + hexVal(muxId)).c_str();
      }
      else
      {
         MUXSelection->Visible = false;
      }
      */
   }
   vkm = VKMixer::GetOutputVKMixer();
   if ( vkm )
   {
      DWORD micout;
      if ( vkm->GetMicOutVolume( micout ) )
      {
         MicOutTrackBar->Visible = true;
         Label11->Visible = true;
         MicOutTrackBar->Position = 0xFFFF - micout;
         Label11->Caption = String( micout );
      }
      else
      {
         MicOutTrackBar->Visible = false;
         Label11->Visible = false;
      }

      DWORD waveout;
      if ( vkm->GetWaveOutVolume( waveout ) )
      {
         WaveOutTrackBar->Visible = true;
         Label16->Visible = true;
         WaveOutTrackBar->Position = 0xFFFF - waveout;
         Label16->Caption = String( waveout );
      }
      else
      {
         WaveOutTrackBar->Visible = false;
         Label16->Visible = false;
      }

      DWORD master;
      if ( vkm->GetMasterVolume( master ) )
      {
         OutputTrackBar->Visible = true;
         Label12->Visible = true;
         OutputTrackBar->Position = 0xFFFF - master;
         Label12->Caption = String( master );
      }
      else
      {
         OutputTrackBar->Visible = false;
         Label12->Visible = false;
      }

      bool micoutmute;
      if ( vkm->GetMicOutMute( micoutmute ) )
      {
         MicOutMute->Visible = true;
         MicOutMute->Checked = micoutmute;
      }
      else
      {
         MicOutMute->Visible = false;
      }

      bool waveoutmute;
      if ( vkm->GetWaveOutMute( waveoutmute ) )
      {
         WaveOutMute->Visible = true;
         WaveOutMute->Checked = waveoutmute;
      }
      else
      {
         WaveOutMute->Visible = false;
      }

      bool mastermute;
      if ( vkm->GetMasterMute( mastermute ) )
      {
         MasterMute->Visible = true;
         MasterMute->Checked = mastermute;
      }
      else
      {
         MasterMute->Visible = false;
      }

   }
   SetBaseMixerLevels();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::MicRecTrackBarChange( TObject */*Sender*/ )
{
   VKMixer * vkm = VKMixer::GetInputVKMixer();
   if ( vkm )
   {
      DWORD mic;
      if ( vkm->GetMicRecVolume( mic ) )
      {
         mic = 0xFFFF - MicRecTrackBar->Position;
         vkm->SetMicRecVolume( mic );
         SetBaseMixerLevels();
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::MicOutTrackBarChange( TObject */*Sender*/ )
{
   VKMixer * vkm = VKMixer::GetInputVKMixer();
   if ( vkm )
   {
      DWORD mic;
      if ( vkm->GetMicOutVolume( mic ) )
      {
         mic = 0xFFFF - MicOutTrackBar->Position;
         vkm->SetMicOutVolume( mic );
         SetBaseMixerLevels();
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::WaveOutTrackBarChange( TObject */*Sender*/ )
{
   VKMixer * vkm = VKMixer::GetOutputVKMixer();
   if ( vkm )
   {
      DWORD rec;
      if ( vkm->GetWaveOutVolume( rec ) )
      {
         rec = 0xFFFF - WaveOutTrackBar->Position;
         vkm->SetWaveOutVolume( rec );
         SetBaseMixerLevels();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::RecordTrackBarChange( TObject */*Sender*/ )
{
   VKMixer * vkm = VKMixer::GetOutputVKMixer();
   if ( vkm )
   {
      DWORD rec;
      if ( vkm->GetRecVolume( rec ) )
      {
         rec = 0xFFFF - RecordTrackBar->Position;
         vkm->SetRecVolume( rec );
         SetBaseMixerLevels();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::OutputTrackBarChange( TObject */*Sender*/ )
{
   VKMixer * vkm = VKMixer::GetOutputVKMixer();
   if ( vkm )
   {
      DWORD master;
      if ( vkm->GetMasterVolume( master ) )
      {
         master = 0xFFFF - OutputTrackBar->Position;
         vkm->SetMasterVolume( master );
         SetBaseMixerLevels();
      }
   }

}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::FormShow( TObject */*Sender*/ )
{
   StartupTimer->Enabled = true;
   PipCheckBox->Checked = getPipEnabled();
   PageControl1->ActivePage = VoiceTab;

   DelayEdit->Text = String(getAutoRepeatDelay());
   AutoRepeatCheckBox->Checked = getEnableAutoRepeat();

   SpeedControl->Text = String(getCWSpeed());
   ToneEdit->Text = getCWTone();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::AutoRepeatCheckBoxClick( TObject */*Sender*/ )
{
   setEnableAutoRepeat( AutoRepeatCheckBox->Checked );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::DelayEditChange( TObject */*Sender*/ )
{
   String temp = DelayEdit->Text;
   int def = getAutoRepeatDelay();
   int val = temp.ToIntDef(def);
   if (val > 0)
   {
      setAutoRepeatDelay( val );
   }
}
//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::PipCheckBoxClick( TObject */*Sender*/ )
{
   setPipEnabled( PipCheckBox->Checked );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::SendButtonClick( TObject */*Sender*/ )
{
   sendCW( AnsiUpperCase( Memo1->Lines->Text.Trim() ).c_str(), SpeedControl->Text.ToIntDef(12), ToneEdit->Text.ToIntDef( 1000 ) );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::ClearButtonClick( TObject */*Sender*/ )
{
   Memo1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::ToneEditKeyPress( TObject */*Sender*/, char &Key )
{
   // Force numeric
   if ( ( Key < '0' || Key > '9' ) && ( Key != '\b' ) )
      Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::Tone1ButtonClick( TObject */*Sender*/ )
{
   sendTone1();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::Tone2ButtonClick( TObject */*Sender*/ )
{
   sendTone2();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::AboutButtonClick( TObject */*Sender*/ )
{
   TAboutBox * AboutBox = new TAboutBox( this );
   AboutBox->ShowModal();
   delete AboutBox;
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::ConnectTimerTimer( TObject */*Sender*/ )
{
   KeyerServer::checkConnection();
}
//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::LogTimerTimer( TObject */*Sender*/ )
{
   /*
      if (Visible && ParamCount() >= 1)
      {
         String p = ParamStr(1);
         if (p.UpperCase().Pos("/H"))
            Visible = false;
      }
   */ 
   // check log queue; if anything on it then log to main window
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         trace( "Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }
   static bool closed = false;
   if ( !closed )
   {
      if ( checkCloseEvent() )
      {
         closed = true;
         Close();
      }
   }

}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::FormKeyUp( TObject */*Sender*/, WORD &Key,
      TShiftState Shift )
{
   if ( ( Shift.Contains( ssCtrl ) || Shift.Contains( ssShift ) ) && Key >= VK_F1 && Key <= VK_F12 )
   {
      if ( Shift.Contains( ssCtrl ) && Shift.Contains( ssShift ) )
      {
         // keyer record keys
         startRecordDVPFile( Key - VK_F1 + 1 );
      }
      else
      {
         // Keyer play keys
         playKeyerFile( Key - VK_F1 + 1, true );    // do actual transmit, and repeat as required
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::FormCloseQuery( TObject */*Sender*/, bool &/*CanClose*/ )
{
   // need to start killing the keyer if it is running
   stopKeyer();
}
//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::closeShow()
{
   WaveShowForm = 0;
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::ShowButtonClick( TObject */*Sender*/ )
{
   if ( !WaveShowForm )
   {
      WaveShowForm = new TWaveShowForm( this );
      WaveShowForm->Top = Top + 50;
      WaveShowForm->Left = Left + 50;
      int fno = KeyCombo->Text.ToIntDef( 1 );
      WaveShowForm->setSamples( fno );
      WaveShowForm->Show();
   }
   else
   {
      WaveShowForm->Close();
      delete WaveShowForm;
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::CWBrowseClick( TObject */*Sender*/ )
{
   CWOpenDialog->FileName = CWFileEdit->Text;
   if ( CWOpenDialog->Execute() )
   {
      CWFileEdit->Text = CWOpenDialog->FileName;
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::SendFileButtonClick( TObject *Sender )
{
   try
   {
      Memo1->Lines->LoadFromFile ( CWFileEdit->Text );
   }
   catch ( Exception & e )
   {
      ShowMessage( "Failed to load file " + CWFileEdit->Text + " : " + e.Message );
      return ;
   }
   SendButtonClick( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;

   ForceDirectories( ".\\Configuration" );
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );

   int L = Left, T = Top, W = Width, H = Height;
   L = iniFile->ReadInteger( "Display", "Left", L );
   T = iniFile->ReadInteger( "Display", "Top", T );
   W = iniFile->ReadInteger( "Display", "Width", W );
   H = iniFile->ReadInteger( "Display", "Height", H );
   SetBounds( L, T, W, H );
   Repaint();
   saveResize = true;
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
      iniFile->WriteInteger( "Display", "Left", Left );
      iniFile->WriteInteger( "Display", "Top", Top );
      iniFile->WriteInteger( "Display", "Width", Width );
      iniFile->WriteInteger( "Display", "Height", Height );
      iniFile->UpdateFile();
   }
}
void __fastcall TKeyControlForm::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::CaptionTimerTimer( TObject */*Sender*/ )
{
   CaptionTimer->Enabled = false;
   KeyerServer::publishState( Caption.c_str() );
}
//---------------------------------------------------------------------------
void __fastcall TKeyControlForm::SpeedControlChange( TObject */*Sender*/ )
{
   // write to CWSpeed
   setCWSpeed( SpeedControl->Text.ToIntDef(12) );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::ToneEditChange( TObject */*Sender*/ )
{
   //   write to CWTone
   setCWTone( ToneEdit->Text.ToIntDef( 12 ) );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::InputMixerComboBoxChange( TObject */*Sender*/ )
{
   unloadKeyers();
   VKMixer::closeMixer();

   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
   iniFile->WriteString( "Mixers", "InputId", InputMixerComboBox->Text );

   Close();
   //   populateMixerList();
   //   loadKeyers();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::SourceLineComboBoxChange( TObject */*Sender*/ )
{
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
   iniFile->WriteString( "Mixers", "InputLine", SourceComboBox->Text );
   VKMixer::GetInputVKMixer() ->setInputMixerLine( SourceComboBox->Text.c_str() );
   TMessage msg;
   MMMixControlChange( msg );
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::OutputMixerComboBoxChange( TObject */*Sender*/ )
{
   unloadKeyers();
   VKMixer::closeMixer();

   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
   iniFile->WriteString( "Mixers", "OutputId", OutputMixerComboBox->Text );

   Close();
   //   populateMixerList();
   //   loadKeyers();
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::DestinationComboBoxChange( TObject */*Sender*/ )
{
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
   iniFile->WriteString( "Mixers", "OutputLine", DestinationComboBox->Text );
   VKMixer::GetOutputVKMixer() ->setOutputMixerLine( DestinationComboBox->Text.c_str() );
   TMessage msg;
   MMMixControlChange( msg );
}
//---------------------------------------------------------------------------

void TKeyControlForm::populateMixerList()
{
   InputMixerComboBox->Items->Clear();
   OutputMixerComboBox->Items->Clear();
   SourceComboBox->Items->Clear();
   DestinationComboBox->Items->Clear();

   std::vector<boost::shared_ptr<VKMixer> > mixers = VKMixer::getMixers( Handle );
   for ( std::vector<boost::shared_ptr<VKMixer> >::iterator i = mixers.begin(); i != mixers.end(); i++ )
   {
      std::string name = ( *i ) ->getMixerName();
      InputMixerComboBox->Items->Add( name.c_str() );
      OutputMixerComboBox->Items->Add( name.c_str() );
   }
   InputMixerComboBox->ItemIndex = 0;
   OutputMixerComboBox->ItemIndex = 0;
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
   String inputMixer = iniFile->ReadString( "Mixers", "InputId", InputMixerComboBox->Text );
   String outputMixer;

   if ( LockMixersCB->Checked )
   {
      outputMixer = inputMixer;
   }
   else
      outputMixer = iniFile->ReadString( "Mixers", "OutputId", OutputMixerComboBox->Text );

   InputMixerComboBox->ItemIndex = InputMixerComboBox->Items->IndexOf( inputMixer );
   OutputMixerComboBox->ItemIndex = OutputMixerComboBox->Items->IndexOf( outputMixer );

   VKMixer *inMix = VKMixer::setInputMixer( inputMixer.c_str() );
   if ( inMix )
   {
      populateInputMixer();
      String inputMixerLine = iniFile->ReadString( "Mixers", "InputLine", SourceComboBox->Text );
      inMix->setInputMixerLine( inputMixerLine.c_str() );
   }

   VKMixer *outMix = VKMixer::setOutputMixer( outputMixer.c_str() );
   if ( outMix )
   {
      populateOutputMixer();
      String outputMixerLine = iniFile->ReadString( "Mixers", "OutputLine", DestinationComboBox->Text );
      outMix->setOutputMixerLine( outputMixerLine.c_str() );
   }

}
//---------------------------------------------------------------------------
void TKeyControlForm::populateInputMixer()
{
   SourceComboBox->Clear();

   VKMixer *inMix = VKMixer::GetInputVKMixer();
   if ( inMix )
   {
      std::vector<std::string> reclines = inMix->getInputLines();

      for ( std::vector<std::string>::iterator i = reclines.begin(); i != reclines.end(); i++ )
      {
         String item = ( *i ).c_str();
         SourceComboBox->Items->Add( item );
      }
      SourceComboBox->ItemIndex = 0;
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
      String inputMixerLine = iniFile->ReadString( "Mixers", "InputLine", SourceComboBox->Text );

      int ip = SourceComboBox->Items->IndexOf( inputMixerLine );
      if ( ip < 0 )
      {
         ip = 0;
         inputMixerLine = SourceComboBox->Items->Strings[ 0 ].c_str();
      }
      VKMixer::GetInputVKMixer() ->setInputMixerLine( inputMixerLine.c_str() );
      SourceComboBox->ItemIndex = ip;

      TMessage msg;
      MMMixControlChange( msg );
      SaveMixerSettings( ".\\InputMixerSettings.xml" );
   }
}
//---------------------------------------------------------------------------
void TKeyControlForm::populateOutputMixer()
{
   DestinationComboBox->Clear();

   VKMixer *outMix = VKMixer::GetOutputVKMixer();

   if ( outMix )
   {
      std::vector<std::string> destlines = outMix->getOutputLines();

      for ( std::vector<std::string>::iterator i = destlines.begin(); i != destlines.end(); i++ )
      {
         String item = ( *i ).c_str();
         DestinationComboBox->Items->Add( item );
      }
      DestinationComboBox->ItemIndex = 0;
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosKeyer.ini" ) );
      String outputMixerLine = iniFile->ReadString( "Mixers", "OutputLine", DestinationComboBox->Text );

      int op = DestinationComboBox->Items->IndexOf( outputMixerLine );
      if ( op < 0 )
      {
         op = 0;
         outputMixerLine = DestinationComboBox->Items->Strings[ 0 ].c_str();
      }
      VKMixer::GetOutputVKMixer() ->setOutputMixerLine( outputMixerLine.c_str() );
      DestinationComboBox->ItemIndex = op;

      TMessage msg;
      MMMixControlChange( msg );
      SaveMixerSettings( ".\\OutputMixerSettings.xml" );
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyControlForm::LockMixersCBClick( TObject * /*Sender*/ )
{
   OutputMixerComboBox->Enabled = !LockMixersCB->Checked;
}
//---------------------------------------------------------------------------

