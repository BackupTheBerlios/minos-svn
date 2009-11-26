/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MonitorFrameH
#define MonitorFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MonitorLogFrame.h"
#include <ExtCtrls.hpp>
#include "MultDisp.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMonitoringFrame : public TFrame
{
   __published: 	// IDE-managed Components
      TStatusBar *MonitorStatusBar;
      TLogMonitorFrame *LogMonitorFrame;
      TSplitter *MultSplitter;
      TMultDispFrame *MultDispFrame;
      TTimer *InitTimer;
      void __fastcall LogMonitorFrameQSOTreeDblClick( TObject *Sender );
      void __fastcall LogMonitorFrameQSOTreeGetNodeDataSize(
         TBaseVirtualTree *Sender, int &NodeDataSize );
      void __fastcall LogMonitorFrameQSOTreeGetText( TBaseVirtualTree *Sender,
            PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
            WideString &CellText );
      void __fastcall LogMonitorFrameQSOTreeBeforeItemErase(
         TBaseVirtualTree *Sender, TCanvas *TargetCanvas, PVirtualNode Node,
         TRect &ItemRect, TColor &ItemColor, TItemEraseAction &EraseAction );
      void __fastcall LogMonitorFrameQSOTreePaintText( TBaseVirtualTree *Sender,
            const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
            TVSTTextType TextType );
      void __fastcall InitTimerTimer( TObject *Sender );
   private: 	// User declarations
      BaseContestLog *contest;
   public: 		// User declarations
      __fastcall TMonitoringFrame( TComponent* Owner );
      void initialise( BaseContestLog * contest );
      void showQSOs();
      BaseContestLog * getContest()
      {
         return contest;
      }
};
//---------------------------------------------------------------------------
extern PACKAGE TMonitoringFrame *MonitoringFrame;
//---------------------------------------------------------------------------
#endif
