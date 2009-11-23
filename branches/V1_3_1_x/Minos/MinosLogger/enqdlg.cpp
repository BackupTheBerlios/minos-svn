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

#include "enqdlg.h" 
//---------------------------------------------------------------------------
#pragma resource "*.dfm" 
//---------------------------------------------------------------------------
// dialog for simple enquiries that have a simple question/answer format
//---------------------------------------------------------------------------
__fastcall TEnquireDlg::TEnquireDlg( TComponent* Owner )
      : TForm( Owner ), intenq( false )
{}
//---------------------------------------------------------------------------
bool enquireDialog( TComponent* Owner, const String &prompt, String &Value, int limit )
{
   std::auto_ptr<TEnquireDlg> EnquireDlg( new TEnquireDlg( Owner ) );
   EnquireDlg->PromptLabel->Caption = prompt;
   EnquireDlg->EnqValue->Text = Value;
   EnquireDlg->EnqValue->MaxLength = ( limit > 0 ? limit : 0 );
   EnquireDlg->intenq = false;
   if ( EnquireDlg->ShowModal() == mrOk )
   {
      Value = EnquireDlg->EnqValue->Text;
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
bool enquireDialog( TComponent* Owner, const String &prompt, std::string &Value, int limit )
{
   String sv = Value.c_str();
   if ( enquireDialog( Owner, prompt, sv, limit ) )
   {
      Value = sv.c_str();
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
bool enquireDialog( TComponent* Owner, const String &prompt, int &Value )
{
   std::auto_ptr<TEnquireDlg> EnquireDlg( new TEnquireDlg( Owner ) );
   EnquireDlg->PromptLabel->Caption = prompt;
   EnquireDlg->EnqValue->Text = String( Value );
   EnquireDlg->intenq = true;
   if ( EnquireDlg->ShowModal() == mrOk )
   {
      Value = EnquireDlg->EnqValue->Text.ToIntDef( -1 );
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
void __fastcall TEnquireDlg::FormShow( TObject */*Sender*/ )
{
   ActiveControl = EnqValue;
}
//---------------------------------------------------------------------------

void __fastcall TEnquireDlg::EnqValueKeyPress( TObject */*Sender*/, char &Key )
{
   if ( intenq )
   {
      if ( Key != '\b' && ( Key < '0' || Key > '9' ) )
         Key = 0;
   }
   //
}
//---------------------------------------------------------------------------

