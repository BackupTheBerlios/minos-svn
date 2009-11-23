/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef WindowsMonitorH
#define WindowsMonitorH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TWindowsMonitorForm : public TForm
{
   __published: 	// IDE-managed Components
      TCheckBox *PTTInCheckBox;
      TCheckBox *L1CheckBox;
      TCheckBox *L2CheckBox;
      TTimer *StartupTimer;
      void __fastcall FormCloseQuery( TObject *Sender, bool &CanClose );
      void __fastcall StartupTimerTimer( TObject *Sender );
   private: 	// User declarations
   public: 		// User declarations
      __fastcall TWindowsMonitorForm( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TWindowsMonitorForm *WindowsMonitorForm;
//---------------------------------------------------------------------------
#endif
