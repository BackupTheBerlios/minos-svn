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
// a pure interface class to be implemented by the parent of QSOFrame descendants
class QSOEditScreen
{
   public:
      virtual void showScreenEntry( ScreenContact &screenContact ) = 0;
      virtual void getScreenEntry( ScreenContact &screenContact ) = 0;
      virtual void afterSelectEntry( BaseContact *lct ) = 0;
      virtual TWinControl *getActiveControl() = 0;
      virtual void reportOverstrike( bool overstrike ) = 0;
      virtual void afterLogContact() = 0;
};
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
   TRadioGroup *BandChoiceRadioGroup;
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
   private: 	// User declarations

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
      virtual void logCurrentContact( bool forcelog ) = 0;
      virtual void killPartial( void ) = 0;
      virtual void doGJVEditChange( TObject *Sender ) = 0;

      void setScoreText( int dist, bool partial, bool xband );
      bool dlgForced();
      void valtrace( int mess_no, bool flag );
      bool isErrSet( int mess_no );
      bool validateControls( validTypes command );
      void contactValid( void );
   protected: 	// User declarations
      BaseContestLog * contest;
      QSOEditScreen *editScreen;
      bool overstrike;
      TWinControl *current;
      ErrorList errs;
      bool updateTimeAllowed;

      bool valid( validTypes command );
      void calcLoc( void );
      void doAutofill( void );
      void fillRst( TLabeledEdit *rIl, std::string &rep, const std::string &mode );
      void getScreenEntry();
      void showScreenEntry( );
      bool checkLogEntry( );
      void selectField( TWinControl *v );
   public: 		// User declarations
      ScreenContact screenContact;  // contact being edited on screen
      BaseContact *selectedContact;   // contact from log list selected

      __fastcall TGJVEditFrame( TComponent* Owner );
      __fastcall ~TGJVEditFrame();
      void setActiveControl( WORD &Key );
      void setMode( String m );
      void clearCurrentField();
      void lgTraceerr( int err );
      void setTimeNow();

      virtual void selectEntry( DisplayContestContact *lct ) = 0;
      virtual void initialise( BaseContestLog * contest, QSOEditScreen *edScreen );
      virtual bool doGJVOKButtonClick( TObject *Sender );
      virtual bool doGJVForceButtonClick( TObject *Sender );
      virtual bool doGJVCancelButtonClick( TObject *Sender ) = 0;
      virtual void updateQSOTime() = 0;
      virtual void updateQSODisplay();
};
//---------------------------------------------------------------------------
#endif
