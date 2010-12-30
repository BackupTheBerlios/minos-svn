/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "display_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLogMonitorFrame *LogMonitorFrame;
//---------------------------------------------------------------------------
__fastcall TLogMonitorFrame::TLogMonitorFrame( TComponent* Owner )
      : TFrame( Owner ), myLogColumnsChanged(false)
{}
void TLogMonitorFrame::initialise( BaseContestLog * pcontest )
{
   contest = pcontest;
}
//---------------------------------------------------------------------------
void __fastcall TLogMonitorFrame::QSOTreeBeforeItemErase( TBaseVirtualTree */*Sender*/,
      TCanvas */*TargetCanvas*/, PVirtualNode Node, TRect &/*ItemRect*/,
      TColor &ItemColor, TItemEraseAction &EraseAction )
{
   BaseContact * ct = contest->pcontactAt( Node->Index );
   if ( ct )
   {
      if ( ct->contactFlags.getValue() & FORCE_LOG )
      {
         ItemColor = ( TColor ) ( 0x00C080FF );
         EraseAction = eaColor;
      }
      else
         if ( ct->getModificationCount() > 1 )
         {
            ItemColor = clMoneyGreen;
            EraseAction = eaColor;
         }
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogMonitorFrame::QSOTreeColumnResize( TVTHeader *Sender,
      TColumnIndex Column )
{
   // preserve the column size in the ini file
   MinosParameters::getMinosParameters() ->setDisplayColumnWidth( ( "LogColumn" + String( Column ) ).c_str(), - 1 );
   MinosParameters::getMinosParameters() ->setDisplayColumnWidth( ( "LogColumnWidth" + String( Column ) ).c_str(), Sender->Columns->Items[ Column ] ->Width );
   myLogColumnsChanged = true;
   LogColumnsChangeTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TLogMonitorFrame::QSOTreeDblClick( TObject */*Sender*/ )
{
   QSOTree->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TLogMonitorFrame::QSOTreeGetNodeDataSize( TBaseVirtualTree */*Sender*/,
      int &NodeDataSize )
{
   NodeDataSize = 0;
}
//---------------------------------------------------------------------------
void __fastcall TLogMonitorFrame::QSOTreeGetText( TBaseVirtualTree */*Sender*/,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType /*TextType*/,
      WideString &CellText )
{
   BaseContact * cont = contest->pcontactAt( Node->Index );
   if ( cont && Column >= 0 )
   {
      std::string line = cont->getField( QSOTreeColumns[ Column ].fieldId, contest );
      CellText = line.c_str();
   }
   else
   {
      CellText = "";
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogMonitorFrame::QSOTreePaintText( TBaseVirtualTree */*Sender*/,
      const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType /*TextType*/ )
{
   TColor multhighlight = clRed;
   BaseContact *ct = contest->pcontactAt( Node->Index );
   if ( ct )
   {
      switch ( QSOTreeColumns[ Column ].fieldId )
      {
         case egTime:
/*
            if (ct->time.isBadDtg())
            {
               TargetCanvas->Font->Color = clBlue;
            }
            else
*/
            if (!contest->checkTime(ct->time))
            {
               TargetCanvas->Font->Color = multhighlight;
            }
            break;
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
//---------------------------------------------------------------------------
void TLogMonitorFrame::showQSOs()
{
   QSOTree->Canvas->Font->Assign( Font );
   QSOTree->BeginUpdate();
   QSOTree->Clear();
   QSOTree->RootNodeCount = 0;

   QSOTree->RootNodeCount = contest->getContactCount() + 1;  // The magic of virtual displays...

   QSOTree->Header->Columns->Clear();
   QSOTree->Margin = 0;
   QSOTree->TextMargin = 4;
   for ( int i = 0; i < LOGTREECOLS; i++ )
   {
      TVirtualTreeColumn *NewColumn = QSOTree->Header->Columns->Add();
      NewColumn->Alignment = QSOTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      int temp = -1;
      String key = "LogColumnWidth" + String( i );
      MinosParameters::getMinosParameters() ->getDisplayColumnWidth( key.c_str(), temp, -1 );

      if ( temp > 0 )
         NewColumn->Width = temp;
      else
         NewColumn->Width = QSOTree->Canvas->TextWidth( QSOTreeColumns[ i ].width );

      NewColumn->Text = QSOTreeColumns[ i ].title;
   }

   QSOTree->EndUpdate();
   QSOTree->Header->Options = ( QSOTree->Header->Options << hoVisible );
   myLogColumnsChanged = false;
}


void __fastcall TLogMonitorFrame::LogColumnsChangeTimerTimer(TObject *Sender)
{
   LogColumnsChangeTimer->Enabled = false;
   if (myLogColumnsChanged)
   {
      MinosLoggerEvents::SendLogColumnsChanged();
      myLogColumnsChanged = false;
   }
}
//---------------------------------------------------------------------------

