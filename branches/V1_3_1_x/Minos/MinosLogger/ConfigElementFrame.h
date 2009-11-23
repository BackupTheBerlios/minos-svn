/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------


#ifndef ConfigElementFrameH
#define ConfigElementFrameH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
class TConfigElementFrm : public TFrame
{
   __published:  	// IDE-managed Components
      TCheckBox *RunCheckBox;
      TLabel *AppNameLabel;
   void __fastcall RunCheckBoxClick(TObject *Sender);
   private:  	// User declarations
   public:  		// User declarations
      TConfigElement *tce;
      __fastcall TConfigElementFrm( TComponent* Owner, TConfigElement *tce );
      bool initialise( String sect );

};
//---------------------------------------------------------------------------
#endif
