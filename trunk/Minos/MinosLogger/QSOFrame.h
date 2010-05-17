/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef QSOFrameH
#define QSOFrameH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TGJVEditFrame : public TFrame
{
   __published: 	// IDE-managed Components
      TPanel *G0GJVPanel;
      TLabel *ModeLabel;
      TLabel *BrgSt;
      TLabel *DistSt;
      TLabeledEdit *TimeEdit;
      TLabeledEdit *CallsignEdit;
      TLabeledEdit *RSTTXEdit;
      TLabeledEdit *SerTXEdit;
      TLabeledEdit *RSTRXEdit;
      TLabeledEdit *SerRXEdit;
      TLabeledEdit *LocEdit;
      TLabeledEdit *QTHEdit;
      TLabeledEdit *CommentsEdit;
      TComboBox *ModeComboBoxGJV;
      TButton *GJVOKButton;
      TButton *GJVForceButton;
      TButton *GJVCancelButton;
      TLabeledEdit *DateEdit;
      TCheckBox *NonScoreCheckBox;
      TCheckBox *DeletedCheckBox;
      TButton *TimeNowButton;
      TButton *ModeButton;
      TButton *FirstUnfilledButton;
      TButton *CatchupButton;
      TComboBox *SecondOpComboBox;
      TLabel *SecondOpLabel;
      TComboBox *MainOpComboBox;
      TLabel *OperatorLabel;
      void __fastcall EditControlEnter( TObject *Sender );
      void __fastcall EditControlExit( TObject *Sender );
      void __fastcall EditKeyDown( TObject *Sender, WORD &Key,
                                   TShiftState Shift );
      void __fastcall DateEditDblClick( TObject *Sender );
      void __fastcall TimeEditDblClick( TObject *Sender );
      void __fastcall TimeEditKeyPress( TObject *Sender, char &Key );
      void __fastcall GJVEditKeyPress( TObject *Sender, char &Key );
      void __fastcall SerTXEditDblClick( TObject *Sender );
      void __fastcall GJVEditChange( TObject *Sender );
      void __fastcall DateEditChange( TObject *Sender );
      void __fastcall TimeEditChange( TObject *Sender );
      void __fastcall SerTXEditChange( TObject *Sender );
      void __fastcall LocEditChange( TObject *Sender );
      void __fastcall TimeNowButtonClick(TObject *Sender);
      void __fastcall ModeButtonClick(TObject *Sender);
   private: 	// User declarations
      MinosEventListener  EL_ValidateError;
      void ValidateError_Event ( MinosEventBase & Event );
      ErrorList errs;

      ValidatedControl *dateIl;
      ValidatedControl *timeIl;
      ValidatedControl *csIl;
      ValidatedControl *rsIl, *ssIl, *rrIl, *srIl;
      ValidatedControl *locIl;
      ValidatedControl *qthIl;
      ValidatedControl *cmntIl;

      std::vector <ValidatedControl *> vcs;

      String oldloc;
      bool locValid;

      virtual void logScreenEntry( ) = 0;
      virtual void logCurrentContact( ) = 0;
      virtual void killPartial( void ) = 0;
      virtual void doGJVEditChange( TObject *Sender ) = 0;

      void setScoreText( int dist, bool partial, bool xband );
      bool dlgForced();
      bool validateControls( validTypes command );
      void contactValid( void );
   protected: 	// User declarations
      BaseContestLog * contest;
      bool overstrike;
      TWinControl *current;
      bool updateTimeAllowed;
      bool catchup;

      bool valid( validTypes command );
      void calcLoc( void );
      void doAutofill( void );
      void fillRst( TLabeledEdit *rIl, std::string &rep, const std::string &mode );
      void getScreenEntry();
      void showScreenEntry( );
      bool checkLogEntry(bool checkDTG );
   public: 		// User declarations
      ScreenContact screenContact;  // contact being edited on screen
      BaseContact *selectedContact;   // contact from log list selected

      __fastcall TGJVEditFrame( TComponent* Owner );
      __fastcall ~TGJVEditFrame();
      void setActiveControl( WORD *Key );
      void setMode( String m );
      void clearCurrentField();
      void lgTraceerr( int err );
      void setTimeNow();

      void selectField( TWinControl *v );
      virtual void initialise( BaseContestLog * contest, bool catchup );
      virtual void refreshOps();
      virtual bool doGJVOKButtonClick( TObject *Sender );
      virtual bool doGJVForceButtonClick( TObject *Sender );
      virtual void doGJVCancelButtonClick( TObject *Sender ) = 0;
      virtual void updateQSOTime() = 0;
      virtual void updateQSODisplay() = 0;
      virtual void closeContest();
};
//---------------------------------------------------------------------------
#endif
