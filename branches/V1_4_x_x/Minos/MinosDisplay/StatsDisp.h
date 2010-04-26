/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef StatsDispH
#define StatsDispH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TStatsDispFrame : public TFrame
{
   __published:  	// IDE-managed Components
      TTimer *RecheckTimer;
      TPanel *Panel1;
      TLabeledEdit *P1Edit;
      TLabeledEdit *P2Edit;
      TLabel *SLabel1;
      void __fastcall RecheckTimerTimer( TObject *Sender );
      void __fastcall P2EditKeyPress( TObject *Sender, char &Key );
   void __fastcall P1EditExit(TObject *Sender);
   void __fastcall P2EditExit(TObject *Sender);
   private:  	// User declarations
      BaseContestLog *ct;
   public:  		// User declarations
      void reInitialise();
      void setContest( BaseContestLog *ct );
      __fastcall TStatsDispFrame( TComponent* Owner );
};
//---------------------------------------------------------------------------
#endif
