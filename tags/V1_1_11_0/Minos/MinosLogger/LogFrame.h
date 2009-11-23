/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef LogFrameH
#define LogFrameH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "VirtualTrees.hpp"
#include "BundleFrame.h"
#include <ComCtrls.hpp>
#include "MultDisp.h"
#include "JvExExtCtrls.hpp"
#include "JvNetscapeSplitter.hpp"
#include "QSOFrame.h"
#include "QSOLogFrame.h"
#include "MonitorLogFrame.h" 
//---------------------------------------------------------------------------
class TMatchCollection;
class MatchNodeData;
class ProtoContest;
// We may need to define our own validation controls with valid methods
// for each needed type...
//==========================================================
class MatchNodeData
{
   public:
      MatchNodeData();

      bool top;
      BaseContestLog *matchedContest;
      BaseContact *matchedContact;
};
class MatchNodeListData
{
   public:
      MatchNodeListData();

      bool top;
      ContactList *matchedList;
      ListContact *matchedContact;
};
//==========================================================
class TSingleLogFrame : public TFrame , public QSOEditScreen
{
   __published:  	// IDE-managed Components
      TPanel *Panel1;
      TPanel *DetailAndMatchPanel;
      TSplitter *LogAreaSplitter;
      TPanel *NextContactDetailsPanel;
      TPopupMenu *EntryChoiceMenu;
      TMenuItem *SetTimeNow;
      TMenuItem *Go1;
      TTimer *NextContactDetailsTimer;
      TPanel *MatchPanel;
      TVirtualStringTree *ThisMatchTree;
      TVirtualStringTree *OtherMatchTree;
      TPanel *Panel5;
      TVirtualStringTree *ArchiveMatchTree;
      TMenuItem *NextContactDetailsOnLeft;
      TPanel *MultPanel;
      TMultDispFrame *MultDispFrame;
      TPanel *TopPanel;
      TJvNetscapeSplitter *MultSplitter;
      TPanel *LogDetailPanel;
      TLabel *NextContactDetailsLabel;
      TMenuItem *Details1;
      TMenuItem *Close1;
      TMenuItem *Details2;
      TMenuItem *ProduceEntryExportFile1;
      TJvNetscapeSplitter *NextContactDetailsSplitter;
      TJvNetscapeSplitter *Splitter1;
      TJvNetscapeSplitter *Splitter3;
      TGJVQSOLogFrame *GJVQSOLogFrame;
      TMenuItem *GotoNextUnfilled1;
      TMenuItem *Cancel1;
      TLogMonitorFrame *LogMonitor;
      TTimer *PublishTimer;
      TTimer *OnShowTimer;
      TMenuItem *Shifttableft1;
      TMenuItem *Shifttabright1;
      void __fastcall ThisMatchTreeGetText( TBaseVirtualTree *Sender,
                                            PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
                                            WideString &CellText );
      void __fastcall ThisMatchTreeGetNodeDataSize( TBaseVirtualTree *Sender,
            int &NodeDataSize );
      void __fastcall QSOTreeColumnResize( TVTHeader *Sender,
                                           TColumnIndex Column );
      void __fastcall LogAreaSplitterMoved( TObject *Sender );
      void __fastcall SetTimeNowClick( TObject *Sender );
      void __fastcall ThisMatchTreeDblClick( TObject *Sender );
      void __fastcall QSOTreeNewText( TBaseVirtualTree *Sender,
                                      PVirtualNode Node, TColumnIndex Column, WideString NewText );
      void __fastcall ThisMatchTreeMouseDown( TObject *Sender, TMouseButton Button,
                                              TShiftState Shift, int X, int Y );
      void __fastcall NextContactDetailsTimerTimer( TObject *Sender );
      void __fastcall ThisMatchTreeEnter( TObject *Sender );
      void __fastcall ThisMatchTreeKeyDown( TObject *Sender, WORD &Key,
                                            TShiftState Shift );
      void __fastcall OtherMatchTreeDblClick( TObject *Sender );
      void __fastcall OtherMatchTreeEnter( TObject *Sender );
      void __fastcall OtherMatchTreeGetNodeDataSize( TBaseVirtualTree *Sender,
            int &NodeDataSize );
      void __fastcall OtherMatchTreeGetText( TBaseVirtualTree *Sender,
                                             PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
                                             WideString &CellText );
      void __fastcall OtherMatchTreeKeyDown( TObject *Sender, WORD &Key,
                                             TShiftState Shift );
      void __fastcall OtherMatchTreeMouseDown( TObject *Sender,
            TMouseButton Button, TShiftState Shift, int X, int Y );
      void __fastcall ArchiveMatchTreeDblClick( TObject *Sender );
      void __fastcall ArchiveMatchTreeEnter( TObject *Sender );
      void __fastcall ArchiveMatchTreeGetNodeDataSize(
         TBaseVirtualTree *Sender, int &NodeDataSize );
      void __fastcall ArchiveMatchTreeGetText( TBaseVirtualTree *Sender,
            PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
            WideString &CellText );
      void __fastcall ArchiveMatchTreeKeyDown( TObject *Sender, WORD &Key,
            TShiftState Shift );
      void __fastcall ArchiveMatchTreeMouseDown( TObject *Sender,
            TMouseButton Button, TShiftState Shift, int X, int Y );
      void __fastcall NextContactDetailsOnLeftClick( TObject *Sender );
      void __fastcall AutoBandmapTuneClick( TObject *Sender );
      void __fastcall AutoBandmapTimeClick( TObject *Sender );
      void __fastcall FrameMouseMove( TObject *Sender, TShiftState Shift, int X,
                                      int Y );
      void __fastcall ThisMatchTreePaintText( TBaseVirtualTree *Sender,
                                              const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
                                              TVSTTextType TextType );
      void __fastcall GJVQSOLogFrame1GJVOKButtonClick( TObject *Sender );
      void __fastcall GJVQSOLogFrame1GJVForceButtonClick( TObject *Sender );
      void __fastcall GJVQSOLogFrame1GJVCancelButtonClick( TObject *Sender );
      void __fastcall GJVQSOLogFrame1MatchXferButtonClick( TObject *Sender );
      void __fastcall LogMonitorQSOTreeDblClick( TObject *Sender );
      void __fastcall LogMonitorQSOTreeEnter( TObject *Sender );
      void __fastcall LogMonitorQSOTreeKeyDown( TObject *Sender, WORD &Key,
            TShiftState Shift );
      void __fastcall LogMonitorQSOTreeMouseDown( TObject *Sender,
            TMouseButton Button, TShiftState Shift, int X, int Y );
      void __fastcall PublishTimerTimer( TObject *Sender );
      void __fastcall OnShowTimerTimer( TObject *Sender );
   private:  	// User declarations

