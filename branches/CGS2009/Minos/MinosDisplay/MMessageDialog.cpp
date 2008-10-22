/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#include "display_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
__fastcall TMessageDialog::TMessageDialog( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
bool mShowYesNoMessage( TComponent* Owner, String mess )
{
   TMessageDialog * MessageDialog = new TMessageDialog( Owner );
   MessageDialog->MessageLabel->Caption = mess;
   int res = MessageDialog->ShowModal();
   delete MessageDialog;
   return res == mrOk;
}
//---------------------------------------------------------------------------

