/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CompDispH
#define CompDispH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TCompFrame : public TFrame
{
   __published:	// IDE-managed Components
      TVirtualStringTree *CompGrid;
   TTimer *RecheckTimer;
   void __fastcall RecheckTimerTimer(TObject *Sender);
   void __fastcall CompGridGetNodeDataSize(TBaseVirtualTree *Sender,
          int &NodeDataSize);
   void __fastcall CompGridGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType, WideString &CellText);
   void __fastcall CompGridInitChildren(TBaseVirtualTree *Sender,
          PVirtualNode Node, DWORD &ChildCount);
   void __fastcall CompGridInitNode(TBaseVirtualTree *Sender,
          PVirtualNode ParentNode, PVirtualNode Node,
          TVirtualNodeInitStates &InitialStates);
   private:  	// User declarations
      BaseContestLog *ct;
   public:  		// User declarations
      void reInitialise();
      void setContest( BaseContestLog *ct );
      __fastcall TCompFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCompFrame *CompFrame;
//---------------------------------------------------------------------------
#endif