      TOperatorFrame *OperatorFrame;
      BaseContestLog * contest;
      int lastStanzaCount;
      //       boost::shared_ptr<LoggerContestLog> contest;

      long long currFreq;
      long long oldFreq;
      String sCurrFreq;
      String sOldFreq;

      void transferDetails( MatchNodeData *MatchTreeIndex );
      void transferDetails( MatchNodeListData *MatchTreeIndex );

      void __fastcall QSOTreeSelect( PVirtualNode sel );
      void __fastcall thisMatchTreeSelect( PVirtualNode sel );
      void __fastcall otherMatchTreeSelect( PVirtualNode sel );
      void __fastcall archiveMatchTreeSelect( PVirtualNode sel );

   public:  		// User declarations
      __fastcall TSingleLogFrame( TComponent* Owner, BaseContestLog * contest );
      __fastcall TSingleLogFrame::~TSingleLogFrame();
      void drawWrapText( TCanvas *c, TRect &CellRect, const String & CellText, int baseline = -1 );
      void showQSOs();
      void goSerial( );
      BaseContestLog * getContest();
      void closeContest();
      void getScreenEntry();
      void showMatchHeaders( );
      void showMatchQSOs( TMatchCollection *matchCollection );
      void showMatchList( TMatchCollection *matchCollection );
      void matchDistrict( const std::string &qth );
      void matchCountry( const std::string &csCs );
      void replaceContestList( TMatchCollection *matchCollection );
      void replaceListList( TMatchCollection *matchCollection );
      void setActiveControl( WORD &Key );
      String makeEntry( bool saveMinos );
      void exportContest();
      void EditContact( BaseContact *lct );

      bool logColumnsChanged;

      void lgTraceerr( int err );

      void setMode( String m );
      void setFreq( String f );
      void updateTrees();
      void updateQSOTime();
      void updateQSODisplay();

      bool getStanza( unsigned int stanza, std::string &stanzaData );
      void showErrorList( ErrorList &errs );

   public: 		// implementations of QSOEditScreen
      PVirtualNode QSOTreeClickNode;
      PVirtualNode matchTreeClickNode;
      PVirtualNode otherTreeClickNode;
      PVirtualNode archiveTreeClickNode;
      TVirtualStringTree *xferTree;

      void showScreenEntry( ScreenContact &screenContact );
      void getScreenEntry( ScreenContact &screenContact );
      void afterSelectEntry( BaseContact *lct );
      TWinControl *getActiveControl();
      void reportOverstrike( bool overstrike );
      void afterLogContact();
      void GoNextUnfilled();
      void doNextContactDetailsOnLeftClick( TObject *Sender );
};
//---------------------------------------------------------------------------
extern PACKAGE TSingleLogFrame *SingleLogFrame;
//---------------------------------------------------------------------------
#endif
