////////////////////////////////////////////////////////////////////////////
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

#include "LogEvents.h"
#include "LoggerContest.h"

#include "gridhint.h"
#include "LogFrame.h"
#include "LogMain.h"
#include "MatchContact.h"
#include "MatchThread.h"
#include "ForceLog.h"
#include "TreeUtils.h"
#include "enqdlg.h"
#include "MultDisp.h"
#include "EntryOptions.h"
#include "SettingsEditor.h"
#include "StatsDisp.h"
#include "ServerEvent.h"
#include "QSOEdit.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"
#include "XMPPRPCObj.h"
#include "RPCPubSub.h"
#include "SendRPCDM.h"
#include "bandlist.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BundleFrame"
#pragma link "MultDisp"
#pragma link "QSOFrame"
#pragma link "QSOLogFrame"
#pragma link "MonitorLogFrame"
#pragma resource "*.dfm"
TSingleLogFrame *SingleLogFrame;
int freqdelta = 2000;   // 2KHz before we "see" a freq change - must be user settable
// preserved, and mode dependant (ideally filter controlled?)
MatchNodeData::MatchNodeData() : top( false ), matchedContact( 0 )
{}
//---------------------------------------------------------------------------
__fastcall TSingleLogFrame::TSingleLogFrame( TComponent* Owner, BaseContestLog * contest )
      : TFrame( Owner ), contest( contest ),
      logColumnsChanged( false ),
      splittersChanged(false),
      currFreq( 0 ), oldFreq( 0 ),
      lastStanzaCount( 0 ),
      matchTreeClickNode( 0 ),
      otherTreeClickNode( 0 ),
      archiveTreeClickNode( 0 ),
      xferTree( 0 ),
      EL_SplitterChanged ( EN_SplittersChanged, & SplittersChanged_Event ),
      EL_LogColumnsChanged ( EN_LogColumnsChanged, & LogColumnsChanged_Event ),
      EL_ContestPageChanged ( EN_ContestPageChanged, & ContestPageChanged_Event ),
      EL_ReportOverstrike ( EN_ReportOverstrike, & ReportOverstrike_Event ),
      EL_AfterLogContact ( EN_AfterLogContact, & AfterLogContact_Event ),
      EL_AfterSelectContact ( EN_AfterSelectContact, & AfterSelectContact_Event ),

      EL_NextContactDetailsOnLeft ( EN_NextContactDetailsOnLeft, & NextContactDetailsOnLeft_Event ),
      EL_ContestDetails ( EN_ContestDetails, & ContestDetails_Event ),
      EL_GoToSerial ( EN_GoToSerial, & GoToSerial_Event ),
      EL_MakeEntry ( EN_MakeEntry, & MakeEntry_Event ),
      EL_SetTimeNow ( EN_SetTimeNow, & SetTimeNow_Event ),
      EL_NextUnfilled ( EN_NextUnfilled, & NextUnfilled_Event ),
      EL_FormKey ( EN_FormKey, & FormKey_Event ),
      EL_SetMode ( EN_SetMode, & SetMode_Event ),
      EL_SetFreq ( EN_SetFreq, & SetFreq_Event ),
      EL_EditMatchContact ( EN_EditMatchContact, & EditMatchContact_Event ),

      EL_ReplaceLogList ( EN_ReplaceLogList, & ReplaceLogList_Event ),
      EL_ReplaceListList ( EN_ReplaceListList, & ReplaceListList_Event ),

      EL_ScrollToCountry ( EN_ScrollToCountry, & ScrollToCountry_Event ),
      EL_ScrollToDistrict ( EN_ScrollToDistrict, & ScrollToDistrict_Event ),
      EL_MatchStarting ( EN_MatchStarting, & MatchStarting_Event ),
      EL_ShowOperators ( EN_ShowOperators, & ShowOperators_Event )

