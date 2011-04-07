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

#include "TManageListsDlg.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManageListsDlg *ManageListsDlg;
//---------------------------------------------------------------------------
__fastcall TManageListsDlg::TManageListsDlg( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void TManageListsDlg::DrawList()
{
   CloseListButton->Enabled = false;
   EditListButton->Enabled = false;
   ListsListBox->Clear();
   for ( ListSlotIterator i = TContestApp::getContestApp() ->listSlotList.begin(); i != TContestApp::getContestApp() ->listSlotList.end(); i++ )
   {
      if ( ( *i ) && ( *i ) ->slot )
      {
         ListsListBox->AddItem( ( *i ) ->slot->name.c_str(), ( TObject * ) ( ( *i ) ->slotno ) );
         CloseListButton->Enabled = true;
         EditListButton->Enabled = true;
      }
   }
   if ( ListsListBox->Items->Count )
   {
      ListsListBox->ItemIndex = 0;
   }
   else
      ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TManageListsDlg::FormShow( TObject */*Sender*/ )
{
   MinosParameters::getMinosParameters() ->applyFontMultiplier(this);
   DrawList();
}
//---------------------------------------------------------------------------
void __fastcall TManageListsDlg::CloseListButtonClick( TObject */*Sender*/ )
{
   unsigned int slotno = ( unsigned int ) ( ListsListBox->Items->Objects[ ListsListBox->ItemIndex ] );
   for ( ListSlotIterator i = TContestApp::getContestApp() ->listSlotList.begin(); i != TContestApp::getContestApp() ->listSlotList.end(); i++ )
   {
      if ( ( *i ) && ( *i ) ->slotno == slotno )
      {
         TContestApp::getContestApp() ->closeListFile( ( *i ) ->slot );
         ( *i ) ->slot = 0;
         break;
      }
   }
   DrawList();
}
//---------------------------------------------------------------------------
void __fastcall TManageListsDlg::EditListButtonClick( TObject *Sender )
{
   // Close list, run editor, reload list
   std::string fname;
   unsigned int slotno = ( unsigned int ) ( ListsListBox->Items->Objects[ ListsListBox->ItemIndex ] );
   for ( ListSlotIterator i = TContestApp::getContestApp() ->listSlotList.begin(); i != TContestApp::getContestApp() ->listSlotList.end(); i++ )
   {
      if ( ( *i ) && ( *i ) ->slotno == slotno )
      {
         fname = ( *i ) ->slot->cfileName;
         break;
      }
   }
   if ( fname.size() )
   {
      CloseListButtonClick( Sender );

      // run editor on the file
      std::string editor;
      TContestApp::getContestApp() ->displayBundle.getStringProfile( edpEditor, editor );

      std::string outfname;
      baseRunApp( editor, fname, "", outfname, true, false );

      TContestApp::getContestApp() ->openListFile( fname, slotno );
   }
   DrawList();
}
//---------------------------------------------------------------------------

