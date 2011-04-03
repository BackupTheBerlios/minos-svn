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

#include "display_pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MonitorLogFrame"
#pragma link "MultDisp"
#pragma resource "*.dfm"
TMonitoringFrame *MonitoringFrame;
//---------------------------------------------------------------------------
__fastcall TMonitoringFrame::TMonitoringFrame( TComponent* Owner )
      : TFrame( Owner ), contest( 0 )
{
   WMultSplitter = new TWhisperSplitter(MultSplitter, MultPanel);
	WMultSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WMultSplitter->HighlightColor = clSkyBlue;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TMonitoringFrame::initialise( BaseContestLog * pcontest )
{
   contest = pcontest;
   LogMonitorFrame->initialise( pcontest );
}
//---------------------------------------------------------------------------
void TMonitoringFrame::showQSOs()
{
   LogMonitorFrame->showQSOs();
}
//---------------------------------------------------------------------------

void __fastcall TMonitoringFrame::LogMonitorFrameQSOTreeDblClick(
   TObject *Sender )
{
   LogMonitorFrame->QSOTreeDblClick( Sender );

}
//---------------------------------------------------------------------------

void __fastcall TMonitoringFrame::LogMonitorFrameQSOTreeGetNodeDataSize(
   TBaseVirtualTree *Sender, int &NodeDataSize )
{
   LogMonitorFrame->QSOTreeGetNodeDataSize( Sender, NodeDataSize );

}
//---------------------------------------------------------------------------

void __fastcall TMonitoringFrame::LogMonitorFrameQSOTreeGetText(
   TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
   TVSTTextType TextType, WideString &CellText )
{
   LogMonitorFrame->QSOTreeGetText( Sender, Node, Column, TextType, CellText );

}
//---------------------------------------------------------------------------


void __fastcall TMonitoringFrame::LogMonitorFrameQSOTreeBeforeItemErase(
   TBaseVirtualTree *Sender, TCanvas *TargetCanvas, PVirtualNode Node,
   TRect &ItemRect, TColor &ItemColor, TItemEraseAction &EraseAction )
{
   LogMonitorFrame->QSOTreeBeforeItemErase( Sender, TargetCanvas, Node,
         ItemRect, ItemColor, EraseAction );

}
//---------------------------------------------------------------------------

void __fastcall TMonitoringFrame::LogMonitorFrameQSOTreePaintText(
   TBaseVirtualTree *Sender, const TCanvas *TargetCanvas, PVirtualNode Node,
   TColumnIndex Column, TVSTTextType TextType )
{
   LogMonitorFrame->QSOTreePaintText( Sender, TargetCanvas, Node, Column,
                                      TextType );

}
//---------------------------------------------------------------------------

void __fastcall TMonitoringFrame::InitTimerTimer( TObject */*Sender*/ )
{
//   MultSplitter->Maximized = true;
   InitTimer->Enabled = false;
   MultDispFrame->setContest( contest );
   MultDispFrame->ErrTab->TabVisible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMonitoringFrame::MultSplitterMoved(TObject */*Sender*/)
{
#warning splitter position should be persisted   
}
//---------------------------------------------------------------------------

