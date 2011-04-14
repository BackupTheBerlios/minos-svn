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

#include "MinosVer.h"   // updated by SubWCRev

#include "RCVersion.h"
#include "KeyerAbout.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox( TComponent *Owner )
      : TForm( Owner )
{
   TMyRCVersion RCVersion;
   RCVersion.initialise();
   ProductName->Caption = RCVersion.ProductName;
   //   Version->Caption = "Version " + RCVersion.FileVersion;
   //   Copyright->Caption = RCVersion.LegalCopyright;
   //   Comments->Caption = RCVersion.Comments;

#ifdef BETA_VERSION
   bool Beta = true;
#else
   bool Beta = false;
#endif

   Version->Caption = "Version " + RCVersion.FileVersion + ( Beta ? " BETA" : "" );

   Copyright->Caption = RCVersion.LegalCopyright;
   Comments->Caption = ( Beta ? "Beta version - use at your own risk!\r\n\r\n\r\n" : "" ) + RCVersion.Comments;

}
//---------------------------------------------------------------------

void __fastcall TAboutBox::FormShow( TObject */*Sender*/ )
{
   WebLabel->Font->Color = clBlue;
   WebLabel->Font->Style = WebLabel->Font->Style << fsUnderline;
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::WebLabelClick( TObject */*Sender*/ )
{
   ShellExecute( Handle, "open", WebLabel->Caption.c_str(), 0, 0, SW_SHOWNORMAL );   //The home page for this program
}
//---------------------------------------------------------------------------

