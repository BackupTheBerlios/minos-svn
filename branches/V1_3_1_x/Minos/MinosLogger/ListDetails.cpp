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

#include "ListDetails.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
__fastcall TContactListDetails::TContactListDetails( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TContactListDetails::CloseButtonClick( TObject */*Sender*/ )
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
bool TContactListDetails::getDetails( ContactList * ct )
{
   ct->name = ListNameEdit->Text.c_str();
   //   ct->loc.loc.setValue(LocatorEdit->Text.c_str());
   //   ct->loc.validate();
   return true;
}
//---------------------------------------------------------------------------
void TContactListDetails::setDetails( ContactList * ct )
{
   ListNameEdit->Text = ct->name.c_str();
   //   LocatorEdit->Text = ct->loc.loc.getValue().c_str();
}
//---------------------------------------------------------------------------

