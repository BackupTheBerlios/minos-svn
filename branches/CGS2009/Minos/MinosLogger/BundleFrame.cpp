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

#include "ContestDetails.h"
#include "gridhint.h"
#include "EntryOptions.h"
#include "SettingsEditor.h"
#include <IniFiles.hpp>
#include "BundleFrame.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Hstcbo"
#pragma resource "*.dfm"
TSettingBundleFrame *SettingBundleFrame;
//---------------------------------------------------------------------------
__fastcall TSettingBundleFrame::TSettingBundleFrame( TComponent* Owner )
      : TFrame( Owner ), bundle( 0 ), bname( 0 )
{
   // needs initialising with a name, a file and a section
}
//---------------------------------------------------------------------------
void TSettingBundleFrame::initialise()
{
   std::string csect = bundle->getSection().c_str();
   std::vector<std::string> slist = bundle->getSections( );
   BundleSection->Clear();

   int offset = -1;
   for ( unsigned int i = 0; i < slist.size(); i++ )
   {
      if ( csect == slist[ i ] )
      {
         offset = i;
      }
      BundleSection->Items->Add( slist[ i ].c_str() );
   }
   if ( offset >= 0 )
   {
      BundleSection->ItemIndex = offset;
   }
   else
   {
      BundleSection->ItemIndex = 0;
      BundleSectionChange( this );
   }
}
void TSettingBundleFrame::initialise( String cap, SettingsBundle *b, MinosItem<std::string> *name )
{
   if ( b )
   {
      bundle = b;
      bname = name;
      // here we need to read in the histories
      BundleNameLabel->Caption = cap;
      initialise();
   }
}
//---------------------------------------------------------------------------
bool TSettingBundleFrame::doEdit( TObject */*Sender*/ )
{
   std::auto_ptr <TSettingsEditDlg> ed ( new TSettingsEditDlg( this, bundle ) );
   if ( ed->ShowModal() == mrOk )
   {
      initialise();
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
void TSettingBundleFrame::enableBundle( bool state )
{
   BundleNameLabel->Enabled = state;
   BundleSection->Enabled = state;
   BundleEdit->Enabled = state;
}
//---------------------------------------------------------------------------
void __fastcall TSettingBundleFrame::BundleSectionChange( TObject */*Sender*/ )
{
   if ( bundle )
   {
      bundle->openSection( BundleSection->Text.t_str() );
      bname->setValue( bundle->getSection() );
   }
}
//---------------------------------------------------------------------------

void __fastcall TSettingBundleFrame::BundleEditClick( TObject *Sender )
{
   doEdit( Sender );
}
//---------------------------------------------------------------------------

