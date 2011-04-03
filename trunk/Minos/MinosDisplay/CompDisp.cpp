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

#include "MinosVer.h"   // updated by SubWCRev
#include "CompDisp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TCompFrame *CompFrame;
//---------------------------------------------------------------------------
std::vector<ContestScore> scores;
//---------------------------------------------------------------------------
enum eCompGridCols {egtName, egtQSOs, egtPoints, egtCountries, egtDistricts, egtLocs, egtMults, egtScore,
                    egtCompMaxCol
                   };
static GridColumn CompGridColumns[ egtCompMaxCol ] =
   {
      GridColumn( egtName, "This is a Contest", "Name", taLeftJustify ),
      GridColumn( egtQSOs, "123", "QSOs", taCenter ),
      GridColumn( egtPoints, "XX100000", "Pts", taLeftJustify ),
      GridColumn( egtCountries, "XX123", "Ctry", taCenter ),
      GridColumn( egtDistricts, "XX123", "Dist", taCenter ),
      GridColumn( egtLocs, "XX123", "Locs", taCenter ),
      GridColumn( egtMults, "XX1234", "Mlt", taCenter ),
      GridColumn( egtScore, "XX123456", "Score", taCenter )
   };
//---------------------------------------------------------------------------
__fastcall TCompFrame::TCompFrame(TComponent* Owner)
   : TFrame(Owner), ct( 0 )
{
#ifdef BETA_VERSION
   CompGrid->BeginUpdate();

   CompGrid->Header->Columns->Clear();
   CompGrid->Margin = 0;
   CompGrid->TextMargin = 4;
   for ( int i = 0; i < egtCompMaxCol; i++ )
   {
      TVirtualTreeColumn *NewColumn = CompGrid->Header->Columns->Add();
      NewColumn->Alignment = CompGridColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      NewColumn->Width = CompGrid->Canvas->TextWidth( CompGridColumns[ i ].width );

      NewColumn->Text = CompGridColumns[ i ].title;
   }
   CompGrid->EndUpdate();
   CompGrid->Header->Options = ( CompGrid->Header->Options << hoVisible );
#endif
}
//---------------------------------------------------------------------------
void TCompFrame::setContest( BaseContestLog *pct )
{
   ct = pct;
}
//---------------------------------------------------------------------------
void TCompFrame::reInitialise()
{
   // here in MinosDisplay we haven't got access to the slot list...
   // I think we need to call back to the app to get a vector with all
   // the info that we need (vector of contestScoreData - needs defining!)

#ifdef BETA_VERSION
   std::vector<BaseContestLog *> logList = MinosParameters::getMinosParameters() ->getContestList();
   scores.clear();
   for (std::vector<BaseContestLog *>::iterator i = logList.begin(); i != logList.end(); i++)
   {
      ContestScore cs((*i), TDateTime::CurrentDateTime());
      scores.push_back(cs);
   }
   CompGrid->RootNodeCount = scores.size();
   CompGrid->ValidateNode( 0, true );
#endif
}
//---------------------------------------------------------------------------
void __fastcall TCompFrame::RecheckTimerTimer(TObject */*Sender*/)
{
   if ( !Visible )
      return ;

   // we don't need a full re-init; just a recalc and repaint
   reInitialise();
}
//---------------------------------------------------------------------------

void __fastcall TCompFrame::CompGridGetNodeDataSize(TBaseVirtualTree */*Sender*/,
      int &NodeDataSize)
{
   NodeDataSize = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCompFrame::CompGridGetText(TBaseVirtualTree */*Sender*/,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType /*TextType*/,
      WideString &CellText)
{
   CellText = "";
   if (Node->Index < scores.size())
   {
      switch (Column)
      {
         case egtName:
            CellText = scores[Node->Index].name.c_str();
            break;
         case egtQSOs:
            CellText = String(scores[Node->Index].nqsos);
            break;
         case egtPoints:
            CellText = String(scores[Node->Index].contestScore);
            break;
         case egtCountries:
            CellText = String(scores[Node->Index].nctry);
            break;
         case egtDistricts:
            CellText = String(scores[Node->Index].ndistrict);
            break;
         case egtLocs:
            CellText = String(scores[Node->Index].nlocs);
            break;
         case egtMults:
            CellText = String(scores[Node->Index].nmults);
            break;
         case egtScore:
            CellText = String(scores[Node->Index].totalScore);
            break;
      }

   }
}
//---------------------------------------------------------------------------

void __fastcall TCompFrame::CompGridInitChildren(TBaseVirtualTree */*Sender*/,
      PVirtualNode /*Node*/, DWORD &/*ChildCount*/)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TCompFrame::CompGridInitNode(TBaseVirtualTree */*Sender*/,
      PVirtualNode /*ParentNode*/, PVirtualNode /*Node*/,
      TVirtualNodeInitStates &/*InitialStates*/)
{
//
}
//---------------------------------------------------------------------------

