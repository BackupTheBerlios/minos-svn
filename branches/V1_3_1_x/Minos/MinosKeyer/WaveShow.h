/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef WaveShowH
#define WaveShowH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TWaveShowForm : public TForm
{
   __published: 	// IDE-managed Components
      TPanel *Panel1;
      TPaintBox *WavPanel;
      TScrollBar *VScrollBar;
      TScrollBar *HScrollBar;
      void __fastcall WavPanelPaint( TObject *Sender );
      void __fastcall VScrollBarScroll( TObject *Sender, TScrollCode ScrollCode,
                                        int &ScrollPos );
      void __fastcall HScrollBarScroll( TObject *Sender, TScrollCode ScrollCode,
                                        int &ScrollPos );
      void __fastcall HScrollBarChange( TObject *Sender );
      void __fastcall VScrollBarChange( TObject *Sender );
      void __fastcall FormDestroy( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
   private: 	// User declarations
      INT16 *sample;
      int WavWidth;
      int Position;
      double Scale;
   public: 		// User declarations
      __fastcall TWaveShowForm( TComponent* Owner );
      void setSamples( int fno );
};
//---------------------------------------------------------------------------
extern PACKAGE TWaveShowForm *WaveShowForm;
//---------------------------------------------------------------------------
#endif
