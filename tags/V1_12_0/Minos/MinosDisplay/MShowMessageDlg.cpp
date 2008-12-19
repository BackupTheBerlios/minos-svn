/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "display_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
void mShowMessage( String mess, TComponent* Owner )
{
   std::auto_ptr<TMinosShowMessage> mshowMessage( new TMinosShowMessage( Owner ) );
   mshowMessage->Label1->Caption = mess;
   mshowMessage->ShowModal();
}
//---------------------------------------------------------------------------
__fastcall TMinosShowMessage::TMinosShowMessage( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TMinosShowMessage::Button1Click( TObject * /*Sender*/ )
{
   Close();
}
//---------------------------------------------------------------------------
