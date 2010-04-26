#error no longer used
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef OpsFrameH
#define OpsFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <ExtCtrls.hpp>
#include "BundleFrame.h"
//---------------------------------------------------------------------------
class TOperatorFrame : public TFrame
{
   __published: 	// IDE-managed Components
      TLabel *Label1;
      TLabel *Label2;
      TComboBox *MainOpComboBox;
      TComboBox *SecondOpComboBox;
      void __fastcall MainOpComboBoxKeyPress( TObject *Sender, char &Key );
      void __fastcall MainOpComboBoxExit( TObject *Sender );
   void __fastcall SecondOpComboBoxExit(TObject *Sender);
   private: 	// User declarations
   public: 		// User declarations
      __fastcall TOperatorFrame( TComponent* Owner );
      void refreshOps();
};
//---------------------------------------------------------------------------
extern PACKAGE TOperatorFrame *OperatorFrame;
//---------------------------------------------------------------------------
#endif
