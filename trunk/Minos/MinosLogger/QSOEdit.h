/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef QSOEditH
#define QSOEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BundleFrame.h"
#include "VirtualTrees.hpp"
#include <ExtCtrls.hpp>
#include "QSOFrame.h"
#include "QSOEditFrame.h"
//---------------------------------------------------------------------------
class TQSOEditDlg : public TForm
{
   __published: 	// IDE-managed Components
      TVirtualStringTree *QSOHistoryTree;
      TGJVQSOEditFrame *GJVQSOEditFrame;
   TTimer *InitialiseTimer;
      void __fastcall QSOHistoryTreeGetNodeDataSize( TBaseVirtualTree *Sender,
            int &NodeDataSize );
      void __fastcall QSOHistoryTreeGetText( TBaseVirtualTree *Sender,
                                             PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
                                             WideString &CellText );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall QSOHistoryTreeInitNode( TBaseVirtualTree *Sender,
                                              PVirtualNode ParentNode, PVirtualNode Node,
                                              TVirtualNodeInitStates &InitialStates );
      void __fastcall QSOHistoryTreeInitChildren( TBaseVirtualTree *Sender,
            PVirtualNode Node, DWORD &ChildCount );
      void __fastcall GJVQSOEditFrame1GJVOKButtonClick( TObject *Sender );
      void __fastcall GJVQSOEditFrame1GJVForceButtonClick( TObject *Sender );
      void __fastcall GJVQSOEditFrame1GJVCancelButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall FormKeyUp( TObject *Sender, WORD &Key, TShiftState Shift );
      void __fastcall GJVQSOEditFrameNextButtonClick( TObject *Sender );
      void __fastcall QSOHistoryTreeBeforeItemErase( TBaseVirtualTree *Sender,
            TCanvas *TargetCanvas, PVirtualNode Node, TRect &ItemRect,
            TColor &ItemColor, TItemEraseAction &EraseAction );
   void __fastcall InitialiseTimerTimer(TObject *Sender);
   private: 	// User declarations

      MinosEventListener  EL_AfterSelectContact;
      void AfterSelectContact_Event ( MinosEventBase & Event );

      BaseContestLog * contest;
      BaseContact *firstContact;
      bool catchup;
      void refreshOps( ScreenContact &screenContact );
   public: 		// User declarations
      __fastcall TQSOEditDlg( TComponent* Owner );
      void selectContact( BaseContestLog * contest, DisplayContestContact *lct );
      void setContest( BaseContestLog * c )
      {
         contest = c;
      }
      void selectCatchup( BaseContestLog * contest );
      void setFirstContact( BaseContact *c )
      {
         firstContact = c;
      }
};
//---------------------------------------------------------------------------
extern PACKAGE TQSOEditDlg *QSOEditDlg;
//---------------------------------------------------------------------------
#endif
