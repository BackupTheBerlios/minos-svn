/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef MinosMonitorMainH
#define MinosMonitorMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TMonitorMain : public TForm
{
   __published:  	// IDE-managed Components
      TTimer *LogTimer;
      TTimer *StartupTimer;
      TTimer *ConnectTimer;
      TPanel *Panel1;
      TButton *CloseButton;
      TVirtualStringTree *MonitorTree;
      TTimer *MonitorTimer;
      TSplitter *Splitter1;
      TPageControl *ContestPageControl;
      TLabel *ScoreLabel;
      TPopupMenu *PopupMenu1;
      TMenuItem *Close1;
      TMenuItem *Cancel1;
   TButton *FocusCurrentButton;
   TImage *Splitter_Image;
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall StartupTimerTimer( TObject *Sender );
      void __fastcall FormResize( TObject *Sender );
      void __fastcall ConnectTimerTimer( TObject *Sender );
      void __fastcall MonitorTreeGetNodeDataSize( TBaseVirtualTree *Sender,
            int &NodeDataSize );
      void __fastcall MonitorTreeInitNode( TBaseVirtualTree *Sender,
                                           PVirtualNode ParentNode, PVirtualNode Node,
                                           TVirtualNodeInitStates &InitialStates );
      void __fastcall MonitorTreeInitChildren( TBaseVirtualTree *Sender,
            PVirtualNode Node, DWORD &ChildCount );
      void __fastcall MonitorTreeGetText( TBaseVirtualTree *Sender,
                                          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
                                          WideString &CellText );
      void __fastcall MonitorTreeDblClick( TObject *Sender );
      void __fastcall MonitorTreeMouseDown( TObject *Sender, TMouseButton Button,
                                            TShiftState Shift, int X, int Y );
      void __fastcall MonitorTimerTimer( TObject *Sender );
      void __fastcall Close1Click( TObject *Sender );
      void __fastcall Cancel1Click( TObject *Sender );
   void __fastcall FocusCurrentButtonClick(TObject *Sender);
   private:  	// User declarations
      std::string localServerName;
      bool subscribed;
      bool saveResize;
      std::vector<MonitoredStation *> stationList;
      PVirtualNode MonitorTreeClickNode;

      void syncStations();
      void addSlot( MonitoredLog *ct );
      TMonitoringFrame *findCurrentLogFrame();
      TTabSheet *findContestPage( BaseContestLog *ct );
   public:  		// User declarations
      __fastcall TMonitorMain( TComponent* Owner );
      __fastcall ~TMonitorMain( );
      void __fastcall logMessage( std::string s );
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void loggerSubscribeClientCallback( bool err, MinosRPCObj *mro, const std::string &from );

      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TMonitorMain *MonitorMain;
//---------------------------------------------------------------------------
#endif

