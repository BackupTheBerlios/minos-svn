/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MinosHelp.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMinosHelpForm *MinosHelpForm;
//---------------------------------------------------------------------------
__fastcall TMinosHelpForm::TMinosHelpForm( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TMinosHelpForm::CloseButtonClick( TObject */*Sender*/ )
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
