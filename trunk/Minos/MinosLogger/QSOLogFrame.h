/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef QSOLogFrameH
#define QSOLogFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "QSOFrame.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TGJVQSOLogFrame : public TGJVEditFrame
{
   __published: 	// IDE-managed Components
      TBitBtn *MatchXferButton;
      TPanel *BandMapPanel;
      TButton *BandMapButton;
      TCheckBox *AutoBandmapTune;
      TCheckBox *AutoBandmapTime;
      TLabel *OperatorLabel;
      void __fastcall BandMapButtonClick( TObject *Sender );
      void __fastcall CatchupButtonClick(TObject *Sender);
      void __fastcall FirstUnfilledButtonClick(TObject *Sender);
   private: 	// User declarations
      MinosEventListener  EL_Op1Change;
      void Op1Change_Event ( MinosEventBase & Event );

      MinosEventListener  EL_Op2Change;
      void Op2Change_Event ( MinosEventBase & Event );

      ScreenContact *partialContact; // contact being edited on screen
      String sCurrFreq;

      bool savePartial( void );
      bool restorePartial( void );
      void killPartial( void );
      void doGJVEditChange( TObject *Sender );
      void logCurrentContact();
      void logScreenEntry( );

   public: 		// User declarations
      __fastcall TGJVQSOLogFrame( TComponent* Owner );
      __fastcall ~TGJVQSOLogFrame();
      virtual void startNextEntry( );
      void doGJVCancelButtonClick( TObject *Sender );
      void transferDetails( const BaseContact * lct, const BaseContestLog *matct );
      void transferDetails( const ListContact * lct, const ContactList *matct );
      void initialise( BaseContestLog * contest, bool catchup );
      void setFreq( String f );
      virtual void updateQSOTime();
      bool isCurrentLog;
};
//---------------------------------------------------------------------------
#endif
