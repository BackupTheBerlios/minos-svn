/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef TMinosBandChooserH
#define TMinosBandChooserH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TMinosBandChooser : public TForm
{
   __published: 	// IDE-managed Components
      TLabel *ScreedLabel;
      TComboBox *BandCombo;
      TButton *OKButton;
   private: 	// User declarations
   public: 		// User declarations
      __fastcall TMinosBandChooser( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TMinosBandChooser *MinosBandChooser;
//---------------------------------------------------------------------------
#endif
