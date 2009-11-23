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
      TLabel *Label1;
      TComboBox *MainOpComboBox;
      TLabel *Label2;
      TComboBox *SecondOpComboBox;
      void __fastcall InsertBeforeButtonClick( TObject *Sender );
      void __fastcall InsertAfterButtonClick( TObject *Sender );
      void __fastcall FirstUnfilledButtonClick( TObject *Sender );
      void __fastcall PriorButtonClick( TObject *Sender );
      void __fastcall NextButtonClick( TObject *Sender );
      void __fastcall ROFieldTimerTimer( TObject *Sender );
      void __fastcall MainOpComboBoxChange( TObject *Sender );
      void __fastcall SecondOpComboBoxChange( TObject *Sender );
      void __fastcall MainOpComboBoxKeyPress( TObject *Sender, char &Key );
   private: 	// User declarations
      void killPartial( void );
      void doGJVEditChange( TObject *Sender );
      void logCurrentContact( );
      void logScreenEntry( );
      BaseContact *getPriorContact();
   public: 		// User declarations
      __fastcall TGJVQSOEditFrame( TComponent* Owner );
      virtual void selectEntry( BaseContact *lct );
      virtual void initialise( BaseContestLog * contest, QSOEditScreen *edScreen, bool catchup );
      bool doGJVOKButtonClick( TObject *Sender );
      void doGJVCancelButtonClick( TObject *Sender );
      virtual void updateQSOTime();
      MinosItem<std::string> *op1Value;
      MinosItem<std::string> *op2Value;
};
//---------------------------------------------------------------------------
#endif
