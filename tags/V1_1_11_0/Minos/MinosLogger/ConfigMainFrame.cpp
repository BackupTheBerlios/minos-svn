/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------

#include "base_pch.h"
#pragma hdrstop

#include "ConfigDM.h"
#include "GuardianThread.h"
#include "ConfigElementFrame.h"
#include "ConfigMainFrame.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTConfigFrame *TConfigFrame;
//---------------------------------------------------------------------------
__fastcall TTConfigFrame::TTConfigFrame( TComponent* Owner )
      : TFrame( Owner )
{}
void TTConfigFrame::initialise()
{
   int etop = 0;
   for ( std::vector <TConfigElement *>::iterator i = TMConfigDM::getConfigDM( 0 ) ->elelist.begin();
         i != TMConfigDM::getConfigDM( 0 ) ->elelist.end();
         i++ )
   {
      String sect = ( *i ) ->name;

      TConfigElementFrm *tcf = new TConfigElementFrm( this, ( *i ) );
      if ( tcf->initialise( sect ) )
      {
         tcf->Parent = ElementScrollBox;
         tcf->Top = etop;
         etop += tcf->Height;

         framelist.push_back( tcf );
      }
      else
         delete tcf;
   }
   StationIdEdit->Text = TMConfigDM::getConfigDM( 0 ) ->getCircleOfHell().c_str();
}
//---------------------------------------------------------------------------
void __fastcall TTConfigFrame::StartButtonClick( TObject *Sender )
{
   SetButtonClick( Sender );    // set the circle of hell
   for ( std::vector <TConfigElementFrm *>::iterator i = framelist.begin(); i != framelist.end(); i++ )
   {
      ( *i ) ->tce->run = ( *i ) ->RunCheckBox->Checked;
      if ( ( *i ) ->RunCheckBox->Checked )
      {
         ( *i ) ->tce->hide = HideCheckBox->Checked;
      }
   }

   TMConfigDM::getConfigDM( 0 ) ->start();
}
//---------------------------------------------------------------------------
void __fastcall TTConfigFrame::StopButtonClick( TObject */*Sender*/ )
{
   TMConfigDM::getConfigDM( 0 ) ->stop();
}
//---------------------------------------------------------------------------
void __fastcall TTConfigFrame::ClearAllButtonClick( TObject */*Sender*/ )
{
   for ( std::vector <TConfigElementFrm *>::iterator i = framelist.begin(); i != framelist.end(); i++ )
   {
      ( *i ) ->RunCheckBox->Checked = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TTConfigFrame::SetButtonClick( TObject */*Sender*/ )
{
   TMConfigDM::getConfigDM( 0 ) ->setCircleOfHell( StationIdEdit->Text.c_str() );
}
//---------------------------------------------------------------------------

void __fastcall TTConfigFrame::HideCheckBoxClick(TObject *Sender)
{
   // Make this active - need a hide/show event that is signalled for show
   // and then all "server" apps need to honour this.
   setShowServers(HideCheckBox->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TTConfigFrame::ModifyButtonClick(TObject *Sender)
{
   // Change the INI file to match what is shown
   for ( std::vector <TConfigElementFrm *>::iterator i = framelist.begin(); i != framelist.end(); i++ )
   {
      (*i)->tce->setRun(( *i ) ->RunCheckBox->Checked);
   }
}
//---------------------------------------------------------------------------

