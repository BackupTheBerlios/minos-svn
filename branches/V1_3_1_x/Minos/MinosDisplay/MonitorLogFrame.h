/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef MonitorLogFrameH
#define MonitorLogFrameH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"

//---------------------------------------------------------------------------
class TLogMonitorFrame : public TFrame
{
   __published: 	// IDE-managed Components
      TVirtualStringTree *QSOTree;
      void __fastcall QSOTreeBeforeItemErase( TBaseVirtualTree *Sender,
                                              TCanvas *TargetCanvas, PVirtualNode Node, TRect &ItemRect,
                                              TColor &ItemColor, TItemEraseAction &EraseAction );
      void __fastcall QSOTreeColumnResize( TVTHeader *Sender, TColumnIndex Column );
      void __fastcall QSOTreeDblClick( TObject *Sender );
      void __fastcall QSOTreeGetNodeDataSize( TBaseVirtualTree *Sender,
                                              int &NodeDataSize );
      void __fastcall QSOTreeGetText( TBaseVirtualTree *Sender, PVirtualNode Node,
                                      TColumnIndex Column, TVSTTextType TextType, WideString &CellText );
      void __fastcall QSOTreePaintText( TBaseVirtualTree *Sender,
                                        const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
                                        TVSTTextType TextType );
   private: 	// User declarations
      BaseContestLog * contest;
   public: 		// User declarations
      //   __fastcall TLogMonitorFrame(TComponent* Owner, LoggerContestLog * contest);
      __fastcall TLogMonitorFrame( TComponent* Owner );
      void initialise( BaseContestLog * contest );
      void showQSOs();
      BaseContestLog * getContest()
      {
         return contest;
      }
};
//---------------------------------------------------------------------------
extern PACKAGE TLogMonitorFrame *LogMonitorFrame;
//---------------------------------------------------------------------------
#endif
