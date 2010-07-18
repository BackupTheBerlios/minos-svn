/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef QSOEditFrameH
#define QSOEditFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "QSOFrame.h"
#include <ExtCtrls.hpp>
#include "BundleFrame.h"
//---------------------------------------------------------------------------
class TGJVQSOEditFrame : public TGJVEditFrame
{
   __published: 	// IDE-managed Components
      TButton *InsertBeforeButton;
      TButton *InsertAfterButton;
      TButton *PriorButton;
      TButton *NextButton;
      TTimer *ROFieldTimer;
      void __fastcall InsertBeforeButtonClick( TObject *Sender );
      void __fastcall InsertAfterButtonClick( TObject *Sender );
      void __fastcall FirstUnfilledButtonClick( TObject *Sender );
      void __fastcall PriorButtonClick( TObject *Sender );
      void __fastcall NextButtonClick( TObject *Sender );
      void __fastcall ROFieldTimerTimer( TObject *Sender );
   void __fastcall MainOpComboBoxExit(TObject *Sender);
   void __fastcall SecondOpComboBoxExit(TObject *Sender);
   private: 	// User declarations
      void killPartial( void );
      void doGJVEditChange( TObject *Sender );
      void logCurrentContact( );
      void logScreenEntry( );
      BaseContact *getPriorContact();
      BaseContact *getNextContact();
   public: 		// User declarations
      __fastcall TGJVQSOEditFrame( TComponent* Owner );
      virtual void selectEntry( BaseContact *lct );
      virtual void initialise( BaseContestLog * contest, bool catchup );
      bool doGJVOKButtonClick( TObject *Sender );
      void doGJVCancelButtonClick( TObject *Sender );
      virtual void updateQSOTime();
      virtual void updateQSODisplay();
};
//---------------------------------------------------------------------------
#endif