{
   Parent = ( TWinControl * ) Owner;               // This makes the JEDI splitter work!

   LogMonitor->initialise( contest );
   GJVQSOLogFrame->initialise( contest, /*this,*/ false );

   WLogAreaSplitter = new TWhisperSplitter(LogAreaSplitter, MatchPanel);
	WLogAreaSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WLogAreaSplitter->HighlightColor = clSkyBlue;

   WMultSplitter = new TWhisperSplitter(MultSplitter, MultPanel);
	WMultSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WMultSplitter->HighlightColor = clSkyBlue;

   WNextContactDetailsSplitter = new TWhisperSplitter(NextContactDetailsSplitter, NextContactDetailsPanel);
	WNextContactDetailsSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WNextContactDetailsSplitter->HighlightColor = clSkyBlue;

   WMatchSplitter = new TWhisperSplitter(MatchSplitter, OtherMatchPanel);
	WMatchSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WMatchSplitter->HighlightColor = clSkyBlue;

   WArchiveMatchSplitter = new TWhisperSplitter(ArchiveMatchSplitter, ArchiveMatchPanel);
	WArchiveMatchSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WArchiveMatchSplitter->HighlightColor = clSkyBlue;

   WMultSplitter->Minimize();  // hide the mults/stats display
   WMatchSplitter->Minimize();     // hide the other list
   WArchiveMatchSplitter->Minimize();     // hide the archive list

   getSplitters();

   WLogAreaSplitter->Restore();

   showMatchHeaders();
   OnShowTimer->Enabled = true;
}
//---------------------------------------------------------------------------
__fastcall TSingleLogFrame::~TSingleLogFrame()
{
   delete WLogAreaSplitter;
   delete WMultSplitter;
   delete WNextContactDetailsSplitter;
   delete WMatchSplitter;
   delete WArchiveMatchSplitter;
}
//---------------------------------------------------------------------------
void TSingleLogFrame::ContestPageChanged_Event ( MinosEventBase & /*Event*/ )
{

   if ( Parent != LogContainer->ContestPageControl->ActivePage )
   {
      GJVQSOLogFrame->savePartial();
      return ;
   }

   BaseContestLog * ct = getContest();
   TContestApp::getContestApp() ->setCurrentContest( ct );

   if ( logColumnsChanged )
   {
      GJVQSOLogFrame->killPartial();
      showQSOs();             // this does a restorePartial
      logColumnsChanged = false;
   }

   if (splittersChanged)
   {
      getSplitters();
   }


   OnShowTimer->Enabled = true;
   GJVQSOLogFrame->CallsignEdit->SetFocus();

}
//---------------------------------------------------------------------------
void TSingleLogFrame::closeContest()
{
   if ( TContestApp::getContestApp() )
   {
      RPCPubSub::publish( "ContestLog", contest->publishedName, makeStr( 0 ), psRevoked );
      GJVQSOLogFrame->closeContest();
      TContestApp::getContestApp() ->closeFile( contest );
      contest = 0;
   }
}
//---------------------------------------------------------------------------
BaseContestLog * TSingleLogFrame::getContest()
{
   return contest;
}
//---------------------------------------------------------------------------
void TSingleLogFrame::ReportOverstrike_Event( MinosEventBase & Event )
{
   ActionEvent2<bool, BaseContestLog *, EN_ReportOverstrike> & S = dynamic_cast<ActionEvent2<bool, BaseContestLog *, EN_ReportOverstrike> &> ( Event );

   if (contest == S.getContext())
   {
      bool overstrike = S.getData();
      LogContainer->StatusBar1->Panels->Items[ 1 ] ->Text = overstrike ? "Overwrite" : "Insert";
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TSingleLogFrame::showQSOs()
{
   TWaitCursor fred;

   LogMonitor->Font->Assign( LogContainer->Font );

   NextContactDetailsTimerTimer( this );


   LogMonitor->showQSOs();
   logColumnsChanged = false;

   GJVQSOLogFrame->clearCurrentField();
   GJVQSOLogFrame->startNextEntry();

}

//---------------------------------------------------------------------------
void TSingleLogFrame::EditContact( BaseContact *lct )
{
   std::auto_ptr <TQSOEditDlg> qdlg( new TQSOEditDlg( this ) );
   ContestContact *ct = dynamic_cast<ContestContact *>( lct );
   qdlg->selectContact( contest, ct );

   qdlg->ShowModal();

   contest->scanContest();

   GJVQSOLogFrame->refreshOps();
   LogMonitor->Invalidate();
   MultDispFrame->refreshMults();
   LogMonitor->Repaint();
   GJVQSOLogFrame->startNextEntry();

}
//---------------------------------------------------------------------------
void TSingleLogFrame::QSOTreeSelectContact( BaseContact * lct )
{
   if (lct)
   {
      EditContact( lct );
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::QSOTreeSelect( PVirtualNode sel )
{
   if ( sel )
   {
      BaseContact * lct = dynamic_cast<BaseContact*>( contest->pcontactAt( sel->Index ) );
      QSOTreeSelectContact(lct);
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::thisMatchTreeSelect( PVirtualNode sel )
{
   if ( !sel )
      return ;
   MatchNodeData *MatchTreeIndex = ( MatchNodeData * ) ( ThisMatchTree->GetNodeData( sel ) );
   BaseContestLog *mclp = MatchTreeIndex->matchedContest;
   if ( MatchTreeIndex && mclp && MatchTreeIndex->matchedContact )
   {
      if ( mclp == contest )
      {
         EditContact( MatchTreeIndex->matchedContact );
      }
      ThisMatchTree->Repaint();
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::otherMatchTreeSelect( PVirtualNode sel )
{
   if ( !sel )
      return ;
   MatchNodeData *MatchTreeIndex = ( MatchNodeData * ) ( OtherMatchTree->GetNodeData( sel ) );
   BaseContestLog *mclp = MatchTreeIndex->matchedContest;
   if ( MatchTreeIndex && mclp && MatchTreeIndex->matchedContact )
   {
      String smessage;

      if ( mclp->isReadOnly() )
         smessage = "READ ONLY contest - Do you wish to switch?";
      else
         smessage = "Do you wish to switch contests?";

      if ( MinosParameters::getMinosParameters() ->yesNoMessage( this, smessage ) )
      {
         LogContainer->selectContest( mclp, MatchTreeIndex->matchedContact );
         return ;
      }
      OtherMatchTree->Repaint();
   }
}
//---------------------------------------------------------------------------
static GridColumn ThisMatchTreeColumns[ THISMATCHTREECOLS ] =
   {
      GridColumn( egTime, "XXXXXXXXXX", "UTC", taLeftJustify ),               // time
      GridColumn( egCall, "MMMMMMMMMMM", "Callsign", taLeftJustify ),         // call
      GridColumn( egRSTTx, "599XXX", "RepTx", taLeftJustify ),                 // RST
      GridColumn( egSNTx, "1234X", "SnTx", taLeftJustify /*taRightJustify*/ ),   // serial
      GridColumn( egRSTRx, "599XXX", "RepRx", taLeftJustify ),                 // RST
      GridColumn( egSNRx, "1234X", "SnRx", taLeftJustify /*taRightJustify*/ ),   // Serial
      GridColumn( egLoc, "MM00MM00X", "Loc", taLeftJustify ),            // LOC
      GridColumn( egBrg, "3601X", "brg", taLeftJustify ),                // bearing
      GridColumn( egScore, "12345XX", "dist", taLeftJustify /*taRightJustify*/ ),  // score
      GridColumn( egExchange, "XXXXXXXXXXXXXXXX", "Exchange", taLeftJustify ),    // QTH
      GridColumn( egComments, "XXXXXXXXXXXXXXXX", "Comments", taLeftJustify )     // comments
   };
//---------------------------------------------------------------------------
static GridColumn OtherMatchTreeColumns[ OTHERMATCHTREECOLS ] =
   {
      GridColumn( egCall, "MMMMMMMMMMM", "Callsign", taLeftJustify ),         // call
      GridColumn( egLoc, "MM00MM00X", "Loc", taLeftJustify ),            // LOC
      GridColumn( egBrg, "3601X", "brg", taLeftJustify ),                // bearing
      GridColumn( egScore, "12345XX", "dist", taLeftJustify /*taRightJustify*/ ),  // score
      GridColumn( egExchange, "XXXXXXXXXXXXXXXX", "Exchange", taLeftJustify ),    // QTH
      GridColumn( egComments, "XXXXXXXXXXXXXXXX", "Comments", taLeftJustify )     // comments
   };
//---------------------------------------------------------------------------
static GridColumn ArchiveMatchTreeColumns[ ARCHIVEMATCHTREECOLS ] =
   {
      GridColumn( egCall, "MMMMMMMMMMM", "Callsign", taLeftJustify ),         // call
      GridColumn( egLoc, "MM00MM00X", "Loc", taLeftJustify ),            // LOC
      GridColumn( egBrg, "3601X", "brg", taLeftJustify ),                // bearing
      GridColumn( egScore, "12345XX", "dist", taLeftJustify /*taRightJustify*/ ),  // score
      GridColumn( egExchange, "XXXXXX", "Exchange", taLeftJustify ),     // exchange
      GridColumn( egComments, "XXXX", "Comments", taLeftJustify )     // comments
   };
//---------------------------------------------------------------------------
void TSingleLogFrame::showMatchHeaders( )
{
   ThisMatchTree->Canvas->Font->Assign( LogContainer->Font );
   OtherMatchTree->Canvas->Font->Assign( LogContainer->Font );
   ArchiveMatchTree->Canvas->Font->Assign( LogContainer->Font );

   ThisMatchTree->Header->Options = ( ThisMatchTree->Header->Options >> hoVisible );
   ThisMatchTree->Header->Columns->Clear();
   // Setting header visibility BEFORE setting columns seems to work...
   for ( int i = 0; i < THISMATCHTREECOLS; i++ )
   {
      TVirtualTreeColumn *NewColumn = ThisMatchTree->Header->Columns->Add();
      NewColumn->Alignment = ThisMatchTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      NewColumn->Width = ThisMatchTree->Canvas->TextWidth( ThisMatchTreeColumns[ i ].width );

      NewColumn->Text = ThisMatchTreeColumns[ i ].title;
   }
   ThisMatchTree->Margin = 0;
   ThisMatchTree->TextMargin = 4;
   ThisMatchTree->Header->Options = ( ThisMatchTree->Header->Options << hoVisible );

   OtherMatchTree->Header->Options = ( OtherMatchTree->Header->Options >> hoVisible );
   OtherMatchTree->Header->Columns->Clear();
   for ( int i = 0; i < OTHERMATCHTREECOLS; i++ )
   {
      TVirtualTreeColumn *NewColumn = OtherMatchTree->Header->Columns->Add();
      NewColumn->Alignment = OtherMatchTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      NewColumn->Width = OtherMatchTree->Canvas->TextWidth( OtherMatchTreeColumns[ i ].width );

      NewColumn->Text = OtherMatchTreeColumns[ i ].title;
   }
   OtherMatchTree->Margin = 0;
   OtherMatchTree->TextMargin = 4;
   OtherMatchTree->Header->Options = ( OtherMatchTree->Header->Options << hoVisible );

   ArchiveMatchTree->Header->Options = ( ArchiveMatchTree->Header->Options >> hoVisible );
   ArchiveMatchTree->Header->Columns->Clear();
   for ( int i = 0; i < ARCHIVEMATCHTREECOLS; i++ )
   {
      TVirtualTreeColumn *NewColumn = ArchiveMatchTree->Header->Columns->Add();
      NewColumn->Alignment = ArchiveMatchTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      NewColumn->Width = ArchiveMatchTree->Canvas->TextWidth( ArchiveMatchTreeColumns[ i ].width );

      NewColumn->Text = ArchiveMatchTreeColumns[ i ].title;
   }
   ArchiveMatchTree->Margin = 0;
   ArchiveMatchTree->TextMargin = 4;
   ArchiveMatchTree->Header->Options = ( ArchiveMatchTree->Header->Options << hoVisible );
}
//---------------------------------------------------------------------------
void TSingleLogFrame::showMatchQSOs( TMatchCollection *matchCollection )
{
   // Display the current contest QSO matches - both "this" and "other"

   // We want a node structure where the LoggerContestLog includes the number of matches
   // and we can tag "DUPLICATE" or whatever on the end of the LoggerContestLog name

   TWaitCursor fred;

   GJVQSOLogFrame->MatchXferButton->Enabled = false;

   ThisMatchTree->Clear();
   ThisMatchTree->RootNodeCount = 0;

   OtherMatchTree->Clear();
   OtherMatchTree->RootNodeCount = 0;

   if ( matchCollection->getContactCount() == 0 )
   {
      PVirtualNode LastNode = ThisMatchTree->AddChild( NULL );

      MatchNodeData * pc = ( MatchNodeData * ) ThisMatchTree->GetNodeData( LastNode );
      pc->top = false;
      pc->matchedContest = 0;
      pc->matchedContact = 0;

      LastNode = OtherMatchTree->AddChild( NULL );
      pc = ( MatchNodeData * ) OtherMatchTree->GetNodeData( LastNode );
      pc->top = false;
      pc->matchedContest = 0;
      pc->matchedContact = 0;

      WMatchSplitter->Minimize();     // hide the archive list
      return ;
   }

   ThisMatchTree->BeginUpdate();
   OtherMatchTree->BeginUpdate();

   BaseContestLog * last_pc;
   PVirtualNode lastTopNode = 0;

   {  // scoping only
      MatchContact *pc0 = matchCollection->pcontactAt( 0 );
      lastTopNode = ThisMatchTree->AddChild( NULL );
      MatchNodeData * pc = ( MatchNodeData * ) ThisMatchTree->GetNodeData( lastTopNode );
      BaseContestLog * mclp = pc0->getContactLog();
      pc->matchedContest = ( ( mclp == contest ) ? contest : 0 );
      pc->matchedContact = 0;
      pc->top = true;         // say no matches
      last_pc = contest;
   }

   PVirtualNode thisnode = 0;             // not currently used
   PVirtualNode othernode = 0;

   for ( int i = 0; i < matchCollection->getContactCount(); i++ )
   {
      MatchContact *pc = matchCollection->pcontactAt( i );
      BaseContestLog * clp = pc->getContactLog();
      if ( clp == contest )
      {
         PVirtualNode LastNode = ThisMatchTree->AddChild( lastTopNode );
         MatchNodeData * thispc = ( MatchNodeData * ) ThisMatchTree->GetNodeData( LastNode );
         thispc->matchedContest = pc->getContactLog();
         thispc->matchedContact = pc->getBaseContact();
         thispc->top = false;
         pc->treePointer = thispc;
         if ( !thisnode )
         {
            thisnode = LastNode;         // not curently used
         }
      }
      else
      {
         if ( !last_pc || last_pc != clp )
         {
            // set up a contest node
            lastTopNode = OtherMatchTree->AddChild( NULL );
            MatchNodeData * thispc = ( MatchNodeData * ) OtherMatchTree->GetNodeData( lastTopNode );
            thispc->matchedContest = pc->getContactLog();
            thispc->matchedContact = 0;
            thispc->top = true;
            WMatchSplitter->Restore();     // hide the other list
         }
         last_pc = clp;
         if ( pc )
         {
            // and the contact node
            PVirtualNode LastNode = OtherMatchTree->AddChild( lastTopNode );
            MatchNodeData * thispc = ( MatchNodeData * ) OtherMatchTree->GetNodeData( LastNode );
            thispc->matchedContest = pc->getContactLog();
            thispc->matchedContact = pc->getBaseContact();
            thispc->top = false;
            pc->treePointer = thispc;
            if ( !othernode )
            {
               othernode = LastNode;
            }
         }
      }
   }

   ThisMatchTree->FullExpand();

   // for now give archive a dummy node - we will use it for lists/DB/etc
   if ( OtherMatchTree->RootNodeCount == 0 )
   {
      PVirtualNode LastNode = OtherMatchTree->AddChild( NULL );
      MatchNodeData * thispc = ( MatchNodeData * ) OtherMatchTree->GetNodeData( LastNode );
      thispc->matchedContest = 0;
      thispc->matchedContact = 0;
      thispc->top = true;
   }
   OtherMatchTree->FullExpand();

   ThisMatchTree->EndUpdate();
   OtherMatchTree->EndUpdate();

   if ( othernode )
   {
      otherTreeClickNode = othernode;

      OtherMatchTree->FocusedNode = otherTreeClickNode;
      OtherMatchTree->Selected[ otherTreeClickNode ] = true;
      // we only allow xfer from other or archive, not from this
      OtherMatchTree->Colors->UnfocusedSelectionColor = ( TColor ) ( 0x00C080FF );
      ArchiveMatchTree->Colors->UnfocusedSelectionColor = clBtnFace;
      GJVQSOLogFrame->MatchXferButton->Font->Color = ( TColor ) ( 0x00C080FF );
      GJVQSOLogFrame->MatchXferButton->Enabled = true;
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::showMatchList( TMatchCollection *matchCollection )
{
   // display the current archive list

   // We want a node structure where the LoggerContestLog includes the number of matches
   // and we can tag "DUPLICATE" or whatever on the end of the LoggerContestLog name

   TWaitCursor fred;

   ArchiveMatchTree->Clear();
   ArchiveMatchTree->RootNodeCount = 0;

   if ( matchCollection->getContactCount() == 0 )
   {
      PVirtualNode LastNode = ArchiveMatchTree->AddChild( NULL );
      MatchNodeListData * pc = ( MatchNodeListData * ) ArchiveMatchTree->GetNodeData( LastNode );
      pc->top = false;
      pc->matchedList = 0;
      pc->matchedContact = 0;

      WArchiveMatchSplitter->Minimize();          /// hide the archive list
      return ;
   }
   WArchiveMatchSplitter->Restore();

   ArchiveMatchTree->BeginUpdate();

   PVirtualNode othernode = 0;
   PVirtualNode lastTopNode = 0;

   {  // scoping only
      MatchContact *pc0 = matchCollection->pcontactAt( 0 );
      lastTopNode = ArchiveMatchTree->AddChild( NULL );
      MatchNodeListData * pc = ( MatchNodeListData * ) ArchiveMatchTree->GetNodeData( lastTopNode );
      ContactList * mclp = pc0->getContactList();
      pc->matchedList = mclp;
      pc->matchedContact = 0;
      pc->top = true;         // say no matches
   }

   for ( int i = 0; i < matchCollection->getContactCount(); i++ )
   {
      MatchContact *pc = matchCollection->pcontactAt( i );
      if ( pc )
      {
         // and the contact node
         PVirtualNode LastNode = ArchiveMatchTree->AddChild( lastTopNode );
         MatchNodeListData * thispc = ( MatchNodeListData * ) ArchiveMatchTree->GetNodeData( LastNode );
         thispc->matchedList = pc->getContactList();
         thispc->matchedContact = pc->getListContact();
         thispc->top = false;
         pc->treePointer = thispc;
         if ( !othernode )
         {
            othernode = LastNode;
         }
      }
   }
   if ( othernode )
   {
      archiveTreeClickNode = othernode;

      ArchiveMatchTree->FocusedNode = archiveTreeClickNode;
      ArchiveMatchTree->Selected[ archiveTreeClickNode ] = true;
      // we only allow xfer from other or archive, not from "this"
      if ( !matchTreeClickNode )
      {
         OtherMatchTree->Colors->UnfocusedSelectionColor = clBtnFace;
         ArchiveMatchTree->Colors->UnfocusedSelectionColor = ( TColor ) ( 0x00C080FF );
         GJVQSOLogFrame->MatchXferButton->Font->Color = ( TColor ) ( 0x00C080FF );
         GJVQSOLogFrame->MatchXferButton->Enabled = true;
      }
   }

   ArchiveMatchTree->FullExpand();

   ArchiveMatchTree->EndUpdate();

}
//---------------------------------------------------------------------------
void TSingleLogFrame::ReplaceLogList_Event ( MinosEventBase & Event )
{
   ActionEvent2<TMatchCollection *, BaseContestLog *, EN_ReplaceLogList> & S = dynamic_cast<ActionEvent2<TMatchCollection *, BaseContestLog *, EN_ReplaceLogList> &> ( Event );
   if (contest == S.getContext())
   {
      TMatchCollection *matchCollection = S.getData();
      replaceContestList(matchCollection);
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::ReplaceListList_Event ( MinosEventBase & Event )
{
   ActionEvent2<TMatchCollection *, BaseContestLog *, EN_ReplaceListList> & S = dynamic_cast<ActionEvent2<TMatchCollection *, BaseContestLog *, EN_ReplaceListList> &> ( Event );
   if (contest == S.getContext())
   {
      TMatchCollection *matchCollection = S.getData();
      replaceListList(matchCollection);
   }
}
//==============================================================================
void TSingleLogFrame::replaceContestList( TMatchCollection *matchCollection )
{
   showMatchQSOs( matchCollection );
}
//==============================================================================
void TSingleLogFrame::replaceListList( TMatchCollection *matchCollection )
{
   showMatchList( matchCollection );
}
//==============================================================================
void TSingleLogFrame::MatchStarting_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_MatchStarting> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_MatchStarting> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      // clear down match trees
      xferTree = 0;
      matchTreeClickNode = 0;
      otherTreeClickNode = 0;
      archiveTreeClickNode = 0;
      OtherMatchTree->Colors->UnfocusedSelectionColor = clBtnFace;
      ArchiveMatchTree->Colors->UnfocusedSelectionColor = clBtnFace;
      GJVQSOLogFrame->MatchXferButton->Font->Color = clBtnText;
      GJVQSOLogFrame->MatchXferButton->Enabled = false;
   }
}

//==============================================================================
void TSingleLogFrame::transferDetails( MatchNodeData *MatchTreeIndex )
{
   // needs to be transferred into QSOLogFrame.cpp
   const BaseContact * lct = MatchTreeIndex->matchedContact;
   if ( !contest )
   {
      return ;
   }

   if ( lct )
   {
      BaseContestLog * matct = MatchTreeIndex->matchedContest;
      GJVQSOLogFrame->transferDetails( lct, matct );
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::transferDetails( MatchNodeListData *MatchTreeIndex )
{
   // needs to be transferred into QSOLogFrame.cpp
   const ListContact * lct = MatchTreeIndex->matchedContact;
   if ( !contest )
   {
      return ;
   }

   if ( lct )
   {
      ContactList * matct = MatchTreeIndex->matchedList;
      GJVQSOLogFrame->transferDetails( lct, matct );
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::LogColumnsChanged_Event ( MinosEventBase & /*Event*/ )
{
// no need to check for current log
   logColumnsChanged = true;
}
//---------------------------------------------------------------------------
void TSingleLogFrame::GoToSerial_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_GoToSerial> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_GoToSerial> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      goSerial();
   }
}
//---------------------------------------------------------------------------

void TSingleLogFrame::goSerial( )
{
   static int serial = 0;
   do
   {
      if ( serial == -1 )
         serial = 0;
      if ( !enquireDialog( this, "Please give serial wanted", serial ) )
         return ;
   }
   while ( serial == -1 );

   DisplayContestContact *cfu = 0;
   for ( LogIterator i = contest->ctList.begin(); i != contest->ctList.end(); i++ )
   {
      if ( serial == String( ( *i ) ->serials.getValue().c_str() ).ToIntDef( -2 ) )
      {
         cfu = dynamic_cast<DisplayContestContact *>( *i );
         break;
      }
   }

   if ( cfu )
   {
      EditContact( cfu );
   }
   else
      MinosParameters::getMinosParameters() ->mshowMessage( "Serial number " + String( serial ) + " not found" );
}




void __fastcall TSingleLogFrame::NextContactDetailsTimerTimer( TObject */*Sender*/ )
{
   /*
      TWinControl *cur =  LogContainer->ActiveControl;
      if (cur)
      {
   	  LogContainer->StatusBar1->Panels->Items[ 1 ] ->Text = cur->Name;
      }
      else
      {
   	  LogContainer->StatusBar1->Panels->Items[ 1 ] ->Text = "unknown";
      }
   */
   if ( contest )
   {
      if ( contest->isReadOnly() )
      {
         GJVQSOLogFrame->BandMapPanel->Visible = false;
         NextContactDetailsLabel->Font->Color = clGray;
         NextContactDetailsLabel->Caption = String( contest->mycall.fullCall.getValue().c_str() ) + "\r\n"
                                            + contest->myloc.loc.getValue().c_str() + "\r\n"
                                            + contest->location.getValue().c_str();
      }
      else
      {
         GJVQSOLogFrame->BandMapPanel->Visible = LogContainer->isBandMapLoaded();
         std::string buff = ( boost::format( "%03.3d" ) % ( contest->maxSerial + 1 ) ).str();
         NextContactDetailsLabel->Caption = String( contest->mycall.fullCall.getValue().c_str() ) + "\r\n"
                                            + buff.c_str() + "\r\n"
                                            + contest->myloc.loc.getValue().c_str() + "\r\n"
                                            + contest->location.getValue().c_str();
      }
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::ScrollToDistrict_Event ( MinosEventBase & Event )
{
   ActionEvent2<std::string, BaseContestLog *, EN_ScrollToDistrict> & S = dynamic_cast<ActionEvent2<std::string, BaseContestLog *, EN_ScrollToDistrict> &> ( Event );
   if (contest == S.getContext( ))
   {
      std::string qth = S.getData();
      matchDistrict(qth);
   }
}
//---------------------------------------------------------------------------

void TSingleLogFrame::matchDistrict( const std::string &qth )
{
   DistrictEntry * dist = MultLists::getMultLists() ->searchDistrict( qth );
   if ( dist )
   {
      unsigned int district_ind = MultLists::getMultLists() ->getDistListIndexOf( dist );
      MultDispFrame->scrollToDistrict( district_ind, true );
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::ScrollToCountry_Event ( MinosEventBase & Event )
{
   ActionEvent2<std::string, BaseContestLog *, EN_ScrollToCountry> & S = dynamic_cast<ActionEvent2<std::string, BaseContestLog *, EN_ScrollToCountry> &> ( Event );
   if (contest == S.getContext())
   {
      std::string prefix = S.getData();
      matchCountry(prefix);
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::matchCountry( const std::string &csCs )
{
   callsign cs( csCs );
   cs.validate( );	// we don't use the result

   CountryEntry *ctryMult = findCtryPrefix( cs );
   if ( ctryMult )
   {
      unsigned int ctry_ind = MultLists::getMultLists() ->getCtryListIndexOf( ctryMult );
      MultDispFrame->scrollToCountry( ctry_ind, true );
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::ThisMatchTreeEnter( TObject */*Sender*/ )
{
   GJVQSOLogFrame->clearCurrentField();
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreeKeyDown( TObject */*Sender*/,
      WORD &Key, TShiftState /*Shift*/ )
{
   if ( Key == VK_SPACE )
   {
      Key = 0;
      thisMatchTreeSelect( ThisMatchTree->FocusedNode );
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreeGetText( TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType /*TextType*/,
      WideString &CellText )
{
   CellText = "";
   MatchNodeData * pc = ( MatchNodeData * ) Sender->GetNodeData( Node );
   BaseContestLog *clp = pc->matchedContest;
   if ( pc && ( clp || pc->matchedContact ) && Column >= 0 )
   {
      if ( pc->top )
      {
         if ( Column == 0 )
            CellText = ( "Current contest" + TMatchThread::getMatchStatus() ).c_str();
      }
      else
      {
         std::string cell;
         BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
         clp->getMatchField( pc->matchedContact, ThisMatchTreeColumns[ Column ].fieldId, cell, ct );     // col 0 is the tree lines
         CellText = cell.c_str();
      }
   }
   else
      if ( Column == 0 )
      {
         // No columns defined if no contacts
         CellText = "Current contest - NO MATCHES";
      }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreePaintText(
   TBaseVirtualTree *Sender, const TCanvas *TargetCanvas, PVirtualNode Node,
   TColumnIndex Column, TVSTTextType /*TextType*/ )
{
   MatchNodeData * pc = ( MatchNodeData * ) Sender->GetNodeData( Node );
   BaseContestLog *clp = pc->matchedContest;
   if ( pc && ( clp || pc->matchedContact ) )
   {
      TColor multhighlight = clRed;
      BaseContact *ct = pc->matchedContact;
      if ( ct )
      {
         switch ( ThisMatchTreeColumns[ Column ].fieldId )
         {
            case egCall:
               if ( ct->newCtry )
                  TargetCanvas->Font->Color = multhighlight;
               break;
            case egExchange:
               if ( ct->newDistrict )
                  TargetCanvas->Font->Color = multhighlight;
               break;
            case egLoc:
               if ( ct->newLoc )
                  TargetCanvas->Font->Color = multhighlight;
               break;
         }
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = sizeof( MatchNodeData );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreeDblClick( TObject */*Sender*/ )
{
   thisMatchTreeSelect( matchTreeClickNode );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreeMouseDown( TObject */*Sender*/,
      TMouseButton /*Button*/, TShiftState /*Shift*/, int X, int Y )
{
   //  xferTree = ThisMatchTree;
   matchTreeClickNode = ThisMatchTree->GetNodeAt( X, Y );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::OtherMatchTreeDblClick( TObject *Sender )
{
   GJVQSOLogFrame1MatchXferButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::OtherMatchTreeEnter( TObject */*Sender*/ )
{
   GJVQSOLogFrame->clearCurrentField();
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::OtherMatchTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = sizeof( MatchNodeData );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::OtherMatchTreeGetText(
   TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
   TVSTTextType /*TextType*/, WideString &CellText )
{
   CellText = "";
   MatchNodeData * pc = ( MatchNodeData * ) Sender->GetNodeData( Node );
   BaseContestLog *clp = pc->matchedContest;
   if ( pc && ( clp || pc->matchedContact ) && Column >= 0 )
   {
      if ( pc->top )
      {
         if ( Column == 0 )
            CellText = ( clp->name.getValue() ).c_str();
      }
      else
         if ( !pc->top )
         {
            std::string cell;
            BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
            clp->getMatchField( pc->matchedContact, OtherMatchTreeColumns[ Column ].fieldId, cell, ct );     // col 0 is the tree lines
            CellText = cell.c_str();
         }
   }
   else
      if ( Column == 0 )
      {
         CellText = "No current (other Contest Log) matches";
      }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::OtherMatchTreeKeyDown( TObject */*Sender*/,
      WORD &Key, TShiftState /*Shift*/ )
{
   if ( Key == VK_SPACE )
   {
      Key = 0;
      otherMatchTreeSelect( OtherMatchTree->FocusedNode );
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::OtherMatchTreeMouseDown( TObject */*Sender*/,
      TMouseButton /*Button*/, TShiftState /*Shift*/, int X, int Y )
{
   xferTree = OtherMatchTree;
   otherTreeClickNode = OtherMatchTree->GetNodeAt( X, Y );
   OtherMatchTree->Colors->UnfocusedSelectionColor = ( TColor ) ( 0x00C080FF );
   ArchiveMatchTree->Colors->UnfocusedSelectionColor = clBtnFace;
   GJVQSOLogFrame->MatchXferButton->Font->Color = ( TColor ) ( 0x00C080FF );
   GJVQSOLogFrame->MatchXferButton->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ArchiveMatchTreeDblClick( TObject *Sender )
{
   GJVQSOLogFrame1MatchXferButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ArchiveMatchTreeEnter( TObject */*Sender*/ )
{
   GJVQSOLogFrame->clearCurrentField();
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ArchiveMatchTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = sizeof( MatchNodeListData );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ArchiveMatchTreeGetText(
   TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
   TVSTTextType /*TextType*/, WideString &CellText )
{
   CellText = "";
   MatchNodeListData * pc = ( MatchNodeListData * ) Sender->GetNodeData( Node );
   ContactList *clp = pc->matchedList;
   if ( pc && ( clp || pc->matchedContact ) && Column >= 0 )
   {
      if ( pc->top )
      {
         if ( Column == 0 )
            CellText = ( clp->name ).c_str();
      }
      else
      {
         std::string cell;
         BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
         if ( ArchiveMatchTreeColumns[ Column ].fieldId == egComments )
         {
            std::string exch;
            clp->getMatchField( pc->matchedContact, egExchange, exch, ct );
            std::string cmnt;
            clp->getMatchField( pc->matchedContact, egComments, cmnt, ct );
            if ( exch.size() )
            {
               cell = exch + "(" + cmnt + ")";
            }
            else
            {
               cell = cmnt;
            }
         }
         else
         {
            clp->getMatchField( pc->matchedContact, ArchiveMatchTreeColumns[ Column ].fieldId, cell, ct );     // col 0 is the tree lines
         }
         CellText = cell.c_str();
      }
   }
   else
   {
      if ( Column == 0 )
         CellText = "No archive matches";
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ArchiveMatchTreeMouseDown(
   TObject */*Sender*/, TMouseButton /*Button*/, TShiftState /*Shift*/, int X,
   int Y )
{
   xferTree = ArchiveMatchTree;
   archiveTreeClickNode = ArchiveMatchTree->GetNodeAt( X, Y );
   OtherMatchTree->Colors->UnfocusedSelectionColor = clBtnFace;
   ArchiveMatchTree->Colors->UnfocusedSelectionColor = ( TColor ) ( 0x00C080FF );
   GJVQSOLogFrame->MatchXferButton->Font->Color = ( TColor ) ( 0x00C080FF );
   GJVQSOLogFrame->MatchXferButton->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::NextContactDetailsOnLeftClick( TObject *Sender )
{
   LogContainer->NextContactDetailsOnLeftActionExecute( Sender );
}
//---------------------------------------------------------------------------
void TSingleLogFrame::doNextContactDetailsOnLeftClick( TObject */*Sender*/ )
{
   NextContactDetailsOnLeft->Checked = LogContainer->isNextContactDetailsOnLeft();
   if ( LogContainer->isNextContactDetailsOnLeft() && NextContactDetailsPanel->Left != 0 )
   {
      NextContactDetailsPanel->Align = alNone;
      GJVQSOLogFrame->Align = alNone;
      NextContactDetailsPanel->Left = 0;
      GJVQSOLogFrame->Left = NextContactDetailsPanel->Width + NextContactDetailsSplitter->Width;
      NextContactDetailsSplitter->Align = alLeft;
      NextContactDetailsPanel->Align = alLeft;
      GJVQSOLogFrame->Align = alClient;
   }
   else
      if ( !LogContainer->isNextContactDetailsOnLeft() && GJVQSOLogFrame->Left != 0 )
      {
         NextContactDetailsPanel->Align = alNone;
         GJVQSOLogFrame->Align = alNone;
         GJVQSOLogFrame->Left = 0;
         NextContactDetailsPanel->Left = GJVQSOLogFrame->Width + NextContactDetailsSplitter->Width;
         NextContactDetailsPanel->Align = alRight;
         NextContactDetailsSplitter->Align = alRight;
         GJVQSOLogFrame->Align = alClient;
      }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::MakeEntry_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_MakeEntry> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_MakeEntry> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      makeEntry( false );
   }
}
//---------------------------------------------------------------------------
String TSingleLogFrame::makeEntry( bool saveMinos )
{
   LoggerContestLog * ct = dynamic_cast<LoggerContestLog *>( contest );
   if ( !ct )
   {
      return "";
   }
   std::auto_ptr <TEntryOptionsForm> EntryDlg( new TEntryOptionsForm( this, ct, saveMinos ) );
   if ( saveMinos )
   {
      EntryDlg->Caption = "Save imported log as a .Minos file";
   }
   if ( EntryDlg->ShowModal() == mrOk )
   {
      ct->commonSave( false );
      String expName = EntryDlg->doFileSave( );
      return expName;
   }
   return "";
}
//---------------------------------------------------------------------------
void TSingleLogFrame::SetMode_Event( MinosEventBase & Event)
{
   ActionEvent2<String, BaseContestLog *, EN_SetMode> & S = dynamic_cast<ActionEvent2<String, BaseContestLog *, EN_SetMode> &> ( Event );
   if (contest == S.getContext())
   {
      String mode = S.getData();
      setMode(mode);
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::setMode( String m )
{
   GJVQSOLogFrame->setMode( m );
}
//---------------------------------------------------------------------------
void TSingleLogFrame::SetFreq_Event( MinosEventBase & Event)
{
   ActionEvent2<String, BaseContestLog *, EN_SetFreq> & S = dynamic_cast<ActionEvent2<String, BaseContestLog *, EN_SetFreq> &> ( Event );
   if (contest == S.getContext())
   {
      String freq = S.getData();
      setFreq(freq);
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::setFreq( String f )
{
   // If freq has moved "enough" then send current QSO details to bandmap
   long long freq = 0;

   int nscan = sscanf( f.c_str(), "%lld", &freq );
   if ( nscan == 1 && ( abs( ( long double ) ( freq - oldFreq ) ) > freqdelta ) )
   {
      oldFreq = currFreq;
      sOldFreq = sCurrFreq;
      currFreq = freq;
      sCurrFreq = f;
      GJVQSOLogFrame->setFreq( f );
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::AutoBandmapTuneClick( TObject */*Sender*/ )
{
   TContestApp::getContestApp() ->displayBundle.setBoolProfile( edpAutoBandMapTune, GJVQSOLogFrame->AutoBandmapTune->Checked );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::AutoBandmapTimeClick( TObject */*Sender*/ )
{
   TContestApp::getContestApp() ->displayBundle.setBoolProfile( edpAutoBandMapTime, GJVQSOLogFrame->AutoBandmapTime->Checked );
}
//---------------------------------------------------------------------------
void TSingleLogFrame::SetTimeNow_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_SetTimeNow> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_SetTimeNow> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      SetTimeNowClick(0);

   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::SetTimeNowClick( TObject */*Sender*/ )
{
   GJVQSOLogFrame->setTimeNow();
}
//---------------------------------------------------------------------------
void TSingleLogFrame::FormKey_Event( MinosEventBase & Event)
{
   ActionEvent2<WORD *, BaseContestLog *, EN_FormKey> & S = dynamic_cast<ActionEvent2<WORD *, BaseContestLog *, EN_FormKey> &> ( Event );
   if (contest == S.getContext())
   {
      WORD *key = S.getData();
      setActiveControl(key);
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::setActiveControl( WORD *Key )
{
   GJVQSOLogFrame->setActiveControl( Key );
   if (
      LogContainer->ActiveControl != LogMonitor->QSOTree
      && LogContainer->ActiveControl != ThisMatchTree
      && LogContainer->ActiveControl != OtherMatchTree
      && *Key == VK_PRIOR )
   {
      LogContainer->ActiveControl = LogMonitor->QSOTree;
      *Key = 0;
   }
   else
      if (
         LogContainer->ActiveControl != LogMonitor->QSOTree
         && LogContainer->ActiveControl != ThisMatchTree
         && LogContainer->ActiveControl != OtherMatchTree
         && *Key == VK_NEXT )
      {
         LogContainer->ActiveControl = ThisMatchTree;
         *Key = 0;
      }
      else
         if (*Key == VK_F12 && GJVQSOLogFrame->MatchXferButton->Enabled)
         {
            GJVQSOLogFrame1MatchXferButtonClick(this);
            *Key = 0;
         }
}
//-------------------------------------------------------------------------
void __fastcall TSingleLogFrame::GJVQSOLogFrame1GJVOKButtonClick(
   TObject *Sender )
{
   if ( LogContainer->ActiveControl == LogMonitor->QSOTree )
   {
      QSOTreeSelect( LogMonitor->QSOTree->FocusedNode );
      return ;
   }
   else
      if ( LogContainer->ActiveControl == ThisMatchTree )
      {
         thisMatchTreeSelect( ThisMatchTree->FocusedNode );
         return ;
      }
      else
         if ( LogContainer->ActiveControl == OtherMatchTree )
         {
            otherMatchTreeSelect( OtherMatchTree->FocusedNode );
            return ;
         }


   GJVQSOLogFrame->doGJVOKButtonClick( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::GJVQSOLogFrame1GJVForceButtonClick(
   TObject *Sender )
{
   GJVQSOLogFrame->doGJVForceButtonClick( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::GJVQSOLogFrame1GJVCancelButtonClick(
   TObject *Sender )
{
   GJVQSOLogFrame->doGJVCancelButtonClick( Sender );
   MultDispFrame->refreshMults();
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::GJVQSOLogFrame1MatchXferButtonClick(
   TObject */*Sender*/ )
{
   // transfer from current match
   if ( contest->isReadOnly() )
      return ;

   // copy relevant parts of match contact to screen contact
   if ( archiveTreeClickNode && ( xferTree == 0 || xferTree == ArchiveMatchTree ) )
   {
      MatchNodeListData * MatchTreeIndex = ( MatchNodeListData * ) ( ArchiveMatchTree->GetNodeData( archiveTreeClickNode ) );
      ContactList *clp = MatchTreeIndex->matchedList;
      if ( MatchTreeIndex && clp && MatchTreeIndex->matchedContact )
      {
         transferDetails( MatchTreeIndex );
      }
   }
   else
      if ( otherTreeClickNode && ( xferTree == 0 || xferTree == OtherMatchTree ) )
      {
         MatchNodeData * MatchTreeIndex = ( MatchNodeData * ) ( OtherMatchTree->GetNodeData( otherTreeClickNode ) );
         BaseContestLog *clp = MatchTreeIndex->matchedContest;
         if ( MatchTreeIndex && clp && MatchTreeIndex->matchedContact )
         {
            transferDetails( MatchTreeIndex );
         }
      }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::AfterSelectContact_Event( MinosEventBase & Event)
{
   ActionEvent2<BaseContact *, BaseContestLog *, EN_AfterSelectContact> & S = dynamic_cast<ActionEvent2<BaseContact *, BaseContestLog *, EN_AfterSelectContact> &> ( Event );
   if (contest == S.getContext())
   {
      BaseContact *lct = S.getData();
      if (!lct)
      {
         PVirtualNode l = LogMonitor->QSOTree->GetLastChild( LogMonitor->QSOTree->RootNode );
         LogMonitor->QSOTree->FocusedNode = l;
         LogMonitor->QSOTree->Selected[ l ] = true;
      }
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::AfterLogContact_Event( MinosEventBase & Event)
{
      ActionEvent1<BaseContestLog *, EN_AfterLogContact> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_AfterLogContact> &> ( Event );
      BaseContestLog *ct = S.getData();

      if (ct == contest)
      {
         contest->scanContest();
         updateTrees();
         NextContactDetailsTimerTimer( this );
      }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::updateTrees()
{
   LogMonitor->QSOTree->RootNodeCount = contest->getContactCount() + 1;
   MultDispFrame->refreshMults();
   LogMonitor->QSOTree->Invalidate();
}
//---------------------------------------------------------------------------
void TSingleLogFrame::NextUnfilled_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_NextUnfilled> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_NextUnfilled> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      GoNextUnfilled();
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::GoNextUnfilled()
{
   BaseContact * nuc = contest->findNextUnfilledContact( );
   if ( nuc )
   {
      std::auto_ptr <TQSOEditDlg> qdlg( new TQSOEditDlg( this ) );
      qdlg->setContest( contest );
      qdlg->setFirstContact( nuc );
      qdlg->ShowModal();
      contest->scanContest();
      LogMonitor->QSOTree->Invalidate();
      MultDispFrame->refreshMults();
      LogMonitor->QSOTree->Repaint();
      GJVQSOLogFrame->startNextEntry();
   }
   else
   {
      ShowMessage( "No unfilled contacts" );
   }

}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::TimerUpdateQSOTimerTimer(TObject */*Sender*/)
{
   updateQSOTime();
}
//---------------------------------------------------------------------------
void TSingleLogFrame::updateQSOTime()
{
   GJVQSOLogFrame->updateQSOTime();
}
//---------------------------------------------------------------------------
void TSingleLogFrame::updateQSODisplay()
{
   BandCombo->Clear();
   std::string cb = trim(contest->band.getValue());
   BandList &blist = BandList::getBandList();
   BandInfo bi;
   bool bandOK = blist.findBand(cb, bi);
   if (bandOK)
   {
      BandCombo->Items->Add(bi.uk.c_str());
   }
   else
   {
      BandCombo->Items->Add(contest->band.getValue().c_str());
   }
   BandCombo->ItemIndex = 0;

   GJVQSOLogFrame->updateQSODisplay();
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::LogMonitorQSOTreeDblClick( TObject */*Sender*/ )
{
   QSOTreeSelect( QSOTreeClickNode );
   LogMonitor->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::LogMonitorQSOTreeEnter( TObject */*Sender*/ )
{
   GJVQSOLogFrame->clearCurrentField();
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::LogMonitorQSOTreeKeyDown( TObject */*Sender*/,
      WORD &Key, TShiftState /*Shift*/ )
{
   if ( Key == VK_SPACE )
   {
      Key = 0;
      QSOTreeSelect( LogMonitor->QSOTree->FocusedNode );
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::LogMonitorQSOTreeMouseDown( TObject */*Sender*/,
      TMouseButton /*Button*/, TShiftState /*Shift*/, int X, int Y )
{
   PVirtualNode Node = LogMonitor->QSOTree->GetNodeAt( X, Y );
   QSOTreeClickNode = Node;
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::PublishTimerTimer( TObject */*Sender*/ )
{
   LoggerContestLog * ct = dynamic_cast<LoggerContestLog *>( contest );
   if ( ct && ct->isMinosFile() && !ct->isUnwriteable() && !ct->isProtected())
   {
      int stanzaCount = contest->getStanzaCount();
      if ( lastStanzaCount != stanzaCount )
      {
         // publish this contest details - what to use?
         // category LoggerContestLog
         // name filename(?)
         // value stanzaCount
         RPCPubSub::publish( "ContestLog", contest->publishedName, makeStr( stanzaCount ), psPublished );
         lastStanzaCount = stanzaCount;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::OnShowTimerTimer( TObject */*Sender*/ )
{
   OnShowTimer->Enabled = false;
   MultDispFrame->setContest( contest );
   doNextContactDetailsOnLeftClick( this );
   MinosLoggerEvents::SendShowOperators();

   // Supress the tabstops we weren't able to manage in the form designed
   // to discover where they went, uncomment the top block in
   // TSingleLogFrame::NextContactDetailsTimerTimer

   MultDispFrame->TabStop = false;
   LogMonitor->TabStop = false;
   GJVQSOLogFrame->TabStop = false;

   updateQSODisplay();
}
//---------------------------------------------------------------------------
bool TSingleLogFrame::getStanza( unsigned int stanza, std::string &stanzaData )
{
   return contest->getStanza( stanza, stanzaData );
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::LogAreaSplitterMoved( TObject */*Sender*/ )
{
   if (!WLogAreaSplitter->Minimized)
   {
      TContestApp::getContestApp() ->displayBundle.setIntProfile( "LogSplitterPosition", MatchPanel->Height );
      SplittersChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::MatchSplitterMoved(TObject */*Sender*/)
{
   if (!WMatchSplitter->Minimized)
   {
      TContestApp::getContestApp() ->displayBundle.setIntProfile( "MatchSplitterPosition", OtherMatchPanel->Width );
      SplittersChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::ArchiveMatchSplitterMoved(TObject */*Sender*/)
{
   if (!WArchiveMatchSplitter->Minimized)
   {
      TContestApp::getContestApp() ->displayBundle.setIntProfile( "ArchiveMatchSplitterPosition", ArchiveMatchPanel->Width );
      SplittersChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::NextContactDetailsSplitterMoved( TObject */*Sender*/)
{
   if (!WNextContactDetailsSplitter->Minimized)
   {
      TContestApp::getContestApp() ->displayBundle.setIntProfile( "NextContactDetailsSplitterPosition", NextContactDetailsPanel->Width );
      SplittersChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::MultSplitterMoved(TObject */*Sender*/)
{
   if (!WMultSplitter->Minimized)
   {
      TContestApp::getContestApp() ->displayBundle.setIntProfile( "MultSplitterPosition", MultPanel->Width );
      SplittersChanged();
   }
}

//---------------------------------------------------------------------------
void TSingleLogFrame::SplittersChanged()
{
   MinosLoggerEvents::SendSplittersChanged();
   /*
   int cf = LogContainer->ContestPageControl->PageCount;
   for ( int j = 0; j < cf; j++ )
   {
      int cc = LogContainer->ContestPageControl->Pages[ j ] ->ControlCount;
      for ( int i = 0; i < cc; i++ )
      {
         if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( LogContainer->ContestPageControl->Pages[ j ] ->Controls[ i ] ) )
         {
            f->splittersChanged = true;
         }
      }
   }
   */
}
void TSingleLogFrame::SplittersChanged_Event ( MinosEventBase & /*Event*/ )
{
// No need to check for current log
   splittersChanged = true;
}
//---------------------------------------------------------------------------
void TSingleLogFrame::getSplitters()
{
   int iTemp;
   bool minimised;

   minimised = WLogAreaSplitter->Minimized;
   TContestApp::getContestApp() ->displayBundle.getIntProfile( "LogSplitterPosition", iTemp, MatchPanel->Height );
   MatchPanel->Height = iTemp;
   if (minimised)
   {
      WLogAreaSplitter->Minimize();
   }

   minimised = WNextContactDetailsSplitter->Minimized;
   TContestApp::getContestApp() ->displayBundle.getIntProfile( "NextContactDetailsSplitterPosition", iTemp, NextContactDetailsPanel->Width );
   NextContactDetailsPanel->Width = iTemp;
   if (minimised)
   {
      WNextContactDetailsSplitter->Minimize();
   }

   minimised = WMultSplitter->Minimized;
   TContestApp::getContestApp() ->displayBundle.getIntProfile( "MultSplitterPosition", iTemp, MultPanel->Width );
   MultPanel->Width = iTemp;
   if (minimised)
   {
      WMultSplitter->Minimize();
   }

   minimised = WMatchSplitter->Minimized;
   TContestApp::getContestApp() ->displayBundle.getIntProfile( "MatchSplitterPosition", iTemp, OtherMatchPanel->Width );
   OtherMatchPanel->Width = iTemp;
   if (minimised)
   {
      WMatchSplitter->Minimize();
   }

   minimised = WArchiveMatchSplitter->Minimized;
   TContestApp::getContestApp() ->displayBundle.getIntProfile( "ArchiveMatchSplitterPosition", iTemp, ArchiveMatchPanel->Width );
   ArchiveMatchPanel->Width = iTemp;
   if (minimised)
   {
      WArchiveMatchSplitter->Minimize();
   }


   splittersChanged = false;
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::EntryChoiceMenuPopup(TObject */*Sender*/)
{
   bool showEdit = false;
   POINT mpos, mpos2;
   ::GetCursorPos( &mpos );
   mpos2 = OtherMatchTree->ScreenToClient( mpos );
   if ( PtInRect( &( OtherMatchTree->ClientRect ), mpos2 ))
   {
      showEdit = true;
   }
// If from "other" match list, we want to enable an "edit contact" option
   MenuEditContact->Visible = showEdit;
   MenuEditSeparator->Visible = showEdit;

}
//---------------------------------------------------------------------------
void TSingleLogFrame::EditMatchContact_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_EditMatchContact> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_EditMatchContact> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      EditMatchContact();
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::EditMatchContact()
{
   otherMatchTreeSelect( otherTreeClickNode );
}
//---------------------------------------------------------------------------
void TSingleLogFrame::NextContactDetailsOnLeft_Event ( MinosEventBase & Event )
{
//   ActionEventV<EN_NextContactDetailsOnLeft> & S = dynamic_cast<ActionEventV<EN_NextContactDetailsOnLeft> &> ( Event );
   OnShowTimer->Enabled = true;
}

//---------------------------------------------------------------------------
void TSingleLogFrame::ContestDetails_Event ( MinosEventBase & Event )
{
   ActionEvent1<BaseContestLog *, EN_ContestDetails> & S = dynamic_cast<ActionEvent1<BaseContestLog *, EN_ContestDetails> &> ( Event );
   BaseContestLog *ct = S.getData();

   if (ct == contest)
   {
      std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );

      LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );
      if ( ct )
      {
         pced->setDetails( ct );
         if ( pced->ShowModal() == mrOk )
         {
            // and we need to do some re-init on the display
            updateQSODisplay();
            ct->scanContest();
         }
      }
   }
}

//---------------------------------------------------------------------------
void TSingleLogFrame::ShowOperators_Event ( MinosEventBase & /*Event*/ )
{
   bool so = LogContainer->isShowOperators();
   GJVQSOLogFrame->SecondOpComboBox->Visible = so;
   GJVQSOLogFrame->SecondOpLabel->Visible = so;
   GJVQSOLogFrame->MainOpComboBox->Visible = so;
   GJVQSOLogFrame->OperatorLabel->Visible = so;
}

//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::ThisMatchTreeBeforeItemErase(
      TBaseVirtualTree *Sender, TCanvas */*TargetCanvas*/, PVirtualNode Node,
      TRect &/*ItemRect*/, TColor &ItemColor, TItemEraseAction &EraseAction)
{
   MatchNodeData * pc = ( MatchNodeData * ) Sender->GetNodeData( Node );
   BaseContestLog *clp = pc->matchedContest;
   if (  clp && clp->isCurDup( pc->matchedContact ) )
   {
         ItemColor = clRed;
         EraseAction = eaColor;
   }

}
//---------------------------------------------------------------------------


