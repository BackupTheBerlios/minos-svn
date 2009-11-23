/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef ctlmainH
#define ctlmainH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TRigCtlMain : public TForm
{
   __published:  	// IDE-managed Components
      TLabel *Label1;
      TLabel *Label2;
      TButton *CloseButton;
      TTimer *Timer1;
      TTimer *LogTimer;
      TLabeledEdit *RigBase;
      TLabeledEdit *TransvertBase;
      TButton *ChangeButton;
      TComboBox *RigCombo;
      TLabel *Label3;
      TButton *RigSelectButton;
      TComboBox *PortCombo;
      TLabel *Label4;
      TRadioGroup *ModeRadioGroup;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall Timer1Timer( TObject *Sender );
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall ChangeButtonClick( TObject *Sender );
      void __fastcall RigSelectButtonClick( TObject *Sender );
   private:  	// User declarations
      void __fastcall logMessage( std::string s );
      RIG *my_rig;		/* handle to rig (nstance) */
      freq_t rigTransConversion;
   public:  		// User declarations
      __fastcall TRigCtlMain( TComponent* Owner );
      void publishCallback( bool err, MinosRPCObj *mro, const std::string &from );
};
//---------------------------------------------------------------------------
extern PACKAGE TRigCtlMain *RigCtlMain;
//---------------------------------------------------------------------------
#endif
