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
      currFreq( 0 ), oldFreq( 0 ),
      lastStanzaCount( 0 ),
      matchTreeClickNode( 0 ),
      otherTreeClickNode( 0 ),
      archiveTreeClickNode( 0 ),
      xferTree( 0 )

{
   Parent = ( TWinControl * ) Owner;               // This makes the JEDI splitter work!
   TTabSheet *OpTabSheet = new TTabSheet( MultDispFrame );
   OpTabSheet->Caption = "Ops";
   OperatorFrame = new TOperatorFrame( OpTabSheet );
   OpTabSheet->PageControl = MultDispFrame->MultPages;
   OperatorFrame->Parent = OpTabSheet;
   OperatorFrame->Align = alClient;
   OperatorFrame->ParentFont = true;

   LogMonitor->initialise( contest );
   GJVQSOLogFrame->initialise( contest, this );

   OperatorFrame->refreshOps();

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

   showMatchHeaders();
   OnShowTimer->Enabled = true;
}
//---------------------------------------------------------------------------
__fastcall TSingleLogFrame::~TSingleLogFrame()
{}
//---------------------------------------------------------------------------
void TSingleLogFrame::closeContest()
{
   if ( TContestApp::getContestApp() )
   {
      RPCPubSub::publish( "ContestLog", contest->publishedName, makeStr( 0 ), psRevoked );
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
TWinControl *TSingleLogFrame::getActiveControl()
{
   return LogContainer->ActiveControl;
}
//---------------------------------------------------------------------------
void TSingleLogFrame::reportOverstrike( bool overstrike )
{
   LogContainer->StatusBar1->Panels->Items[ 1 ] ->Text = overstrike ? "Overwrite" : "Insert";
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TSingleLogFrame::showQSOs()
{
   TWaitCursor fred;

   LogMonitor->Font->Assign( LogContainer->Font );

   NextContactDetailsTimerTimer( this );

   int iTemp;
   TContestApp::getContestApp() ->displayBundle.getIntProfile( "LogSplitterPosition", iTemp, LogAreaSplitter->Height );
   LogAreaSplitter->Height = iTemp;

   LogMonitor->showQSOs();
   logColumnsChanged = false;

   GJVQSOLogFrame->clearCurrentField();
   GJVQSOLogFrame->selectEntry( 0 );

}
//---------------------------------------------------------------------------
void TSingleLogFrame::EditContact( BaseContact *lct )
{
   if ( lct )
   {
      TQSOEditDlg * ptr = new TQSOEditDlg( this );
      std::auto_ptr <TQSOEditDlg> qdlg( ptr );

      ContestContact *ct = dynamic_cast<ContestContact *>( lct );
      qdlg->selectContact( contest, ct );

      qdlg->ShowModal();

      contest->startScan();
      LogMonitor->Invalidate();
      MultDispFrame->refreshMults();
      OperatorFrame->refreshOps();
      LogMonitor->Repaint();
      //GJVQSOLogFrame->selectEntry( 0 );
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::QSOTreeSelect( PVirtualNode sel )
{
   if ( sel )
   {
      DisplayContestContact * lct = dynamic_cast<DisplayContestContact*>( contest->pcontactAt( sel->Index ) );
      EditContact( lct );

      if ( lct->op1.getValue().size() )
      {
         contest->oplist.insert( lct->op1.getValue() );
      }
      if ( lct->op2.getValue().size() )
      {
         contest->oplist.insert( lct->op2.getValue() );
      }
      if ( ( int ) sel->Index == contest->getContactCount() - 1 )
      {
         contest->op1.setValue( lct->op1 );
         contest->op2.setValue( lct->op2 );
      }
      OperatorFrame->refreshOps();
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
void __fastcall TSingleLogFrame::archiveMatchTreeSelect( PVirtualNode sel )
{
   if ( !sel )
      return ;
   MatchNodeData *MatchTreeIndex = ( MatchNodeData * ) ( ArchiveMatchTree->GetNodeData( sel ) );
   BaseContestLog *mclp = MatchTreeIndex->matchedContest;
   if ( MatchTreeIndex && mclp && MatchTreeIndex->matchedContact )
   {
      if ( MinosParameters::getMinosParameters() ->yesNoMessage( this, "ARCHIVE LIST - Do you wish to switch?" ) )
      {
         LogContainer->selectContest( MatchTreeIndex->matchedContest, MatchTreeIndex->matchedContact );
      }
      ArchiveMatchTree->Repaint();
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

//      Splitter1->Maximized = true;     // hide the archive list
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
//            Splitter1->Maximized = false;     // hide the archive list
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

//      Splitter3->Maximized = true;     // hide the archive list
      return ;
   }
//   Splitter3->Maximized = false;     // expose the archive list

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
//---------------------------------------------------------------------------
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

      TMatchThread::startMatch();						// make sure rematched
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

      TMatchThread::startMatch();						// make sure rematched
   }
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::QSOTreeColumnResize( TVTHeader *Sender,
      TColumnIndex Column )
{
   LogMonitor->QSOTreeColumnResize( Sender, Column );
   // preserve the column size in the ini file
   //   TContestApp::getContestApp() ->displayBundle.setIntProfile( ( "LogColumn" + String( Column ) ).c_str(), Sender->Columns->Items[ Column ] ->Width );

   // and this should happen for all columns...
   int cf = LogContainer->ContestPageControl->PageCount;
   for ( int j = 0; j < cf; j++ )
   {
      int cc = LogContainer->ContestPageControl->Pages[ j ] ->ControlCount;
      for ( int i = 0; i < cc; i++ )
      {
         if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( LogContainer->ContestPageControl->Pages[ j ] ->Controls[ i ] ) )
         {
            if ( Column < THISMATCHTREECOLS )
            {
               int temp = -1;
               String key = "LogColumn" + String( Column );
               TContestApp::getContestApp() ->displayBundle.getIntProfile( key.c_str(), temp, -1 );
               TVirtualTreeColumn *CurColumn = LogMonitor->QSOTree->Header->Columns->Items[ Column ];
               if ( CurColumn->Width != temp )
               {
                  f->logColumnsChanged = true;
               }
            }

            break;
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::LogAreaSplitterMoved( TObject */*Sender*/ )
{
   // preserve size of "bottom" Panel2
   TContestApp::getContestApp() ->displayBundle.setIntProfile( "LogSplitterPosition", LogAreaSplitter->Height );
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
void __fastcall TSingleLogFrame::OtherMatchTreeDblClick( TObject */*Sender*/ )
{
   otherMatchTreeSelect( otherTreeClickNode );
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
   //   archiveMatchTreeSelect( matchTreeClickNode );
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
void __fastcall TSingleLogFrame::ArchiveMatchTreeKeyDown( TObject */*Sender*/,
      WORD &Key, TShiftState /*Shift*/ )
{
   if ( Key == VK_SPACE )
   {
      Key = 0;
      //      archiveMatchTreeSelect( ArchiveMatchTree->FocusedNode );
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
void TSingleLogFrame::setMode( String m )
{
   GJVQSOLogFrame->setMode( m );
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
void __fastcall TSingleLogFrame::FrameMouseMove( TObject */*Sender*/,
      TShiftState /*Shift*/, int /*X*/, int /*Y*/ )
{
   LogContainer->GridHintWindow->SetHintControl( this );
   LogContainer->GridHintWindow->Showing = false;
   LogContainer->GridHintTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::QSOTreeNewText( TBaseVirtualTree */*Sender*/,
      PVirtualNode /*Node*/, TColumnIndex /*Column*/, WideString /*NewText*/ )
{
   /*
      // it has been edited, so set it
      //bool ContestContact::setField(int ACol, const std::string Value)
      ContestContact * cont = contest->pcontactAt( Node->Index );
      if ( cont )
      {
         if ( Column )
         {
            cont->setField( Column - 1, String( NewText ).c_str() );
         }
      }
      */
}
//---------------------------------------------------------------------------
void __fastcall TSingleLogFrame::SetTimeNowClick( TObject */*Sender*/ )
{
   GJVQSOLogFrame->setTimeNow();
}
//---------------------------------------------------------------------------
void TSingleLogFrame::setActiveControl( WORD &Key )
{
   GJVQSOLogFrame->setActiveControl( Key );
   if (
      LogContainer->ActiveControl != LogMonitor->QSOTree
      && LogContainer->ActiveControl != ThisMatchTree
      && LogContainer->ActiveControl != OtherMatchTree
      && Key == VK_PRIOR )
   {
      LogContainer->ActiveControl = LogMonitor->QSOTree;
      Key = 0;
   }
   else
      if (
         LogContainer->ActiveControl != LogMonitor->QSOTree
         && LogContainer->ActiveControl != ThisMatchTree
         && LogContainer->ActiveControl != OtherMatchTree
         && Key == VK_NEXT )
      {
         LogContainer->ActiveControl = ThisMatchTree;
         Key = 0;
      }
      else
         if (Key == VK_F12 && GJVQSOLogFrame->MatchXferButton->Enabled)
         {
            GJVQSOLogFrame1MatchXferButtonClick(this);
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
void TSingleLogFrame::afterSelectEntry( BaseContact *lct )
{
   // How do we find the correct PVirtualNode for lct?
   if ( lct )
   {
      EditContact( lct );
   }
   else
   {
      PVirtualNode l = LogMonitor->QSOTree->GetLastChild( LogMonitor->QSOTree->RootNode );
      LogMonitor->QSOTree->FocusedNode = l;
      LogMonitor->QSOTree->Selected[ l ] = true;
      TMatchThread::startMatch();
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::showScreenEntry( ScreenContact &/*screenContact*/ )
{
   // callback
}
//---------------------------------------------------------------------------
void TSingleLogFrame::getScreenEntry( ScreenContact &screenContact )
{
   // callback
   if ( contest )
   {
      screenContact.op1.setValue( contest->op1 );
      screenContact.op2.setValue( contest->op2 );
   }
}
//---------------------------------------------------------------------------
void TSingleLogFrame::afterLogContact()
{
   // callback
   updateTrees();
   afterSelectEntry( 0 );
   NextContactDetailsTimerTimer( this );
}
//---------------------------------------------------------------------------
void TSingleLogFrame::updateTrees()
{
   LogMonitor->QSOTree->RootNodeCount = contest->getContactCount() + 1;
   MultDispFrame->refreshMults();
   OperatorFrame->refreshOps();
   LogMonitor->QSOTree->Invalidate();
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
      contest->startScan();
      LogMonitor->QSOTree->Invalidate();
      MultDispFrame->refreshMults();
      OperatorFrame->refreshOps();
      LogMonitor->QSOTree->Repaint();
      //GJVQSOLogFrame->selectEntry( 0 );
   }
   else
   {
      ShowMessage( "No unfilled contacts" );
   }

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
   if ( ct && ct->isMinosFile() && !ct->isReadOnly() && !ct->isPostEntry() )
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
void TSingleLogFrame::showErrorList( ErrorList &errs )
{
   MultDispFrame->showErrorList( errs );
}








void __fastcall TSingleLogFrame::MatchSplitterMoved(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::ArchiveMatchSplitterMoved(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::NextContactDetailsSplitterMoved(
      TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TSingleLogFrame::MultSplitterMoved(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

